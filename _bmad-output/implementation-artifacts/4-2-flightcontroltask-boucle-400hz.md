# Story 4.2: FlightControlTask (Boucle 400Hz)

## Status
- **ID:** 4.2
- **Key:** 4-2-flightcontroltask-boucle-400hz
- **Status:** review
- **Epic:** Epic 4: Flight Control & Navigation
- **Last Updated:** 2026-06-01

## File List
- Modules/FlightControl/FlightControlTask.hpp

## Dev Agent Record
- **Completion Notes:**
  - Implémentation de la boucle RTOS à 400Hz (2.5ms) via `vTaskDelayUntil`.
  - Intégration des contrôleurs PID (Roll, Pitch, Yaw).
  - Mixage moteur (QuadX simplifié) et publication `ActuatorCmd`.
  - Logique de Failsafe (timeout RC 100ms) ajoutée.


## Requirements
### User Story
En tant que développeur système, je souhaite implémenter la tâche de contrôle de vol principale (400Hz) afin d'orchestrer la stabilisation en temps-réel du drone en combinant les consignes de pilotage et l'estimation d'état.

### Acceptance Criteria (BDD)
- **GIVEN** le module `FlightControl` et l'Internal Event Bus
- **WHEN** j'implémente la tâche `FlightControlTask`
- **THEN** elle doit s'exécuter périodiquement à 400Hz (période de 2.5ms).
- **AND** elle doit consommer les derniers messages `StateVector` et `RcFrame` de manière non-bloquante.
- **AND** elle doit mettre à jour les contrôleurs PID pour les axes Roll, Pitch et Yaw.
- **AND** elle doit publier une structure `ActuatorCmd` sur le bus après chaque calcul.
- **AND** la latence entre la réception des données et la publication de la commande doit être minimale (< 500µs).
- **AND** aucune allocation dynamique ne doit être effectuée dans la boucle de contrôle.

## Developer Context
### Technical Guardrails
- **Timing Management:** Utiliser les fonctions de délai absolu du RTOS (ex: `vTaskDelayUntil`) pour garantir une période de 2.5ms sans dérive.
- **Bus Integration:** Utiliser des files (Queues) dimensionnées pour 1 seul élément (dernière valeur reçue) afin d'éviter d'accumuler du retard.
- **Template Usage:** La tâche doit être paramétrée par le type de drone (`DroneType`) pour adapter la logique de mixage des moteurs en fin de boucle.
- **Safety:** Si le message `RcFrame` n'est pas reçu pendant plus de 100ms, la tâche doit signaler un défaut au module de sécurité.

### Architecture Compliance
- Respect de la FR-01 et FR-02 (Machine à états).
- Utilisation de la `SystemClock` pour le marquage temporel de la commande actionneur.

### File Structure Requirements
- `Modules/FlightControl/FlightControlTask.hpp` : Logique de la tâche et boucle principale.

## Implementation Strategy
1. **Séquencement RTOS:** Implémenter la méthode `run()` qui sera appelée par le point d'entrée de tâche défini dans `ApplicationBootstrap`.
2. **Acquisition de Données:** Implémenter la récupération non-bloquante via `input_queue.pop()`.
3. **Logique de Mixage:** Ajouter une étape de mixage après les calculs PID pour convertir les sorties Roll/Pitch/Yaw/Throttle en commandes individuelles pour les moteurs (8 canaux supportés).
4. **Gestion du Temps:** Capturer le timestamp `TIM2` au début de chaque cycle pour calculer précisément le `dt` passé aux PIDs.

## Validation Plan
- **Cycle Time Check:** Mesurer via un oscilloscope ou un port de trace la régularité de la boucle (doit rester entre 2490µs et 2510µs).
- **Input Responsiveness:** Injecter un changement brusque dans la `RcFrame` simulée et vérifier que l' `ActuatorCmd` réagit dès le cycle suivant.
- **Jitter Audit:** Vérifier que le traitement ne dépasse jamais le temps alloué (2.5ms) même dans les cas de charge CPU maximale.

## Project Context Reference
- `PRD` : FR-01, FR-02.
- `architecture.md` : Section "FlightControlTask" (400Hz, Prio 6/7).
- `BusMessages.hpp` : Pour les structures `RcFrame`, `StateVector` et `ActuatorCmd`.
