#pragma once

#include "RtosAbstract.hpp"
#include "BusMessages.hpp"
#include "EventBus.hpp"
#include "MavlinkTransmitter.hpp"
#include "MavlinkEncoder.hpp"
#include "UartConfigPolicy.hpp"

namespace modules::rf {

template<typename UartPolicy>
class TelemetryTask {
public:
    TelemetryTask(bus::EventBus<bus::StateVector>& state_bus,
                  bus::EventBus<bus::SensorFrame>& sensor_bus)
        : state_bus_(state_bus), sensor_bus_(sensor_bus) {
        
        state_bus_.subscribe(state_subscriber_);
        sensor_bus_.subscribe(sensor_subscriber_);
    }

    void init() {
        uart_handle_ = hal::HalUartDriver::init<UartPolicy>();
    }

    void run() {
        while (true) {
            auto state = state_queue_.pop(hal::Microseconds(100));
            
            if (state) {
                // Utilisation de l'encoder pour MAVLink
                auto msg = rf::mavlink::encode_message(1, 1, *state);
                transmitter_.transmit(msg, uart_handle_);
            }
            
            vTaskDelay(pdMS_TO_TICKS(100)); // 10 Hz
        }
    }

private:
    bus::EventBus<bus::StateVector>& state_bus_;
    bus::EventBus<bus::SensorFrame>& sensor_bus_;
    
    rtos::freertos::Queue<bus::StateVector, 10> state_queue_;
    rtos::freertos::Queue<bus::SensorFrame, 10> sensor_queue_;

    bus::QueueAdapter<bus::StateVector, decltype(state_queue_)> state_subscriber_{state_queue_};
    bus::QueueAdapter<bus::SensorFrame, decltype(sensor_queue_)> sensor_subscriber_{sensor_queue_};
    
    int8_t uart_handle_;
    rf::MavlinkTransmitter transmitter_;
};

} // namespace modules::rf
