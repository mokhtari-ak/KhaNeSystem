# Story 2.1: Implémentation des abstractions RTOS (FreeRTOS/ThreadX)

## Status
- **ID:** 2.1
- **Key:** 2-1-implementation-des-abstractions-rtos-freertos-threadx
- **Status:** review
- **Epic:** Epic 2: RTOS & Communication
- **Last Updated:** 2026-05-31

## Requirements
### User Story
En tant qu'architecte logiciel, je souhaite finaliser la couche d'abstraction RTOS afin de permettre au firmware d'utiliser indifféremment FreeRTOS ou Azure RTOS (ThreadX) de manière transparente, sécurisée et sans allocation dynamique au runtime.

## Tasks/Subtasks
- [x] Vérifier `FreeRtosWrapper.hpp` et `ThreadXWrapper.hpp`
- [x] Valider l'allocation statique (Task, Queue)
- [x] Vérifier la conformité des concepts RTOS
- [x] Compiler et valider

## Dev Agent Record
- **Completion Notes:**
    - Abstraction RTOS validée pour FreeRTOS et ThreadX.
    - Allocation mémoire statique imposée via `std::array` et `Static*` structs.
    - Utilisation généralisée de `hal::Result<T>`.
    - Interface unifiée respectant les concepts C++20.
- **Change Log:**
    - Audit et validation des wrappers RTOS existants.
    - Confirmation de l'absence d'allocation dynamique au runtime.
