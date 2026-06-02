# Story 10.2: Sous-module Gestion de la Batterie (BMS)

## Status
- **Status:** ready-for-dev
- **Epic:** 10 (Power Management & System Health)
- **Key:** 10-2-sous-module-gestion-de-la-batterie-bms

## Description
Implémenter la logique du système de gestion de batterie (BMS) pour surveiller l'état de santé, estimer l'état de charge (SoC) et gérer l'équilibrage des cellules de manière sûre et efficace.

## Context
- Le BMS doit interagir avec le `IPowerMonitor` (implémenté dans 10.1) pour récupérer les mesures de tension et de courant.
- Ce module est critique pour la sécurité du vol et la gestion de l'énergie.

## Architectural Requirements
- **SoC Estimation:** Implémenter un algorithme d'estimation de l'état de charge (SoC) utilisant le comptage de Coulomb (Coulomb Counting) et une compensation basée sur la tension au repos (Open Circuit Voltage).
- **Cell Balancing Logic:** Implémenter la logique d'équilibrage passif des cellules lorsque l'écart de tension dépasse un seuil défini.
- **Data Integrity:** Utiliser des structures de données statiques pour le stockage des états des cellules.

## Technical Requirements (NON-NEGOTIABLE)
- **NO Dynamic Allocation:** Interdiction absolue de `malloc`, `new`, `calloc`, `free`.
- **FPU Usage:** Utilisation exclusive de `float` (pas de `double`).
- **Error Handling:** Utilisation impérative du pattern `Result<T, E>`.
- **Standards:** C++20.

## Acceptance Criteria

### GIVEN: Le système est initialisé
- **WHEN:** Le BMS reçoit les données du `IPowerMonitor`
- **THEN:** Le SoC est calculé et mis à jour.

### GIVEN: Les tensions des cellules sont lues
- **WHEN:** L'écart entre la cellule la plus haute et la plus basse dépasse le seuil d'équilibrage
- **THEN:** La logique d'équilibrage active les résistances de décharge correspondantes.

### GIVEN: Une erreur de lecture survient
- **WHEN:** `IPowerMonitor` retourne un code d'erreur
- **THEN:** Le BMS retourne une erreur typée `Result<void, BMSError>` et applique une stratégie de repli (Failsafe).

## Developer Notes
- S'assurer que les buffers DMA utilisés par `IPowerMonitor` ne sont pas corrompus.
- Le calcul de SoC doit être robuste aux variations de charge.
- Vérifier les dépendances : `Libs/wrappers`.
