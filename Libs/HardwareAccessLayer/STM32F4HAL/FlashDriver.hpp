#pragma once

#include "stm32f4xx_hal.h"
#include "IFlashDriver.hpp"

namespace hal {

class FlashDriver : public IFlashDriver {
public:
    Result<void> erase_sector(uint32_t sector) noexcept override {
        FLASH_EraseInitTypeDef erase_init{FLASH_TYPEERASE_SECTORS, sector, 1, FLASH_VOLTAGE_RANGE_3};
        uint32_t sector_error;
        if (HAL_FLASHEx_Erase(&erase_init, &sector_error) != HAL_OK) {
            return std::unexpected(HalError::Error);
        }
        return {};
    }

    Result<void> write_word(uint32_t address, uint32_t data) noexcept override {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data) != HAL_OK) {
            return std::unexpected(HalError::Error);
        }
        return {};
    }

    Result<void> unlock() noexcept override {
        if (HAL_FLASH_Unlock() != HAL_OK) {
            return std::unexpected(HalError::Error);
        }
        return {};
    }

    Result<void> lock() noexcept override {
        if (HAL_FLASH_Lock() != HAL_OK) {
            return std::unexpected(HalError::Error);
        }
        return {};
    }
};

class CrcDriver : public ICrcDriver {
public:
    uint32_t calculate(const void* data, size_t size) noexcept override {
        __HAL_RCC_CRC_CLK_ENABLE();
        CRC_HandleTypeDef hcrc;
        hcrc.Instance = CRC;
        
        if (HAL_CRC_Init(&hcrc) != HAL_OK) {
            __HAL_RCC_CRC_CLK_DISABLE();
            return 0;
        }
        
        uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)data, size / 4);
        
        __HAL_RCC_CRC_CLK_DISABLE();
        return crc;
    }
};

} // namespace hal
