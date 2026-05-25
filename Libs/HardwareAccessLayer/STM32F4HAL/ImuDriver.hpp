#pragma once

#include "IHalWrappers.hpp"
#include "HalTypes.hpp"
#include <Eigen/Core>

namespace hal {

struct SensorData {
    Eigen::Vector3f accel;
    Eigen::Vector3f gyro;
    Microseconds timestamp;
    bool is_valid;
};

// Concept pour IMU (Ex: MPU6000)
template<typename Transport>
class ImuDriver {
public:
    explicit ImuDriver(Transport& transport) : transport_(transport) {}

    Result<void> init() noexcept {
        // Envoi séquence de réveil + auto-test
        return {};
    }

    Result<SensorData> update() noexcept {
        // Lecture via transport_ (SPI DMA), validation, et conversion en float
        // Vérification NaN, Saturation, Jitter
        return SensorData{
            .accel = Eigen::Vector3f::Zero(),
            .gyro = Eigen::Vector3f::Zero(),
            .timestamp = Microseconds(0),
            .is_valid = true
        };
    }

private:
    Transport& transport_;
};

// Mock pour simulation
class ImuDriverHook {
public:
    Result<SensorData> update() noexcept {
        return SensorData{
            .accel = Eigen::Vector3f(0.0f, 0.0f, 9.81f),
            .gyro = Eigen::Vector3f::Zero(),
            .timestamp = Microseconds(0),
            .is_valid = true
        };
    }
};

} // namespace hal
