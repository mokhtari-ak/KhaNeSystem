#pragma once

#include "ISimulationBridge.hpp"
#include "MavlinkTypes.hpp"
#include <mavlink/standard/mavlink.h>
#include <iostream>

namespace hal {

/**
 * @brief Implémentation concrète du pont SIL via MAVLink UDP.
 */
class MavlinkBridge : public ISimulationBridge {
public:
    MavlinkBridge() {
        // TODO: Initialiser le socket UDP ici
        // socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    }
    
    ~MavlinkBridge() override {
        // TODO: Fermer le socket
    }

    Result<void> send(std::span<const uint8_t> data) noexcept override {
        // TODO: Remplacer par l'envoi socket réel
        // sendto(socket_fd, data.data(), data.size(), ...);
        return {};
    }

    Result<size_t> receive(std::span<uint8_t> buffer) noexcept override {
        // TODO: Remplacer par la réception socket réelle
        // return recvfrom(socket_fd, buffer.data(), buffer.size(), ...);
        return 0;
    }

    bool is_connected() const noexcept override {
        return true; 
    }
    
private:
    int socket_fd_ = -1;
};

} // namespace hal
