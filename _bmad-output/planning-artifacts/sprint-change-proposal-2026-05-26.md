# Sprint Change Proposal - 2026-05-26

## 1. Issue Summary
L'analyse initiale a omis plusieurs modules critiques définis dans le PRD et l'Architecture : `System Clock`, `RF Communication`, `Logger`, et `Calibration/Tuning`. Ces modules sont essentiels pour le déterminisme temporel, la télémétrie, la traçabilité et la sécurité pré-vol du firmware KhaNeSystem.

## 2. Impact Analysis
- **Epic 1**: Ajout d'une story fondamentale pour la source de temps.
- **Epic 2**: Ajout de deux modules de communication asynchrones (RF et SD).
- **Epic 5**: Ajout de la gestion de calibration pour la sécurité.
- **Architecture**: Aucun conflit, ces ajouts complètent les décisions architecturales existantes.

## 3. Recommended Approach
Direct Adjustment: Intégrer les stories manquantes dans le backlog actuel. L'ordre de priorité de l'Épique 1 doit être ajusté car la `SystemClock` est une dépendance pour presque tous les autres modules.

## 4. Detailed Change Proposals

### Stories Updates
- **ADD Story 1.4**: Implémentation de la `SystemClock` (TIM2 32-bit).
- **ADD Story 2.4**: Module RF Communication (MAVLink v2).
- **ADD Story 2.5**: Module Logger (SD Card DMA).
- **ADD Story 5.4**: Module Calibration & Tuning.

## 5. Implementation Handoff
- **Scope**: Moderate (Backlog reorganization needed).
- **Recipient**: Developer Agent.
- **Success Criteria**: Backlog mis à jour et stories prêtes pour l'analyse de contexte.
