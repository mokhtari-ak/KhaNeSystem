#pragma once

#include "IHalWrappers.hpp"
#include "HalTypes.hpp"
#include <Eigen/Core>

namespace hal {

struct GnssData {
    Eigen::Vector3d position_ned; // Lat, Lon, Alt
    Eigen::Vector3f velocity_ned;
    float hdop;
    uint8_t fix_type;
    uint8_t satellites;
    Microseconds timestamp;
    bool is_valid;
};

// Driver GNSS (Ex: M8N)
template<typename Transport>
class GnssDriver {
public:
    explicit GnssDriver(Transport& transport) : transport_(transport) {}

    Result<void> init() noexcept {
        // Configuration UART + envoi messages UBX pour paramétrage
        return {};
    }

    Result<GnssData> update() noexcept {
        // Lecture non-bloquante via transport_ (UART DMA), parsing NMEA/UBX
        // Validation : HDOP < 2.5, fix 3D, vitesse < 300 m/s
        return GnssData{
            .position_ned = Eigen::Vector3d::Zero(),
            .velocity_ned = Eigen::Vector3f::Zero(),
            .hdop = 1.0f,
            .fix_type = 3,
            .satellites = 10,
            .timestamp = Microseconds(0),
            .is_valid = true
        };
    }

private:
    Transport& transport_;
};

// Mock pour simulation
class GnssDriverHook {
public:
    Result<GnssData> update() noexcept {
        return GnssData{
            .position_ned = Eigen::Vector3d(48.8566, 2.3522, 100.0),
            .velocity_ned = Eigen::Vector3f::Zero(),
            .hdop = 0.8f,
            .fix_type = 3,
            .satellites = 12,
            .timestamp = Microseconds(0),
            .is_valid = true
        };
    }
};

} // namespace hal
