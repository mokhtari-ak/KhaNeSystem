# Story 5.1: SafetyTask (FaultBus, Watchdog, Failsafe)

## Status
- **ID:** 5.1
- **Key:** 5-1-safetytask-faultbus-watchdog-failsafe
- **Status:** review
- **Epic:** Epic 5: Sécurité & Supervision
- **Last Updated:** 2026-06-01

## File List
- Modules/Safety/SafetyTask.hpp

## Dev Agent Record
- **Completion Notes:**
  - Implémentation de la boucle RTOS à 400Hz via `vTaskDelayUntil`.
  - Machine à états Failsafe de base implémentée (Niveaux None/Critical).
  - Watchdog matériel (IWDG) conditionnel : rafraîchi uniquement si le niveau est < Critical.


## Requirements
### User Story
En tant que responsable de la sécurité du vol, je souhaite implémenter une tâche de surveillance haute priorité (400Hz) capable d'agréger les défauts système et de déclencher des procédures de sauvegarde (Failsafe) afin de protéger l'intégrité du drone et de son environnement en cas d'anomalie critique.

### Acceptance Criteria (BDD)
- **GIVEN** le module `Safety` et l'Internal Event Bus
- **WHEN** j'implémente la `SafetyTask`
- **THEN** elle doit s'exécuter à 400Hz avec la priorité la plus haute (7/7).
- **AND** elle doit s'abonner au `FaultBus` pour recevoir les événements de panne des autres modules.
- **AND** elle doit implémenter la machine à états FAILSAFE (Warning, Critical, Emergency, Lockout - FR-07).
- **AND** elle doit rafraîchir le Watchdog matériel (IWDG) uniquement si toutes les tâches critiques sont "healthy".
- **AND** le temps de réponse entre la détection d'une faute critique et l'action de sauvegarde doit être < 10ms.
- **AND** aucune allocation dynamique ne doit être effectuée.

## Developer Context
### Technical Guardrails
- **Watchdog Logic:** Ne pas se contenter d'un simple "kick" périodique. Utiliser un mécanisme de "checkpointing" où chaque tâche critique (Sensor, FlightControl, Ekf) doit signaler sa santé avant que la `SafetyTask` n'autorise le kick de l'IWDG.
- **Failsafe Response:**
    - **CRITICAL:** Déclencher une procédure RTH (Return To Home).
    - **EMERGENCY:** Coupure immédiate des gaz (Kill Switch) ou descente contrôlée.
    - **LOCKOUT:** Reset forcé du MCU via IWDG.
- **FaultBus Aggregation:** Utiliser un masque de bits (`std::bitset<32>`) pour stocker l'état courant de tous les défauts actifs.
- **Determinism:** La boucle de sécurité doit être la plus courte possible pour garantir le respect de la deadline de 2.5ms (à 400Hz).

### Architecture Compliance
- Respect de la FR-07 (FAILSAFE multi-niveaux) et NFR-02 (Temps de réponse < 10ms).
- Utilisation de `bus::FaultEvent` pour la communication inter-modules.

### File Structure Requirements
- `Modules/Safety/SafetyTask.hpp` : Logique de surveillance et machine à états.

## Implementation Strategy
1. **Machine à États Failsafe:** Implémenter la logique de transition entre les niveaux de sévérité.
2. **Gestion du Watchdog:** Configurer l'IWDG (STM32) via la HAL ST et implémenter le système de santé des tâches (Status flags).
3. **Traitement du FaultBus:** Implémenter le callback d'abonnement au bus qui met à jour le masque de fautes interne.
4. **Actionneur Failsafe:** Implémenter une méthode `apply_failsafe_actions()` qui peut forcer les valeurs du `ActuatorCmd` ou couper l'alimentation des moteurs.

## Validation Plan
- **Failsafe Transition Test:** Injecter une faute de type "Sensor Loss" et vérifier que le niveau passe de `None` à `Critical`.
- **Watchdog Timeout Test:** Simuler le blocage d'une tâche (ex: boucle infinie) et vérifier que l'IWDG n'est plus rafraîchi, provoquant le reset du MCU.
- **Response Time Audit:** Utiliser un timer matériel pour mesurer le délai entre l'injection d'un défaut sur le bus et le changement d'état des sorties moteurs.

## Project Context Reference
- `PRD` : FR-07, NFR-02.
- `architecture.md` : Section "SafetyTask" (Prio 7/7, 400Hz).
- `BusMessages.hpp` : Structure `FaultEvent`.
