#pragma once

#include "RtosAbstract.hpp"
#include "BusMessages.hpp"
#include "EventBus.hpp"
#include "ImuDriver.hpp"
#include "BarometerDriver.hpp"
#include "MagnetometerDriver.hpp"
#include "GnssDriver.hpp"
#include "AirspeedDriver.hpp"
#include "SystemClock.hpp"

namespace modules::sensors {

template<typename Imu, typename Baro, typename Mag, typename Gnss, typename Air>
class SensorTask {
public:
    SensorTask(bus::EventBus<bus::SensorFrame>& bus, 
               Imu& imu, Baro& baro, Mag& mag, Gnss& gnss, Air& air) 
        : bus_(bus), imu_(imu), baro_(baro), mag_(mag), gnss_(gnss), air_(air) {}

    void init() {
        imu_.init();
        baro_.init();
        mag_.init();
        gnss_.init();
        air_.init();
    }

    void run() {
        while (true) {
            auto imu_data = imu_.update();
            auto baro_data = baro_.update();
            
            if (imu_data) {
                bus::SensorFrame frame;
                frame.header.msg_id = bus::MessageType::SensorFrame;
                frame.header.timestamp = hal::SystemClock::now_us();
                frame.accel = imu_data->accel;
                frame.gyro = imu_data->gyro;
                frame.barometer_alt = baro_data ? baro_data->altitude : 0.0f;
                frame.flags = (imu_data->is_valid ? 0x03 : 0x00) | (baro_data ? 0x04 : 0x00);

                bus_.publish(frame);
            }
            
            // 1000 Hz loop
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }

private:
    bus::EventBus<bus::SensorFrame>& bus_;
    Imu& imu_;
    Baro& baro_;
    Mag& mag_;
    Gnss& gnss_;
    Air& air_;
};

} // namespace modules::sensors
