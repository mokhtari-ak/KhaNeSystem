#pragma once

#include "BusMessages.hpp"
#include "EventBus.hpp"
#include "mavlink/standard/mavlink.h"
#include "UartDriver.hpp"
#include <array>
#include <atomic>

namespace rf {

// Buffer circulaire pour l'envoi DMA non-bloquant
class MavlinkTransmitter {
public:
    static constexpr size_t BufferSize = 512;

    void transmit(const mavlink_message_t& msg, int8_t uart_handle) {
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t len = mavlink_msg_to_send_buffer(buffer, &msg);
        
        // Utilisation du driver UART avec DMA
        hal::HalUartDriver::transmit(uart_handle, buffer, len, 100);
    }
};

} // namespace rf
