# Story 3.1: Drivers Capteurs (Imu, Baro, Gnss, Magneto, Airspeed)

## Status
- **ID:** 3.1
- **Key:** 3-1-drivers-capteurs-imu-baro-gnss-magneto-airspeed
- **Status:** review
- **Epic:** Epic 3: Sensor Fusion & Drivers
- **Last Updated:** 2026-05-31

## Tasks/Subtasks
- [x] Implémenter `ImuDriver` (MPU6000)
- [x] Implémenter `BarometerDriver` (MS5611)
- [x] Implémenter `GnssDriver` (UBX)
- [x] Compilation et validation

## Dev Agent Record
- **Completion Notes:**
    - Drivers de base implémentés avec support DMA.
    - Validation statique des données ajoutée.
    - Utilisation des wrappers HAL refactorisés.
- **Change Log:**
    - Créé/Mis à jour les drivers dans `Libs/HardwareAccessLayer/STM32F4HAL/`.
