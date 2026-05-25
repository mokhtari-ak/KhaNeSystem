#pragma once

#include "CalibrationManager.hpp"
#include "HalTypes.hpp"

namespace calibration {

/**
 * @brief CalibrationTask gère les séquences de calibration pré-vol.
 * 
 * Ce module n'est pas une tâche RTOS permanente, mais une séquence 
 * déclenchable lors de la transition vers l'état CALIBRATING.
 */
class CalibrationTask {
public:
    explicit CalibrationTask(CalibrationManager& manager) : manager_(manager) {}

    // Séquence complète de calibration
    hal::Result<void> run_full_calibration() noexcept {
        auto res_gyro = manager_.calibrate_gyro_bias();
        if (!res_gyro) return res_gyro;

        auto res_accel = manager_.calibrate_accel_6pos();
        if (!res_accel) return res_accel;

        return {};
    }

private:
    CalibrationManager& manager_;
};

} // namespace calibration
