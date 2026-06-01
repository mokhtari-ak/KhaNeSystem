#pragma once

#include "HalTypes.hpp"
#include "FlashStorage.hpp"
#include <array>
#include <functional>

#include <cstddef>

namespace cfg {

struct alignas(4) ConfigParams {
    float pid_roll_p;
    float pid_roll_i;
    float pid_pitch_p;
    float pid_pitch_i;
    float battery_warning_v;
    uint32_t sequence; // Déplacé avant le checksum pour faciliter le calcul
    uint32_t checksum; // Doit être le dernier champ pour la logique de calcul simplifiée
};

class ConfigurationManager {
public:
    const ConfigParams& get() const noexcept { return current_params_; }

    hal::Result<void> set(const ConfigParams& new_params) noexcept {
        current_params_ = new_params;
        return {};
    }

    hal::Result<void> save() noexcept {
        current_params_.sequence++;
        // Calcul CRC sur tout ce qui précède le champ 'checksum'
        current_params_.checksum = FlashStorage::calculate_crc(&current_params_, offsetof(ConfigParams, checksum));
        
        // Basculement : si on a écrit dans A, on écrit dans B ensuite
        uint32_t next_sector = (last_sector_ == FLASH_SECTOR_A) ? FLASH_SECTOR_B : FLASH_SECTOR_A;
        auto res = FlashStorage::write_buffer(next_sector, &current_params_, sizeof(ConfigParams));
        if (res) last_sector_ = next_sector;
        return res;
    }

    // Chargement au boot (implémentation simplifiée)
    static ConfigurationManager& load() noexcept {
        static ConfigurationManager instance;
        // Ici : lire A et B, comparer les séquences et CRC
        instance.last_sector_ = FLASH_SECTOR_A;
        return instance;
    }

private:
    ConfigurationManager() = default;
    ConfigParams current_params_;
    uint32_t last_sector_ = FLASH_SECTOR_A;
};

} // namespace cfg
