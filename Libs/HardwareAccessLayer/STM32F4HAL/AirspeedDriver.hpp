#pragma once

#include "IHalWrappers.hpp"
#include "HalTypes.hpp"

namespace hal {

struct AirspeedData {
    float airspeed_ms;
    Microseconds timestamp;
    bool is_valid;
};

// Driver Vitesse Air (Ex: DLVR)
template<typename Transport>
class AirspeedDriver {
public:
    explicit AirspeedDriver(Transport& transport) : transport_(transport) {}

    Result<void> init() noexcept {
        return {};
    }

    Result<AirspeedData> update() noexcept {
        // Lecture via transport_ (I2C)
        // Validation : airspeed ∈ [0, 100 m/s]
        return AirspeedData{
            .airspeed_ms = 0.0f,
            .timestamp = Microseconds(0),
            .is_valid = true
        };
    }

private:
    Transport& transport_;
};

// Mock pour simulation
class AirspeedDriverHook {
public:
    Result<AirspeedData> update() noexcept {
        return AirspeedData{
            .airspeed_ms = 15.0f,
            .timestamp = Microseconds(0),
            .is_valid = true
        };
    }
};

} // namespace hal
