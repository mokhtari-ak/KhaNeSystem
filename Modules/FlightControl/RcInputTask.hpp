#pragma once

#include "RtosAbstract.hpp"
#include "BusMessages.hpp"
#include "EventBus.hpp"
#include "UartDriver.hpp"
#include "UartConfigPolicy.hpp"

namespace modules::flight {

template<typename UartPolicy>
class RcInputTask {
public:
    RcInputTask(bus::EventBus<bus::RcFrame>& bus) : bus_(bus) {}

    void init() {
        uart_handle_ = uart_.init<UartPolicy>();
        // Activation des interruptions pour réception UART non-bloquante
    }

    void run() {
        while (true) {
            // Lecture UART et décodage SBUS/CRSF
            // ... décodage ...
            bus::RcFrame frame;
            frame.header.msg_id = bus::MessageType::RcFrame;
            // Remplissage frame.channels...
            
            bus_.publish(frame);
            
            // Délai pour 200 Hz (5ms)
            vTaskDelay(pdMS_TO_TICKS(5));
        }
    }

private:
    bus::EventBus<bus::RcFrame>& bus_;
    hal::HalUartDriver uart_;
    int8_t uart_handle_;
};

} // namespace modules::flight
