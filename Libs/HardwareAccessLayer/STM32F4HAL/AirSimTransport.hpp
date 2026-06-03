#pragma once

#include "IHalWrappers.hpp"
#include "HalTypes.hpp"

namespace hal {

// Bridge vers l'API AirSim (simulé via interface Socket)
class AirSimTransport {
public:
    // Simulation de la lecture SPI via RPC AirSim
    Result<void> transmit_receive(std::span<uint8_t> tx, std::span<uint8_t> rx, Microseconds timeout) noexcept {
        // En mode SIL/HITL, nous interceptons l'appel SPI
        // Ici, nous devrions appeler l'API AirSim (ex: rpc_client.get_imu_data())
        // Pour cet exemple, on remplit le buffer rx avec des données simulées
        for (auto& byte : rx) {
            byte = 0x55; 
        }
        return {};
    }
};

} // namespace hal
