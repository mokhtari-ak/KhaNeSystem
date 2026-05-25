#pragma once

#include "stm32f4xx_hal.h"
#include "HalTypes.hpp"
#include <array>
#include <optional>

namespace hal {

// Remplacement du vecteur dynamique par une structure statique
// Adapté au STM32F407VG (SPI1, SPI2, SPI3)
struct SpiInstance {
    SPI_HandleTypeDef handle;
    bool initialized = false;
};

class SpiDriver {
public:
    static constexpr size_t MaxInstances = 3;

    // Initialisation statique sans allocation dynamique
    template<typename Config>
    static Result<uint8_t> init() noexcept {
        // Validation des index et config...
        // Utilisation de la table statique
        static std::array<SpiInstance, MaxInstances> instances{};
        
        // ... Logique d'init HAL ...
        return 0; // Index de l'instance
    }

    static Result<void> transmit_receive(uint8_t index, std::span<uint8_t> tx, std::span<uint8_t> rx, Microseconds timeout) noexcept {
        // Logique HAL non bloquante utilisant les handles statiques
        return {};
    }

private:
    // Callback table statique au lieu de std::map<std::function>
    using CallbackFunc = void(*)();
    static inline std::array<CallbackFunc, MaxInstances> tx_complete_callbacks{};
};

} // namespace hal
