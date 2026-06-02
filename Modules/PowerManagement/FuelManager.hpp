#pragma once

#include "IPowerMonitor.hpp"
#include "HalTypes.hpp"

namespace modules::fuel {

/**
 * @brief Gestionnaire de carburant (Fuel Manager).
 * Estime la consommation et l'autonomie restante.
 */
class FuelManager {
public:
    explicit FuelManager(hal::IPowerMonitor& power_monitor) 
        : power_monitor_(power_monitor), fuel_remaining_(100.0f) {}

    // Mise à jour de l'estimation de fuel
    hal::Result<void> update(float dt) noexcept {
        auto data = power_monitor_.get_data();
        if (!data) return std::unexpected(data.error());

        // Calcul simplifié de consommation
        float consumption = data->current * dt; // Ah
        fuel_remaining_ -= consumption;
        
        return {};
    }

    float get_fuel_remaining() const noexcept { return fuel_remaining_; }

private:
    hal::IPowerMonitor& power_monitor_;
    float fuel_remaining_;
};

} // namespace modules::fuel
