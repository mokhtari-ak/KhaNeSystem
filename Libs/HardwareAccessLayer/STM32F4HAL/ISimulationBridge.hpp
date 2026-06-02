#pragma once

#include "HalTypes.hpp"
#include <span>
#include <cstdint>

namespace hal {

/**
 * @brief Interface pour le pont de communication avec le simulateur (SIL/HITL).
 */
class ISimulationBridge {
public:
    virtual ~ISimulationBridge() = default;

    // Envoie des données au simulateur
    virtual Result<void> send(std::span<const uint8_t> data) noexcept = 0;

    // Reçoit des données du simulateur
    virtual Result<size_t> receive(std::span<uint8_t> buffer) noexcept = 0;

    // Vérifie si la connexion est établie
    virtual bool is_connected() const noexcept = 0;
};

} // namespace hal
