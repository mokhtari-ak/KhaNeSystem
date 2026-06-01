# Story 4.1: Implémentation des contrôleurs PID déterministes

## Status
- **ID:** 4.1
- **Key:** 4-1-implementation-des-controleurs-pid-deterministes
- **Status:** review
- **Epic:** Epic 4: Flight Control & Navigation
- **Last Updated:** 2026-06-01

## File List
- Modules/FlightControl/PidController.hpp
- Modules/FlightControl/test/PidController_test.cpp
- Modules/FlightControl/test/CMakeLists.txt

## Dev Agent Record
- **Completion Notes:**
  - Implémentation de `PidController` avec P, I (Anti-Windup), D (filtré passe-bas, calculé sur la mesure).
  - Respect des contraintes : simple précision `float`, zéro allocation dynamique.
  - Tests unitaires ajoutés et configurés dans CMake.

## Requirements
### User Story
En tant que développeur de lois de commande, je souhaite implémenter une classe `PidController` générique, typée et déterministe afin de garantir une stabilisation précise du drone sur tous ses axes (Roll, Pitch, Yaw, Altitude) sans gigue temporelle ni dépassement d'intégrale.

### Acceptance Criteria (BDD)
- **GIVEN** la classe template `PidController` dans `Modules/FlightControl/`
- **WHEN** j'implémente la logique de calcul
- **THEN** le calcul doit inclure les termes Proportionnel, Intégral et Dérivé.
- **AND** un mécanisme d'Anti-Windup (clamping de l'intégrale) doit être présent.
- **AND** le terme Dérivé doit être filtré par un passe-bas (Low-pass filter) pour éviter l'amplification du bruit capteur.
- **AND** la classe doit supporter le polymorphisme statique pour s'adapter aux spécificités (Fixed-Wing vs Quadcopter).
- **AND** toutes les opérations doivent être effectuées en `float` (simple précision).
- **AND** aucune allocation dynamique (tas) ne doit être utilisée.

## Developer Context
### Technical Guardrails
- **Determinism:** Le temps d'exécution doit être constant quelle que soit l'erreur d'entrée.
- **Anti-Windup:** Utiliser une politique de "clamping" sur le terme intégral (`integrator_ = std::clamp(integrator_, -gains_.i_limit, gains_.i_limit)`).
- **Derivative Kick:** Implémenter le calcul du terme D sur la mesure (`-d_measured/dt`) plutôt que sur l'erreur pour éviter les sauts brusques lors d'un changement de consigne.
- **Time Delta:** Utiliser `hal::Microseconds` pour le calcul du `dt` afin de garantir la précision temporelle dans les calculs intégrale/dérivée.

### Architecture Compliance
- Utilisation de `Axis` enum pour identifier les instances.
- Alignement avec les exigences de performance de la boucle 400Hz.

### File Structure Requirements
- `Modules/FlightControl/PidController.hpp` : Contient la classe `PidController` et ses structures de gains.

## Implementation Strategy
1. **Implémentation du Calcul:** Écrire la méthode `update()` en suivant l'algorithme PID standard discret.
2. **Filtrage D:** Ajouter une constante de temps `tau` pour le filtre passe-bas du terme dérivé.
3. **Optimisation:** S'assurer que le compilateur peut inliner les appels de `update()` pour minimiser l'overhead de la boucle 400Hz.
4. **Saturation:** Ajouter une limite de sortie (`output_limit`) globale pour le contrôleur.

## Validation Plan
- **Unit Testing (Host):**
    - Vérifier qu'un PID avec P=1, I=0, D=0 retourne exactement l'erreur.
    - Vérifier que l'intégrale sature bien à `i_limit`.
    - Simuler une rampe d'entrée et vérifier le comportement du terme D filtré.
- **Timing Check:** S'assurer que le calcul d'une instance PID prend moins de 10µs sur le STM32F4.

## Project Context Reference
- `PRD` : FR-01 (Fixed-Wing / Quadcopter support).
- `architecture.md` : Section "FlightControlTask" (400Hz).
- `HalTypes.hpp` : Pour les types de temps.
