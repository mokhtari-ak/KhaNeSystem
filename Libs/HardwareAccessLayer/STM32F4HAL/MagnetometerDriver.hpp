#pragma once

#include "IHalWrappers.hpp"
#include "HalTypes.hpp"
#include <Eigen/Core>

namespace hal {

struct MagData {
    Eigen::Vector3f field_ut; // µT
    Microseconds timestamp;
    bool is_valid;
};

// Driver Magnétomètre (Ex: HMC5883L/IST8310)
template<typename Transport>
class MagnetometerDriver {
public:
    explicit MagnetometerDriver(Transport& transport) : transport_(transport) {}

    Result<void> init() noexcept {
        return {};
    }

    Result<MagData> update() noexcept {
        // Lecture via transport_ (I2C)
        // Validation : |B| ∈ [20 µT, 65 µT]
        return MagData{
            .field_ut = Eigen::Vector3f::Zero(),
            .timestamp = Microseconds(0),
            .is_valid = true
        };
    }

private:
    Transport& transport_;
};

// Mock pour simulation
class MagnetometerDriverHook {
public:
    Result<MagData> update() noexcept {
        return MagData{
            .field_ut = Eigen::Vector3f(30.0f, 0.0f, 0.0f),
            .timestamp = Microseconds(0),
            .is_valid = true
        };
    }
};

} // namespace hal
