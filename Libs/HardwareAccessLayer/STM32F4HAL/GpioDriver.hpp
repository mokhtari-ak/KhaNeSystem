#pragma once

#include "stm32f4xx_hal.h"
#include "HalTypes.hpp"
#include <cstdint>

namespace hal {

class GpioDriver {
public:
    static Result<void> write(GPIO_TypeDef* port, uint16_t pinMask, bool state) noexcept {
        HAL_GPIO_WritePin(port, pinMask, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
        return {};
    }

    static Result<bool> read(GPIO_TypeDef* port, uint16_t pinMask) noexcept {
        return HAL_GPIO_ReadPin(port, pinMask) == GPIO_PIN_SET;
    }

    static Result<void> toggle(GPIO_TypeDef* port, uint16_t pinMask) noexcept {
        HAL_GPIO_TogglePin(port, pinMask);
        return {};
    }
};

} // namespace hal
