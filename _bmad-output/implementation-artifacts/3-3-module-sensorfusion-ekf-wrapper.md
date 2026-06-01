# Story 3.3: Module SensorFusion (EKF Wrapper)

## Status
- **ID:** 3.3
- **Key:** 3-3-module-sensorfusion-ekf-wrapper
- **Status:** in-progress
- **Epic:** Epic 3: Sensor Fusion & Drivers
- **Last Updated:** 2026-06-01

## File List
- Modules/SensorFusion/SensorFusion.hpp
- Modules/SensorFusion/test/system_model_test.cpp
- Modules/SensorFusion/test/CMakeLists.txt

## Senior Developer Review (AI)
- **Review Outcome:** Changes Requested
- **Review Date:** 2026-06-01
- **Findings Summary:** 
  - 2 CRITIQUE (Allocation dynamique, conformité FPU)
  - 2 HAUTE (Gestion erreurs, Bus Safety)
  - 2 MOYENNE/BASSE (Refactorisation Uart, Documentation)

## Review Follow-ups (AI)
- [x] C-01: Vérifier/supprimer allocations dynamiques dans EKF (Eigen)
- [x] C-02: Supprimer tout `double` (vérifier avec -Wdouble-promotion)
- [x] H-01: Wrapper HAL_IWDG_Refresh
- [x] H-02: Vérifier instanciation statique EventBus
- [x] M-01: Supprimer `const_cast` dans UartDriver
- [x] B-01: Compléter les équations dans DroneSystemModel

## Requirements
### User Story
En tant que développeur d'algorithmes de vol, je souhaite implémenter un wrapper EKF robuste au sein du module `SensorFusion` afin de transformer les mesures brutes des capteurs en une estimation précise et stable de l'état du drone (attitude, position, vitesse).

### Acceptance Criteria (BDD)
- **GIVEN** le module `SensorFusion` et la bibliothèque `KalmanFilterLibrary`
- **WHEN** j'implémente le wrapper EKF
- **THEN** il doit s'abonner aux messages `SensorFrame` et `GnssFrame` via le bus interne.
- **AND** il doit exécuter le cycle `predict` / `update` de l'EKF à une fréquence de 250Hz.
- **AND** le vecteur d'état estimé doit être publié sur le bus sous forme de `StateVector`.
- **AND** un mécanisme de surveillance des innovations doit détecter les divergences capteurs (innovation gating).
- **AND** la méthode `is_healthy()` doit retourner `false` si l'EKF diverge ou si les capteurs critiques sont perdus.
- **AND** aucune allocation dynamique ne doit être effectuée dans la boucle de calcul (`run()`).

## Developer Context
### Technical Guardrails
- **State Vector Mapping:** Aligner le vecteur d'état de l'EKF (ex: 15 états : position, vitesse, attitude, biais gyro, biais accel) avec les champs du `bus::StateVector`.
- **Initialization:** Implémenter une phase d'alignement statique (ex: 2 secondes au boot) pour estimer les biais initiaux et l'orientation par rapport à la gravité.
- **Innovation Gating:** Rejeter les mesures GNSS ou Baro dont l'innovation dépasse un seuil de confiance (test du Chi-deux).
- **Eigen Usage:** Utiliser `Eigen::Quaternionf` pour représenter l'attitude afin d'éviter les singularités des angles d'Euler.

### Architecture Compliance
- Respect de la FR-03 (Fusion de capteurs EKF).
- Implémentation du concept `IStateEstimator` défini dans `SensorFusion.hpp`.
- Utilisation de la `EkfTask` pour le séquencement RTOS.

### File Structure Requirements
- `Modules/SensorFusion/SensorFusion.hpp` : Logique de l'algorithme.
- `Modules/SensorFusion/EkfTask.hpp` : Gestion de la tâche RTOS.
- `Modules/SensorFusion/SystemModel.hpp` : (Nouveau) Définition des équations de mouvement pour l'EKF.
- `Modules/SensorFusion/MeasurementModels.hpp` : (Nouveau) Modèles d'observation (GPS, Baro, etc.).

## Implementation Strategy
1. **Définition des Modèles:** Créer les classes `DroneSystemModel` et les modèles de mesure (`ImuMeasurementModel`, `GnssMeasurementModel`) en utilisant les templates de la `KalmanFilterLibrary`.
2. **Instanciation EKF:** Instancier `Kalman::ExtendedKalmanFilter` avec le type d'état approprié.
3. **Logique de Predict:** Dans `update_attitude`, calculer le delta temps `dt` et appeler `predict()`.
4. **Logique d'Update:** Appeler `update()` dès qu'une nouvelle mesure GNSS ou Baro est disponible.
5. **Gestion des Biais:** Intégrer les biais estimés par l'EKF dans la correction des mesures IMU avant le prochain cycle de prédiction.

## Validation Plan
- **Unit Testing (Simulation):** Injecter des trajectoires simulées (ex: cercle parfait) avec bruit gaussien et vérifier que l'EKF converge vers la vérité terrain.
- **Innovation Monitor Test:** Injecter une mesure GPS aberrante et vérifier qu'elle est rejetée par le gating.
- **Timing Check:** S'assurer que le calcul complet (`predict` + `update`) s'exécute en moins de 1ms sur STM32F4.

## Project Context Reference
- `PRD` : FR-03, FR-10.
- `architecture.md` : Section "EkfTask" et "RTOS Task Mapping".
- `BusMessages.hpp` : Structures `SensorFrame`, `GnssFrame` et `StateVector`.
