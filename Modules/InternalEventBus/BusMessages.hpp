#pragma once

#include <cstdint>
#include "HalTypes.hpp"
#include "Eigen/Core"

namespace bus {

// Header commun pour tous les messages du bus
struct MessageHeader {
    uint32_t version;
    hal::Microseconds timestamp;
};

// --- Types de données principaux ---

struct alignas(4) SensorFrame {
    MessageHeader header;
    Eigen::Vector3f accel;
    Eigen::Vector3f gyro;
    float barometer_alt;
    float airspeed;
    uint32_t flags;
};

struct alignas(4) GnssFrame {
    MessageHeader header;
    int32_t lat;
    int32_t lon;
    float alt;
    float vel_north;
    float vel_east;
    float vel_down;
    uint16_t hdop;
    uint8_t fix_type;
    uint8_t satellites;
};

struct alignas(4) StateVector {
    MessageHeader header;
    Eigen::Quaternionf attitude;
    Eigen::Vector3f position;
    Eigen::Vector3f velocity;
    Eigen::Vector3f gyro_bias;
};

struct alignas(4) ActuatorCmd {
    MessageHeader header;
    std::array<float, 8> channels; // Normalisé [-1.0, 1.0]
};

struct alignas(4) RcFrame {
    MessageHeader header;
    std::array<float, 8> channels;
    bool signal_present;
    bool failsafe;
};

struct alignas(4) NavGuidance {
    MessageHeader header;
    float target_heading;
    float target_alt;
    float target_speed;
    float xte; // Cross Track Error
};

struct alignas(4) FaultEvent {
    MessageHeader header;
    uint32_t fault_mask;
    uint8_t source_id;
};

struct alignas(4) PowerStatus {
    MessageHeader header;
    float voltage;
    float current;
    uint8_t soc_percent;
    uint16_t time_remaining_min;
};

// Vérification statique des tailles (limite 128 octets)
static_assert(sizeof(SensorFrame) <= 128);
static_assert(sizeof(GnssFrame) <= 128);
static_assert(sizeof(StateVector) <= 128);
static_assert(sizeof(ActuatorCmd) <= 128);
static_assert(sizeof(RcFrame) <= 128);
static_assert(sizeof(NavGuidance) <= 128);
static_assert(sizeof(FaultEvent) <= 128);
static_assert(sizeof(PowerStatus) <= 128);

} // namespace bus
