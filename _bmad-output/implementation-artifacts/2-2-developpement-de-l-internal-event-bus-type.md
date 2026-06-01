# Story 2.2: Développement de l'Internal Event Bus typé

## Status
- **ID:** 2.2
- **Key:** 2-2-developpement-de-l-internal-event-bus-type
- **Status:** review
- **Epic:** Epic 2: RTOS & Communication
- **Last Updated:** 2026-05-31

## Tasks/Subtasks
- [x] Refactoriser l'interface `ISubscriber` et l'adaptateur
- [x] Implémenter le locking après init
- [x] Assurer le zéro allocation dynamique via statics
- [x] Compilation et validation

## Dev Agent Record
- **Completion Notes:**
    - Bus d'événements interne refactorisé avec des adaptateurs statiques.
    - Élimination des allocations dynamiques et des `std::function`.
    - Mécanisme de verrouillage après initialisation implémenté.
- **Change Log:**
    - Modifié `Modules/InternalEventBus/EventBus.hpp` pour utiliser des `QueueAdapter` statiques.
