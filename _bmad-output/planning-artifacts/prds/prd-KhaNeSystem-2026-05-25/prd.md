---
title: KhaNeSystem Flight Controller Firmware PRD
status: draft
created: 2026-05-25
updated: 2026-05-25
---

# 1. Vision
Firmware de vol modulaire, déterministe et hautement sécurisé pour drones (Fixed-Wing & Quadcopter) sur STM32F407VG. Conçu pour la fiabilité critique avec une architecture compile-time, sans exception ni allocation dynamique en temps-réel.

# 2. Essential Spine
- **Modularité**: Architecture par domaines (Control, Fusion, Comm, Storage, Power, Safety).
- **Déterminisme**: C++20 embarqué, zéro exception/RTTI, temps-réel garanti.
- **Sécurité**: Pattern `Result`, Watchdogs hiérarchiques, FAILSAFE explicite.
- **Support Hardware**: STM32F407VG (Cortex-M4F, 192KB RAM).

# 3. Functional Requirements
- **FR-01**: Support double typologie de drone (Fixed-Wing / Quadcopter) via compile-time template selection.
- **FR-02**: Machine à états sécurisée (DISARMED/ARMED/FAILSAFE).
- **FR-03**: Fusion de capteurs (EKF) pour estimation état véhicule.
- **FR-04**: Communication MAVLink v2 (Downlink/Uplink) via UART DMA.
- **FR-05**: Logging non-bloquant sur SD Card (secteurs bruts).
- **FR-06**: Navigation Waypoints avec Geofencing.
- **FR-07**: Gestion FAILSAFE multi-niveaux :
    - WARNING: Alerte télémétrie, log.
    - CRITICAL: RTH ou procédure de secours configurée.
    - EMERGENCY: Glide (FW) / Descente contrôlée (Quad) + coupure throttle.
    - LOCKOUT: Coupure totale + Reset MCU si non-responsive.
- **FR-08**: Internal Event Bus typé :
    - Transport de messages versionés (SensorFrame, GnssFrame, StateVector, ActuatorCmd, RcFrame, NavGuidance, FaultEvent, PowerStatus).
    - Taille maximale par message : 128 octets, zéro allocation dynamique.
- **FR-09**: Séquence de démarrage déterministe (Boot Sequence) :
    - Initialisation ordonnée : BSP -> SystemClock -> HAL Wrappers -> Logging -> Configuration -> Drivers -> Fusion -> Autopilot.
    - Validation CRC des coefficients de calibration avant armement.
- **FR-10**: Validation des données capteurs :
    - Validation stricte des données avant publication (Saturation, NaN, incohérence, timestamp jitter).
    - Flag de validité systématique dans les frames de capteurs.
- **FR-11**: Traçabilité complète (Logging) :
    - Journalisation systématique des transitions d'états (Machine à États) et des FaultEvents.
    - Chaque entrée inclut horodatage, ID module, ID message et payload binaire.

# 4. Non-Functional Requirements
- **NFR-01**: Pas d'allocation dynamique dans les boucles de contrôle RT.
- **NFR-02**: Temps de réponse SafetyTask < 10 ms.
- **NFR-03**: Support agnostique RTOS (FreeRTOS / ThreadX) via abstraction.
- **NFR-04**: Utilisation exclusive du FPU simple précision.


# 5. Concerns
- Mémoire: Gestion stricte des 192 KB RAM, buffers DMA hors CCM.
- Sécurité: Validation exhaustive des capteurs (IMU, Baro, GNSS, etc.).
- Performance: Respect des budgets CPU (~62% total estimé).
