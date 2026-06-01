# Story 5.2: ConfigurationManager & Persistance Flash

## Status
- **ID:** 5.2
- **Key:** 5-2-configurationmanager-persistance-flash
- **Status:** review
- **Epic:** Epic 5: Sécurité & Supervision
- **Last Updated:** 2026-06-01

## File List
- Modules/ConfigurationManager/ConfigurationManager.hpp
- Modules/ConfigurationManager/FlashStorage.hpp

## Dev Agent Record
- **Completion Notes:**
  - Implémentation du système double-buffer (Secteurs A/B) avec validation CRC32 matérielle.
  - Sauvegarde atomique avec séquenceur pour identifier la version la plus récente.
  - Structure `ConfigParams` alignée sur 32 bits.


## Requirements
### User Story
En tant que pilote de drone, je souhaite que mes réglages (gains PID, seuils batterie, etc.) soient sauvegardés de manière permanente dans la mémoire flash du MCU afin qu'ils soient conservés après une mise hors tension et validés à chaque démarrage.

### Acceptance Criteria (BDD)
- **GIVEN** le module `ConfigurationManager` et la mémoire Flash STM32F4
- **WHEN** je modifie et sauvegarde un paramètre
- **THEN** la donnée doit être écrite dans l'un des deux secteurs de configuration (A/B) de manière atomique.
- **AND** un CRC32 doit être calculé et stocké pour chaque bloc de configuration.
- **AND** au démarrage, le manager doit charger le secteur le plus récent dont le CRC est valide (FR-09).
- **AND** si aucun secteur n'est valide, des valeurs par défaut sécurisées doivent être chargées.
- **AND** l'accès aux paramètres par les autres modules doit être immédiat (O(1)).
- **AND** aucune allocation dynamique (tas) ne doit être utilisée.

## Developer Context
### Technical Guardrails
- **Flash Safety:** L'effacement et l'écriture en Flash sont des opérations bloquantes et critiques. Elles doivent être effectuées hors des boucles de contrôle de vol (ex: uniquement au sol ou dans une tâche basse priorité).
- **Double Buffering:** Utiliser deux secteurs flash distincts. Écrire alternativement dans l'un puis l'autre en incluant un compteur de séquence pour identifier la version la plus récente.
- **CRC Hardware:** Exploiter le périphérique matériel CRC du STM32F4 pour une validation rapide au boot.
- **Alignment:** La structure `ConfigParams` doit être alignée sur 32 bits (`alignas(4)`) pour l'écriture Flash.

### Architecture Compliance
- Respect de la FR-09 (Séquence de démarrage déterministe).
- Utilisation de `hal::Result<void>` pour toutes les opérations d'entrée/sortie.

### File Structure Requirements
- `Modules/ConfigurationManager/ConfigurationManager.hpp` : Logique applicative et accès aux paramètres.
- `Modules/ConfigurationManager/FlashStorage.hpp` : Couche d'accès bas-niveau à la Flash interne.

## Implementation Strategy
1. **Implémentation CRC:** Activer l'horloge du périphérique CRC et implémenter la méthode `calculate_crc()`.
2. **Gestion de la Persistence:** Implémenter la logique de détection du dernier secteur valide au démarrage.
3. **Double-Buffer Flash:** Implémenter l'alternance entre `FLASH_SECTOR_10` et `FLASH_SECTOR_11`.
4. **Validation des Bornes:** Ajouter des vérifications de sécurité dans `set()` pour empêcher des gains PID aberrants.
5. **Atomicité:** S'assurer que le CRC n'est écrit qu'à la toute fin de l'opération de programmation flash.

## Validation Plan
- **Persistence Test:** Sauvegarder une valeur, éteindre le MCU (virtuellement en simulation), et vérifier qu'elle est rechargée correctement.
- **CRC Validation:** Corrompre volontairement un octet en Flash et vérifier que le manager rejette le secteur et charge les valeurs par défaut.
- **Flash Stress Test:** Vérifier que le système de double-buffer gère correctement une coupure d'alimentation au milieu d'une écriture (le secteur précédent doit rester valide).

## Project Context Reference
- `PRD` : FR-09, FR-11.
- `project-context.md` : Section "Memory Constraints".
- `GEMINI.md` : Prohibitions d'allocation dynamique.
