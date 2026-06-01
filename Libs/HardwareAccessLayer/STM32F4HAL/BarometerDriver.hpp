#pragma once

#include "IHalWrappers.hpp"
#include "HalTypes.hpp"
#include "I2cDriver.hpp"

namespace hal {

struct BaroData {
    float altitude;
    float pressure;
    Microseconds timestamp;
    bool is_valid;
};

template<typename Transport>
class BarometerDriver {
public:
    explicit BarometerDriver(Transport& transport) : transport_(transport) {}

    Result<void> init() noexcept {
        // Init I2C/SPI Baro (ex: MS5611)
        return {};
    }

    Result<BaroData> update() noexcept {
        return BaroData{
            .altitude = 0.0f,
            .pressure = 1013.25f,
            .timestamp = Microseconds(0),
            .is_valid = true
        };
    }

private:
    Transport& transport_;
};

} // namespace hal
