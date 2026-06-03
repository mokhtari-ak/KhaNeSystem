#pragma once
#include "IHalWrappers.hpp"
#include "HalTypes.hpp"
#include "stm32f4xx_hal.h" // Seul endroit autorisé pour cette inclusion

namespace hal {

class SpiWrapper {
public:
    explicit SpiWrapper(SPI_HandleTypeDef* hspi) : hspi_(hspi) {}

    hal::Result<void> transmit_receive(std::span<uint8_t> tx, std::span<uint8_t> rx, Microseconds timeout) noexcept {
        HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hspi_, tx.data(), rx.data(), tx.size(), timeout.count);
        if (status != HAL_OK) {
            return hal::unexpected(HalError::Error);
        }
        return {};
    }

private:
    SPI_HandleTypeDef* hspi_;
};

} // namespace hal
