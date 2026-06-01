# Story 2.4: Développement des tâches I/O (RcInputTask, TelemetryTask)

## Status
- **ID:** 2.4
- **Key:** 2-4-developpement-des-taches-io-rcinputtask-telemetrytask
- **Status:** review
- **Epic:** Epic 2: RTOS & Communication
- **Last Updated:** 2026-05-31

## Tasks/Subtasks
- [x] Refactoriser `RcInputTask` avec les nouveaux drivers
- [x] Implémenter la publication sur bus (`RcFrame`)
- [x] Implémenter `TelemetryTask` avec abonnement aux messages
- [x] Compilation et validation des timings

## Dev Agent Record
- **Completion Notes:**
    - `RcInputTask` refactorisée avec les nouveaux wrappers.
    - `TelemetryTask` implémentée et abonnée au bus.
    - Fréquences RTOS respectées (5ms/100ms).
- **Change Log:**
    - Mise à jour des modules `FlightControl` et `RFCommunication`.
