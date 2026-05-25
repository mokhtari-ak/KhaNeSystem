#pragma once

#include "IHalWrappers.hpp"
#include "HalTypes.hpp"

namespace hal {

struct BaroData {
    float pressure_pa;
    float altitude_m;
    Microseconds timestamp;
    bool is_valid;
};

// Driver Baromètre (Ex: MS5611)
template<typename Transport>
class BarometerDriver {
public:
    explicit BarometerDriver(Transport& transport) : transport_(transport) {}

    Result<void> init() noexcept {
        // Envoi séquence de réinitialisation PROM
        return {};
    }

    Result<BaroData> update() noexcept {
        // Lecture ADC via transport_ (SPI), conversion press/temp
        // Validation : [-500m, 9000m], variation < 50m/s
        return BaroData{
            .pressure_pa = 101325.0f,
            .altitude_m = 0.0f,
            .timestamp = Microseconds(0),
            .is_valid = true
        };
    }

private:
    Transport& transport_;
};

// Mock pour simulation
class BarometerDriverHook {
public:
    Result<BaroData> update() noexcept {
        return BaroData{
            .pressure_pa = 101325.0f,
            .altitude_m = 10.0f,
            .timestamp = Microseconds(0),
            .is_valid = true
        };
    }
};

} // namespace hal
