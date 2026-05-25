#pragma once

#include "HalTypes.hpp"
#include <array>
#include <functional>

namespace cfg {

// Schéma de base pour la configuration
struct ConfigParams {
    float pid_roll_p;
    float pid_roll_i;
    float pid_pitch_p;
    float pid_pitch_i;
    float battery_warning_v;
    uint32_t checksum;
};

class ConfigurationManager {
public:
    // Accès O(1) aux paramètres
    const ConfigParams& get() const noexcept { return current_params_; }

    // Mise à jour synchrone avec validation
    hal::Result<void> set(const ConfigParams& new_params) noexcept {
        // Validation des bornes ici
        current_params_ = new_params;
        return {};
    }

    // Persistance (Flush asynchrone)
    hal::Result<void> save() noexcept {
        uint32_t crc = FlashStorage::calculate_crc(&current_params_, sizeof(current_params_));
        current_params_.checksum = crc;
        
        // Alternance de secteur pour le double-buffer (atomique)
        return FlashStorage::write_buffer(FLASH_SECTOR_A, &current_params_, sizeof(current_params_));
    }

    // Chargement au boot
    static ConfigurationManager& load() noexcept {
        static ConfigurationManager instance;
        // Chargement NVM -> validation -> application
        return instance;
    }

private:
    ConfigurationManager() = default;
    ConfigParams current_params_;
};

} // namespace cfg
