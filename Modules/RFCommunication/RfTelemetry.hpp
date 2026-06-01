#pragma once

#include "BusMessages.hpp"
#include "EventBus.hpp"
#include "mavlink/standard/mavlink.h"
#include "UartDriver.hpp"

namespace rf {

class RfTelemetry {
public:
    RfTelemetry(bus::EventBus<bus::StateVector>& bus, int8_t uart_handle) 
        : bus_(bus), uart_handle_(uart_handle) {}

    void send_heartbeat() noexcept {
        mavlink_message_t msg;
        mavlink_msg_heartbeat_pack(1, 1, &msg, MAV_TYPE_FIXED_WING, MAV_AUTOPILOT_GENERIC, 
                                   MAV_MODE_PREFLIGHT, 0, MAV_STATE_STANDBY);
        
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t len = mavlink_msg_to_send_buffer(buffer, &msg);
        
        // Envoi via driver statique
        hal::HalUartDriver::transmit(uart_handle_, buffer, len, 1000);
    }

private:
    bus::EventBus<bus::StateVector>& bus_;
    int8_t uart_handle_;
};

} // namespace rf
