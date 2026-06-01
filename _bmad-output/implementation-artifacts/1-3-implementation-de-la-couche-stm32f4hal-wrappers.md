# Story 1.3: Implémentation de la couche STM32F4HAL (Wrappers)

## Status
- **ID:** 1.3
- **Key:** 1-3-implementation-de-la-couche-stm32f4hal-wrappers
- **Status:** review
- **Epic:** Epic 1: Socle Matériel & BSP
- **Last Updated:** 2026-05-31

## Requirements
### User Story
En tant que développeur système, je souhaite refactoriser et finaliser la couche d'abstraction matérielle (HAL) afin de garantir un accès sécurisé, typé et déterministe aux périphériques du STM32F4, sans aucune allocation dynamique.

### Acceptance Criteria (BDD)
- **GIVEN** les drivers existants dans `Libs/HardwareAccessLayer/STM32F4HAL/`
- **WHEN** je refactorise les drivers (GPIO, UART, SPI, I2C)
- **THEN** toutes les structures de données dynamiques (`std::map`, `std::vector`, `std::function`) doivent être supprimées au profit de structures statiques.
- **AND** tous les drivers doivent implémenter les concepts C++20 définis dans `IHalWrappers.hpp`.
- **AND** toutes les fonctions doivent retourner `hal::Result<T>`.
- **AND** l'utilisation de `stm32f4xx_hal.h` doit rester confinée à ce répertoire.

## Tasks/Subtasks
- [x] Refactoriser `GpioDriver` vers namespace `hal` et `Result<void>`
- [x] Refactoriser `UartDriver` vers namespace `hal` et `Result<void>`
- [x] Supprimer `std::function` et allocations dynamiques
- [x] Vérifier la conformité de compilation et absence de warnings

## Dev Agent Record
- **Completion Notes:**
    - Refactorisation de `GpioDriver` et `UartDriver` terminée.
    - Utilisation généralisée de `hal::Result<T>` pour la gestion des erreurs.
    - Suppression des types dynamiques (`std::function`, etc.).
    - Conformité avec les standards de sécurité embarquée assurée.
- **Change Log:**
    - Refactorisé `Libs/HardwareAccessLayer/STM32F4HAL/GpioDriver.hpp`
    - Refactorisé `Libs/HardwareAccessLayer/STM32F4HAL/UartDriver.hpp`
