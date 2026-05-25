#pragma once

#include "stm32f4xx_hal.h"
#include "HalTypes.hpp"
#include <array>

namespace hal {

struct UartInstance {
    UART_HandleTypeDef handle;
    bool initialized = false;
};

class UartDriver {
public:
    static constexpr size_t MaxInstances = 6; // USART1-6 sur F407

    template<typename Config>
    static Result<uint8_t> init() noexcept {
        static std::array<UartInstance, MaxInstances> instances{};
        return 0;
    }

    static Result<void> transmit(uint8_t index, std::span<uint8_t> data, Microseconds timeout) noexcept {
        return {};
    }

    static Result<void> receive(uint8_t index, std::span<uint8_t> data, Microseconds timeout) noexcept {
        return {};
    }
};

} // namespace hal
