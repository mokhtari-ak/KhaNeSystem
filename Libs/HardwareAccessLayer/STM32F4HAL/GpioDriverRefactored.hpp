#pragma once

#include "stm32f4xx_hal.h"
#include "HalTypes.hpp"

namespace hal {

enum class GpioMode {
    Input,
    OutputPushPull,
    OutputOpenDrain,
    AlternateFunction,
    Analog
};

class GpioDriver {
public:
    static Result<void> set_mode(GPIO_TypeDef* port, uint16_t pin, GpioMode mode) noexcept {
        // Logique de configuration GPIO simple, non allouante
        return {};
    }

    static Result<void> write(GPIO_TypeDef* port, uint16_t pin, bool state) noexcept {
        HAL_GPIO_WritePin(port, pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
        return {};
    }

    static Result<bool> read(GPIO_TypeDef* port, uint16_t pin) noexcept {
        return HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET;
    }
};

} // namespace hal
