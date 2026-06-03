#pragma once

#include "RtosAbstract.hpp"
#include "EventBus.hpp"
#include "BusMessages.hpp"
#include "ImuDriver.hpp"
#include "BarometerDriver.hpp"
#include "SensorFusion.hpp"

// Backend RTOS pour les files génériques
#ifdef USE_THREADX
    #include "ThreadXWrapper.hpp"
    template<typename T, size_t D> using RtosQueue = rtos::threadx::Queue<T, D>;
#else
    #include "FreeRtosPrimitives.hpp"
    template<typename T, size_t D> using RtosQueue = rtos::freertos::Queue<T, D>;
#endif

#ifdef SIL_TARGET
    #include "AirSimTransport.hpp"
#else
    #include "SpiWrapper.hpp"
#endif

namespace system_init {

// Instances globales ou statiques
inline bus::EventBus<bus::SensorFrame> sensor_bus;
inline RtosQueue<bus::SensorFrame, 8> ekf_queue;
inline bus::QueueAdapter<bus::SensorFrame, decltype(ekf_queue)> ekf_subscriber(ekf_queue);

// Configuration des drivers (ex: SPI1)
#ifdef SIL_TARGET
    inline hal::AirSimTransport transport;
#else
    extern SPI_HandleTypeDef hspi1;
    inline hal::SpiWrapper transport(&hspi1);
#endif

inline hal::ImuDriver<decltype(transport)> imu_driver(transport);

void init_sensors() {
    sensor_bus.subscribe(ekf_subscriber);
}

void sensor_task_loop() {
    auto imu_data = imu_driver.update();
    
    if (imu_data) {
        bus::SensorFrame frame{
            .header = { .version = 1, .timestamp = hal::Microseconds(0) },
            .accel = imu_data->accel,
            .gyro = imu_data->gyro,
            .flags = 0
        };
        sensor_bus.publish(frame);
    }
}

} // namespace system_init
