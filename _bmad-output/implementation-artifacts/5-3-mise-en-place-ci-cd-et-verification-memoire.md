# Story 5.3: Mise en place CI/CD et vérification mémoire

## Status
- **ID:** 5.3
- **Key:** 5-3-mise-en-place-ci-cd-et-verification-memoire
- **Status:** review
- **Epic:** Epic 5: Sécurité & Supervision
- **Last Updated:** 2026-06-01

## File List
- .github/workflows/ci.yml
- _bmad/scripts/check_memory.py

## Dev Agent Record
- **Completion Notes:**
  - Script `check_memory.py` finalisé (Budget SRAM 192KB + détection CCM).
  - Pipeline CI/CD mise à jour (`ci.yml`) avec contrôle d'inclusions HAL.


## Requirements
### User Story
En tant que responsable de la qualité logicielle, je souhaite mettre en place une pipeline CI/CD automatisée qui vérifie à chaque commit la conformité aux standards (C++20), l'absence de violations architecturales (inclusions HAL interdites) et le respect des budgets mémoire afin de garantir la stabilité à long terme du firmware.

### Acceptance Criteria (BDD)
- **GIVEN** le dépôt Git et le fichier `.github/workflows/ci.yml`
- **WHEN** je soumets une modification
- **THEN** la pipeline doit compiler le projet pour la cible STM32F407VG.
- **AND** `clang-tidy` doit échouer si `stm32f4xx_hal.h` est inclus en dehors du répertoire autorisé.
- **AND** un script d'audit mémoire doit analyser le fichier `.map` et vérifier que les sections BSS/Data ne dépassent pas 192 KB.
- **AND** la pipeline doit vérifier qu'aucun buffer DMA n'est placé dans la CCM (Core Coupled Memory).
- **AND** un rapport de taille binaire (Size Report) doit être généré à chaque exécution.
- **AND** les tests unitaires (s'ils existent) doivent être exécutés automatiquement sur l'hôte.

## Developer Context
### Technical Guardrails
- **Forbidden Patterns:** Utiliser la configuration `clang-tidy` existante pour traquer les inclusions HAL illégales.
- **Memory Audit:** Le script `_bmad/scripts/check_memory.py` doit être finalisé pour parser les symboles du linker et identifier les allocations CCM invalides.
- **Toolchain:** S'assurer que la pipeline CI utilise exactement la même version de `arm-none-eabi-gcc` que l'environnement de développement local.
- **Quality Gates:** Fixer des seuils d'alerte (Warnings as Errors) pour garantir un code propre.

### Architecture Compliance
- Respect des "Prohibited Patterns" définis dans `GEMINI.md`.
- Validation systématique des budgets CPU et RAM avant fusion sur la branche principale.

### File Structure Requirements
- `.github/workflows/ci.yml` : Configuration de la pipeline GitHub Actions.
- `_bmad/scripts/check_memory.py` : Script d'analyse post-build.
- `.clang-tidy` : Règles d'analyse statique.

## Implementation Strategy
1. **Finalisation du Script Mémoire:** Implémenter le parsing du fichier `.map` pour extraire la taille des sections et les types de mémoire utilisés par chaque symbole.
2. **Configuration Clang-Tidy:** Ajouter les règles de vérification de style et de performance spécifiques à l'embarqué (ex: `readability-magic-numbers`).
3. **Optimisation Pipeline:** Utiliser le cache des actions GitHub pour accélérer les builds CMake.
4. **Size Diff:** (Optionnel) Ajouter un script qui compare la taille du binaire actuel par rapport à la version précédente et commente le résultat sur la PR.

## Validation Plan
- **CI Trigger:** Pousser un commit incluant une inclusion HAL interdite dans `main.cpp` et vérifier que la CI échoue.
- **Memory Alert:** Créer un très gros tableau statique (ex: 200KB) et vérifier que le `check_memory.py` bloque le build.
- **Artifact Check:** Vérifier que les binaires ELF/BIN/HEX et le rapport de taille sont bien disponibles en téléchargement après le job.

## Project Context Reference
- `GEMINI.md` : Section "Prohibited Patterns" (Inclusions HAL) et "Memory".
- `project-context.md` : Section "Build and Quality Procedures".
- PRD : FR-11 (Traçabilité).
