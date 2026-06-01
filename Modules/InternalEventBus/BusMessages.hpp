#pragma once

#include <cstdint>
#include <array>
#include <type_traits>
#include "HalTypes.hpp"
#include "Eigen/Core"

namespace bus {

// Enum pour identifier les types de messages
enum class MessageType : uint32_t {
    SensorFrame = 0,
    GnssFrame,
    StateVector,
    ActuatorCmd,
    RcFrame,
    NavGuidance,
    FaultEvent,
    PowerStatus
};

// Header commun
struct MessageHeader {
    uint32_t version;
    MessageType msg_id;
    hal::Microseconds timestamp;
};

// --- Types de données principaux ---

struct alignas(4) SensorFrame {
    MessageHeader header;
    Eigen::Vector3f accel;
    Eigen::Vector3f gyro;
    float barometer_alt;
    float airspeed;
    uint32_t flags; // Flags de validité : BIT0=ACCEL, BIT1=GYRO, BIT2=BARO, BIT3=AIRSPEED
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
    uint8_t fix_type; // 0=NONE, 1=FIX, 2=DGPS
    uint8_t satellites;
    uint32_t flags; // Flags de validité
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
    std::array<float, 8> channels;
};

struct alignas(4) RcFrame {
    MessageHeader header;
    std::array<float, 8> channels;
    bool signal_present;
    bool failsafe;
    uint8_t padding[2]; // Alignement
};

struct alignas(4) NavGuidance {
    MessageHeader header;
    float target_heading;
    float target_alt;
    float target_speed;
    float xte;
};

struct alignas(4) FaultEvent {
    MessageHeader header;
    uint32_t fault_mask;
    uint8_t source_id;
    uint8_t padding[3];
};

struct alignas(4) PowerStatus {
    MessageHeader header;
    float voltage;
    float current;
    uint8_t soc_percent;
    uint16_t time_remaining_min;
    uint8_t padding[1];
};

// Vérification statique : Taille, POD, et trivialité
#define VALIDATE_MESSAGE(Type) \
    static_assert(sizeof(Type) <= 128, "Message exceeds 128 bytes"); \
    static_assert(std::is_trivially_copyable_v<Type>, "Message must be trivially copyable (POD)")

VALIDATE_MESSAGE(SensorFrame);
VALIDATE_MESSAGE(GnssFrame);
VALIDATE_MESSAGE(StateVector);
VALIDATE_MESSAGE(ActuatorCmd);
VALIDATE_MESSAGE(RcFrame);
VALIDATE_MESSAGE(NavGuidance);
VALIDATE_MESSAGE(FaultEvent);
VALIDATE_MESSAGE(PowerStatus);

} // namespace bus
