#pragma once

#include "IPowerMonitor.hpp"
#include "HalTypes.hpp"

namespace modules::bms {

/**
 * @brief Gestionnaire de batterie (BMS).
 * Surveille la tension/courant via IPowerMonitor et estime le SoC.
 */
class BatteryManagementSystem {
public:
    explicit BatteryManagementSystem(hal::IPowerMonitor& power_monitor) 
        : power_monitor_(power_monitor), soc_(1.0f) {}

    // Mise à jour périodique des mesures et estimation
    hal::Result<void> update() noexcept {
        auto data = power_monitor_.get_data();
        if (!data) return hal::unexpected(data.error());

        // Algorithme simplifié d'estimation SoC (State-of-Charge)
        // A remplacer par un modèle plus robuste
        if (data->voltage < 3.3f) soc_ = 0.0f;
        else if (data->voltage > 4.2f) soc_ = 1.0f;
        else soc_ = (data->voltage - 3.3f) / (4.2f - 3.3f);
        
        return {};
    }

    float get_soc() const noexcept { return soc_; }

private:
    hal::IPowerMonitor& power_monitor_;
    float soc_;
};

} // namespace modules::bms
