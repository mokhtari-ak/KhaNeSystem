#pragma once

#include "BatteryManagementSystem.hpp"
#include "FuelManager.hpp"
#include "HalTypes.hpp"

namespace modules::safety {

/**
 * @brief Politique de sécurité énergétique (Failsafe).
 * Surveille BMS et FuelManager pour déclencher des procédures de sécurité.
 */
class EnergyFailsafePolicy {
public:
    EnergyFailsafePolicy(modules::bms::BatteryManagementSystem& bms, 
                         modules::fuel::FuelManager& fuel_manager)
        : bms_(bms), fuel_manager_(fuel_manager) {}

    enum class FailsafeAction {
        None,
        LandImmediately,
        ReturnToHome
    };

    hal::Result<FailsafeAction> check_safety() noexcept {
        // Logique Failsafe
        if (bms_.get_soc() < 0.1f) return FailsafeAction::LandImmediately;
        if (fuel_manager_.get_fuel_remaining() < 5.0f) return FailsafeAction::ReturnToHome;
        
        return FailsafeAction::None;
    }

private:
    modules::bms::BatteryManagementSystem& bms_;
    modules::fuel::FuelManager& fuel_manager_;
};

} // namespace modules::safety
