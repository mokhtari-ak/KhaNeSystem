# Story 3.2: Intégration bibliothèque KalmanFilter

## Status    
- **ID:** 3.2
- **Key:** 3-2-integration-bibliotheque-kalmanfilter
- **Status:** review
- **Epic:** Epic 3: Sensor Fusion & Drivers
- **Last Updated:** 2026-06-01

## File List
- Modules/KalmanFilterLibrary/CMakeLists.txt
- Modules/KalmanFilterLibrary/test/compilation_test.cpp
- Modules/KalmanFilterLibrary/test/system_model_example.cpp
- Modules/KalmanFilterLibrary/include/kalman/KalmanFilterBase.hpp

## Dev Agent Record
- **Completion Notes:**
  - Intégration CMakeLibrary terminée en tant qu'INTERFACE library.
  - Conformité C++20, fno-exceptions, fno-rtti validée.
  - Utilisation Eigen via Eigen3::Eigen configurée.
  - Scalar Type Enforcement: `static_assert` ajouté dans `KalmanFilterBase` pour forcer `float`.
  - Integration Example: Modèle `SystemModel` créé et validé via `test/system_model_example.cpp`.
## Requirements
### User Story
En tant que développeur d'algorithmes de vol, je souhaite intégrer la bibliothèque `KalmanFilterLibrary` dans l'environnement de build du projet afin de disposer des primitives mathématiques (EKF, UKF) nécessaires à l'estimation précise de l'état du drone.

### Acceptance Criteria (BDD)
- **GIVEN** la bibliothèque source dans `Modules/KalmanFilterLibrary/`
- **WHEN** j'intègre la bibliothèque via CMake
- **THEN** elle doit être disponible en tant qu'interface library pour les autres modules.
- **AND** elle doit compiler avec les flags `-fno-exceptions` et `-fno-rtti`.
- **AND** l'utilisation de `float` (simple précision) doit être forcée partout pour exploiter la FPU du STM32F4.
- **AND** un exemple de modèle de système (SystemModel) et de modèle de mesure (MeasurementModel) compatible avec l'architecture `KhaNeSystem` doit être validé.
- **AND** aucune allocation dynamique ne doit être effectuée durant les étapes de `predict` et `update`.

## Developer Context
### Technical Guardrails
- **Eigen Integration:** La bibliothèque repose lourdement sur Eigen. S'assurer que la version d'Eigen utilisée est celle fournie dans `Modules/eigen/` pour éviter les conflits.
- **Zero dynamic allocation:** Vérifier que les matrices Eigen sont de taille fixe (`RowsAtCompileTime`) pour garantir l'allocation sur la pile ou en statique.
- **Precision:** Interdire l'utilisation de `double` au sein des templates de la bibliothèque (utiliser `float` comme paramètre de type scalaire).
- **Safety:** Bien que la bibliothèque soit template-heavy, elle doit respecter la contrainte de non-utilisation du RTTI.

### Architecture Compliance
- Utilisation de `Eigen::Vector3f`, `Eigen::Quaternionf`, etc. pour la compatibilité avec les messages du bus (`BusMessages.hpp`).
- Alignement avec la FR-03 (Fusion de capteurs EKF).

### File Structure Requirements
- `Modules/KalmanFilterLibrary/include/kalman/` : Contient les headers de la bibliothèque.
- `Modules/KalmanFilterLibrary/CMakeLists.txt` : Doit exposer la cible `KalmanFilterLib`.

## Implementation Strategy
1. **CMake Export:** Modifier le `CMakeLists.txt` de la bibliothèque pour créer une `INTERFACE` library exportant les répertoires d'include et les dépendances Eigen.
2. **Standard Validation:** Créer un petit test de compilation (`static_assert`) vérifiant que les classes `ExtendedKalmanFilter` sont instanciables avec `-fno-exceptions`.
3. **Scalar Type Enforcement:** Définir une politique ou un alias global pour s'assurer que `Kalman::T` est toujours un `float`.
4. **Integration Example:** Implémenter un modèle de système de base (ex: Constant Velocity) pour valider l'interface.

## Validation Plan
- **Compilation Check:** Vérifier que le module `SensorFusion` peut lier la bibliothèque sans erreur.
- **Binary Audit:** S'assurer que l'inclusion de la bibliothèque n'introduit pas de sections de gestion d'exceptions dans le binaire ELF final.
- **Static Analysis:** Vérifier via `clang-tidy` l'absence de `std::vector` ou `std::map` au sein des classes de filtrage instanciées.

## Project Context Reference
- `PRD` : FR-03.
- `GEMINI.md` : Section "FPU usage" (Simple précision uniquement).
- `architecture.md` : Section "EkfTask" (fréquence 250Hz).
