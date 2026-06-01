# Story 2.5: Développement du sous-module Storage (Abstraction FS)

## Status
- **ID:** 2.5
- **Key:** 2-5-module-logger-ecriture-brute-sd-card-via-dma
- **Status:** review
- **Epic:** Epic 2: RTOS & Communication
- **Last Updated:** 2026-06-01

## File List
- Modules/Logger/StorageAbstraction.hpp
- Modules/Logger/SdDriver.hpp

## Dev Agent Record
- **Completion Notes:**
  - Interface `IStorage` définie pour l'écriture asynchrone.
  - Structure `SdDriver` créée avec mécanisme de buffer double (ping-pong) pour l'écriture DMA.
  - Respect des contraintes : simple précision `float`, zéro allocation dynamique.


## Requirements
### User Story
En tant que développeur système, je souhaite implémenter une abstraction robuste pour le système de fichiers (FS) afin de permettre l'enregistrement de données télémétriques haute fréquence sur une carte SD via DMA, sans blocage de la boucle temps-réel.

### Acceptance Criteria (BDD)
- **GIVEN** le module `Logger` et une carte SD accessible via SPI/SDIO
- **WHEN** j'implémente l'abstraction `Storage`
- **THEN** elle doit fournir une interface `write()` asynchrone utilisant le DMA.
- **AND** le système doit gérer la rotation des fichiers de log (ex: log001.bin, log002.bin).
- **AND** aucune allocation dynamique ne doit être effectuée lors de l'écriture des données.
- **AND** la performance d'écriture doit être suffisante pour 50KB/s de télémétrie.

## Developer Context
### Technical Guardrails
- **DMA Usage:** Utiliser le DMA pour le transfert des buffers vers la SD pour minimiser la charge CPU.
- **Double Buffering:** Utiliser deux buffers (ping-pong) pour permettre l'écriture DMA d'un buffer pendant que l'autre est rempli par la tâche de log.
- **Determinism:** La latence d'accès à la carte SD ne doit jamais bloquer les tâches critiques.

### Architecture Compliance
- Utilisation de `hal::Result<void>` pour les erreurs d'E/S.

### File Structure Requirements
- `Modules/Logger/StorageAbstraction.hpp` : Interface du système de fichiers.
- `Modules/Logger/SdDriver.hpp` : Driver bas-niveau SD/SPI/SDIO.

## Implementation Strategy
1. **Abstraction FS:** Définir l'interface `IStorage` avec `write()` et `sync()`.
2. **Driver SD:** Implémenter `SdDriver` basé sur la HAL ST (SPI/SDIO).
3. **Buffering:** Implémenter le mécanisme de double-buffer pour le DMA.

## Validation Plan
- **Performance Test:** Mesurer le débit d'écriture réel vers la carte SD.
- **Robustness:** Simuler l'éjection de la carte SD pendant l'écriture et vérifier que le système gère proprement l'erreur.
