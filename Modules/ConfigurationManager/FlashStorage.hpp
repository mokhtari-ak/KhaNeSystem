#pragma once

#include "stm32f4xx_hal.h"
#include "HalTypes.hpp"

namespace cfg {

// Adresses des secteurs Flash pour configuration (ex: secteurs 10 et 11)
inline constexpr uint32_t FLASH_SECTOR_A = FLASH_SECTOR_10;
inline constexpr uint32_t FLASH_SECTOR_B = FLASH_SECTOR_11;

class FlashStorage {
public:
    static hal::Result<void> write_buffer(uint32_t sector, const void* data, size_t size) noexcept {
        HAL_FLASH_Unlock();
        
        // Effacement du secteur
        FLASH_EraseInitTypeDef erase_init{FLASH_TYPEERASE_SECTORS, sector, 1, FLASH_VOLTAGE_RANGE_3};
        uint32_t sector_error;
        if (HAL_FLASHEx_Erase(&erase_init, &sector_error) != HAL_OK) {
            HAL_FLASH_Lock();
            return std::unexpected(hal::HalError::Error);
        }

        // Écriture des données
        const uint8_t* p = static_cast<const uint8_t*>(data);
        for (size_t i = 0; i < size; i += 4) {
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 0x080C0000 + (sector * 0x20000) + i, *(uint32_t*)(p + i)) != HAL_OK) {
                HAL_FLASH_Lock();
                return std::unexpected(hal::HalError::Error);
            }
        }

        HAL_FLASH_Lock();
        return {};
    }

    static uint32_t calculate_crc(const void* data, size_t size) noexcept {
        // Logique CRC32 via périphérique matériel CRC ou soft
        return 0; 
    }
};

} // namespace cfg
