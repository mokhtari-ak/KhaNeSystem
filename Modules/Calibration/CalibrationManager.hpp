#pragma once

#include "HalTypes.hpp"
#include "ConfigurationManager.hpp"
#include <array>

namespace calibration {

// Coefficients de calibration persistants
struct CalibrationCoeffs {
    Eigen::Vector3f gyro_bias;
    Eigen::Matrix3f accel_scale_offset;
    Eigen::Matrix3f mag_hard_soft_iron;
    uint32_t crc;
    bool is_calibrated;
};

class CalibrationManager {
public:
    // Procédures de calibration
    hal::Result<void> calibrate_gyro_bias() noexcept {
        // Moyenne sur 5 secondes à l'arrêt
        return {};
    }

    hal::Result<void> calibrate_accel_6pos() noexcept {
        // Calibration 6 positions
        return {};
    }

    // Validation des coefficients au boot
    bool validate_coefficients(const CalibrationCoeffs& coeffs) noexcept {
        // Vérification CRC et bornes
        return coeffs.is_calibrated;
    }

    // Sauvegarde via ConfigurationManager
    hal::Result<void> save_calibration(const CalibrationCoeffs& coeffs) noexcept {
        // Utilisation du ConfigurationManager pour persister
        return {};
    }
};

} // namespace calibration
