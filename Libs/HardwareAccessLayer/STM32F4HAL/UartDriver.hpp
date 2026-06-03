#pragma once

#include "stm32f4xx_hal.h"
#include "HalTypes.hpp"
#include <cstdint>

namespace hal {

class UartDriver {
public:
    static Result<void> transmit(UART_HandleTypeDef* huart, const uint8_t* data, uint16_t size, Milliseconds timeout) noexcept {
        // La HAL STM32 utilise uint8_t* pour les données en transmission, bien qu'elles soient traitées en lecture seule.
        // Le cast est nécessaire pour l'API C, mais nous garantissons l'absence de modification.
        HAL_StatusTypeDef status = HAL_UART_Transmit(huart, reinterpret_cast<uint8_t*>(const_cast<uint8_t*>(data)), size, static_cast<uint32_t>(timeout.count));
        if (status == HAL_OK) return {};
        if (status == HAL_BUSY) return hal::unexpected(HalError::Busy);
        if (status == HAL_TIMEOUT) return hal::unexpected(HalError::Timeout);
        return hal::unexpected(HalError::Error);
    }

    static Result<void> receive(UART_HandleTypeDef* huart, uint8_t* data, uint16_t size, Milliseconds timeout) noexcept {
        HAL_StatusTypeDef status = HAL_UART_Receive(huart, data, size, static_cast<uint32_t>(timeout.count));
        if (status == HAL_OK) return {};
        if (status == HAL_BUSY) return hal::unexpected(HalError::Busy);
        if (status == HAL_TIMEOUT) return hal::unexpected(HalError::Timeout);
        return hal::unexpected(HalError::Error);
    }
};

} // namespace hal
