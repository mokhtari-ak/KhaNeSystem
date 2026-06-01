#pragma once

#include "StorageAbstraction.hpp"
#include "stm32f4xx_hal.h"
#include <array>
#include <atomic>

namespace modules::logger {

// Driver SD basé sur HAL
class SdDriver : public IStorage {
public:
    SdDriver(SPI_HandleTypeDef* hspi) : hspi_(hspi) {
        // Initialisation des buffers ping-pong
        buffer_index_ = 0;
        is_writing_ = false;
    }

    hal::Result<void> write(const uint8_t* data, size_t size) noexcept override {
        // Copier dans le buffer actif
        // Si buffer plein -> déclencher DMA
        return {};
    }

    hal::Result<void> sync() noexcept override {
        // Attendre la fin du DMA
        return {};
    }

    bool is_ready() const noexcept override {
        return !is_writing_;
    }

private:
    SPI_HandleTypeDef* hspi_;
    std::array<uint8_t, 512> buffer_a_;
    std::array<uint8_t, 512> buffer_b_;
    size_t buffer_index_;
    std::atomic<bool> is_writing_;
};

} // namespace modules::logger
