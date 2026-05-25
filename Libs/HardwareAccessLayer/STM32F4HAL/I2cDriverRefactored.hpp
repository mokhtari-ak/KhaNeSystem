#pragma once

#include "stm32f4xx_hal.h"
#include "HalTypes.hpp"
#include <array>

namespace hal {

struct I2cInstance {
    I2C_HandleTypeDef handle;
    bool initialized = false;
};

class I2cDriver {
public:
    static constexpr size_t MaxInstances = 3; // I2C1, I2C2, I2C3

    template<typename Config>
    static Result<uint8_t> init() noexcept {
        static std::array<I2cInstance, MaxInstances> instances{};
        // Logique d'init statique ici
        return 0;
    }

    static Result<void> master_transmit(uint8_t index, uint16_t dev_addr, std::span<uint8_t> data, Microseconds timeout) noexcept {
        return {};
    }

    static Result<void> master_receive(uint8_t index, uint16_t dev_addr, std::span<uint8_t> data, Microseconds timeout) noexcept {
        return {};
    }
};

} // namespace hal
