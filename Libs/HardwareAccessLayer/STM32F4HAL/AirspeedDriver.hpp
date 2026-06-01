#pragma once

#include "IHalWrappers.hpp"
#include "HalTypes.hpp"

namespace hal {

struct AirspeedData {
    float airspeed;
    float temperature;
    Microseconds timestamp;
    bool is_valid;
};

template<typename Transport>
class AirspeedDriver {
public:
    explicit AirspeedDriver(Transport& transport) : transport_(transport) {}

    Result<void> init() noexcept {
        return {};
    }

    Result<AirspeedData> update() noexcept {
        return AirspeedData{
            .airspeed = 0.0f,
            .temperature = 25.0f,
            .timestamp = Microseconds(0),
            .is_valid = true
        };
    }

private:
    Transport& transport_;
};

} // namespace hal
