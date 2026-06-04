#pragma once

#include "BusMessages.hpp"
#include <mavlink/standard/mavlink.h>
#include <span>

namespace hal::mavlink_utils {

/**
 * @brief Helper pour sérialiser les commandes actionneurs internes en messages MAVLink.
 */
inline mavlink_message_t pack_actuator_cmd(const bus::ActuatorCmd& cmd) {
    mavlink_message_t msg;
    // Mapping: bus::ActuatorCmd -> SET_ACTUATOR_CONTROL_TARGET
    // Note: Les indices peuvent varier selon le simulateur (AirSim attend souvent un tableau de 8)
    mavlink_msg_set_actuator_control_target_pack(
        1, 1, &msg, 
        0, // time_usec (à implémenter via SystemClock)
        0, // group_mlx
        cmd.channels[0], cmd.channels[1], cmd.channels[2], cmd.channels[3],
        0, 0, 0, 0
    );
    return msg;
}

/**
 * @brief Helper pour désérialiser les messages MAVLink en messages internes.
 */
inline bus::RcFrame unpack_rc_channels(const mavlink_rc_channels_t& rc) {
    bus::RcFrame frame;
    frame.channels[0] = rc.chan1_raw;
    frame.channels[1] = rc.chan2_raw;
    frame.channels[2] = rc.chan3_raw;
    frame.channels[3] = rc.chan4_raw;
    return frame;
}

} // namespace hal::mavlink_utils
