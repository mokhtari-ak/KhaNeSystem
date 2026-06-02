# Story: 5-4-module-calibration-tuning-persistance-et-validation-crc

## Description
Refactor Calibration and Configuration modules for robust persistence and integrity verification.

## Developer Context
The current implementation of flash storage and calibration persistence has several issues:
1.  **HAL Violation:** `FlashStorage.hpp` directly includes `stm32f4xx_hal.h` and uses raw HAL functions, violating project guidelines to only access HAL via `Libs/HardwareAccessLayer/STM32F4HAL/`.
2.  **Weak Persistence:** `ConfigurationManager` uses a simplistic A/B sector toggle without verifying integrity or sequence numbers on boot to determine which sector holds the valid, latest configuration.
3.  **Missing CRC:** While a `calculate_crc` function exists, it is not used for validating the configuration data *upon loading* to detect corruption.
4.  **Calibration:** `CalibrationManager` is currently a skeleton and lacks integrated persistence.

## Implementation Strategy
1.  **Refactor FlashStorage:**
    -   Move HAL dependencies out of `FlashStorage.hpp` to `Libs/HardwareAccessLayer/STM32F4HAL/`.
    -   Expose necessary HAL abstractions to `FlashStorage`.
2.  **Robust Persistence:**
    -   Implement A/B sector validation:
        -   On boot, read both Sector A and Sector B.
        -   Calculate CRC for both and compare against stored checksums.
        -   Compare sequence numbers.
        -   Load configuration from the sector with the highest valid sequence number and matching CRC.
3.  **Calibration Integration:**
    -   Implement `save_calibration` and load/verify `CalibrationCoeffs` using the improved persistence mechanism.
4.  **Standards Compliance:**
    -   Strictly use `Result<T, E>` for all return types.
    -   Ensure all calculations use `float`.
    -   Adhere to C++20.

## Acceptance Criteria
- [x] `FlashStorage` does not include `stm32f4xx_hal.h`.
- [x] Configuration persistence validates CRC and sequence numbers on boot.
- [x] Calibration coefficients are correctly saved and loaded.
- [x] All functions return `Result<T, E>`.
- [x] No `double` usage; all `float`.

## Status
- **Status:** review
