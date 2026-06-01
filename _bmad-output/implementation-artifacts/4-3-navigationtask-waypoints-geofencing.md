# Story 4.3: NavigationTask (Waypoints, Geofencing)

## Status
- **ID:** 4.3
- **Key:** 4-3-navigationtask-waypoints-geofencing
- **Status:** review
- **Epic:** Epic 4: Flight Control & Navigation
- **Last Updated:** 2026-06-01

## File List
- Modules/Navigation/NavigationTask.hpp

## Dev Agent Record
- **Completion Notes:**
  - Implémentation de la boucle 50Hz avec `vTaskDelayUntil`.
  - Logique de passage automatique de waypoint (rayon 2m).
  - Calcul de guidage basique (cap, altitude).
  - Surveillance Geofencing (distance point origine) avec émission de `FaultEvent`.
## Requirements
### User Story
En tant que développeur de mission, je souhaite implémenter une tâche de navigation (50Hz) capable de suivre une séquence de waypoints et de surveiller les limites géographiques (Geofencing) afin de garantir que le drone reste dans son enveloppe de vol autorisée tout en atteignant ses objectifs.

### Acceptance Criteria (BDD)
- **GIVEN** le module `Navigation` et l'Internal Event Bus
- **WHEN** j'implémente la tâche `NavigationTask`
- **THEN** elle doit s'exécuter périodiquement à 50Hz.
- **AND** elle doit gérer une mission statique de 256 waypoints maximum.
- **AND** elle doit calculer les consignes de guidage (`target_heading`, `target_alt`, `xte`) vers le prochain waypoint.
- **AND** un mécanisme de Geofencing cylindrique (rayon max, altitude max) doit déclencher un `FaultEvent` en cas de violation.
- **AND** le passage au waypoint suivant doit être automatique une fois le rayon d'acceptation atteint.
- **AND** aucune allocation dynamique ne doit être effectuée lors de l'exécution de la mission.

## Developer Context
### Technical Guardrails
- **Waypoint Storage:** Utiliser `std::array<Waypoint, 256>` pour éviter toute allocation dynamique au runtime.
- **Geofencing:** Implémenter une vérification systématique de la position actuelle par rapport à un point "Home" et des limites d'altitude/distance définies dans la configuration.
- **Guidance Math:** Utiliser des formules de navigation (Loxodromie ou Orthodromie simplifiée) pour le calcul du cap (`heading`) et de l'erreur latérale (`xte`).
- **Determinism:** La complexité du calcul doit être indépendante du nombre de waypoints dans la mission.

### Architecture Compliance
- Respect de la FR-06 (Navigation Waypoints) et FR-07 (FAILSAFE).
- Utilisation de `bus::NavGuidance` pour communiquer avec la `FlightControlTask`.

### File Structure Requirements
- `Modules/Navigation/NavigationTask.hpp` : Logique de guidage et surveillance.

## Implementation Strategy
1. **Séquencement de Mission:** Implémenter la logique de passage au waypoint suivant (détection de rayon).
2. **Calcul de Guidage:** Implémenter le calcul de l'angle vers le waypoint et de l'erreur de trajectoire (`xte`).
3. **Logiciel de Geofencing:** Ajouter une méthode `check_geofence()` appelée à chaque cycle qui vérifie la distance par rapport au point de décollage.
4. **Publication Bus:** Envoyer le message `NavGuidance` périodiquement.
5. **Gestion des Erreurs:** Publier un `FaultEvent` sur le bus en cas de violation de zone ou de perte de position GPS valide.

## Validation Plan
- **Unit Testing (Mission logic):** Vérifier que la tâche passe bien du waypoint 1 au 2 quand la position simulée approche de la cible.
- **Geofence Test:** Simuler une position au-delà du rayon limite et vérifier que le bus reçoit un `FaultEvent`.
- **Performance:** S'assurer que les calculs de trigonométrie (très fréquents en navigation) sont optimisés pour la FPU.

## Project Context Reference
- `PRD` : FR-06, FR-07.
- `architecture.md` : Section "NavigationTask" (50Hz).
- `BusMessages.hpp` : Structure `NavGuidance` et `FaultEvent`.
