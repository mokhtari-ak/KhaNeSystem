#pragma once

#define MAVLINK_USE_CONVENIENCE_FUNCTIONS
#define MAVLINK_CRC_EXTRA 1

#include "mavlink/standard/mavlink.h"

namespace rf::mavlink {

inline mavlink_message_t encode_message(uint8_t system_id, uint8_t component_id, const bus::StateVector& state) {
    mavlink_message_t msg;
    mavlink_msg_attitude_pack(system_id, component_id, &msg, 
                              0, // timestamp
                              state.attitude.w(), state.attitude.x(), 
                              state.attitude.y(), state.attitude.z(), 
                              0, 0, 0); // gyro
    return msg;
}

} // namespace rf::mavlink
