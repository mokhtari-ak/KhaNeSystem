#pragma once

#include "IHalWrappers.hpp"
#include "HalTypes.hpp"

namespace hal {

struct GnssData {
    int32_t lat;
    int32_t lon;
    float alt;
    uint8_t fix_type;
    uint8_t satellites;
    Microseconds timestamp;
    bool is_valid;
};

template<typename Transport>
class GnssDriver {
public:
    explicit GnssDriver(Transport& transport) : transport_(transport) {}

    Result<void> init() noexcept {
        return {};
    }

    Result<GnssData> update() noexcept {
        return GnssData{
            .lat = 0, .lon = 0, .alt = 0.0f,
            .fix_type = 0, .satellites = 0,
            .timestamp = Microseconds(0),
            .is_valid = false
        };
    }

private:
    Transport& transport_;
};

} // namespace hal
