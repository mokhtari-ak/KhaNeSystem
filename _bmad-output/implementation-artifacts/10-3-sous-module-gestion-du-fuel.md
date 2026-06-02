# Story 10.3: Sous-module Gestion du Fuel

## Status
- **Status:** ready-for-dev
- **Epic:** 10 (Power Management & System Health)
- **Key:** 10-3-sous-module-gestion-du-fuel

## Description
Implémenter le module de gestion du carburant (fuel) pour surveiller le débit, estimer le niveau de carburant restant et calculer l'autonomie de vol restante.

## Context
- Le module Fuel doit interagir avec les modules Power et BMS existants pour corréler la consommation énergétique et le carburant.
- Ce module est critique pour la sécurité du vol et la planification de mission.

## Architectural Requirements
- **Fuel Monitoring:** Surveillance en temps réel du débit de carburant.
- **Remaining Flight Time:** Calcul de l'autonomie restante basé sur le débit actuel et la capacité totale.
- **Integration:** Doit s'interfacer proprement avec `IPowerMonitor` et le module BMS pour une vision globale de l'énergie.

## Technical Requirements (NON-NEGOTIABLE)
- **NO Dynamic Allocation:** Interdiction absolue de `malloc`, `new`, `calloc`, `free`.
- **FPU Usage:** Utilisation exclusive de `float` (pas de `double`).
- **Error Handling:** Utilisation impérative du pattern `Result<T, E>`.
- **Standards:** C++20.

## Acceptance Criteria

- [x] Implement `FuelManager` logic for consumption estimation.
- [x] Integrate with `IPowerMonitor` interface (10.1).
- [x] Adhere to project guidelines (No dynamic allocation, float only, Result pattern).

## Status
- **Status:** review

## Developer Notes
- Vérifier les dépendances `Libs/wrappers`.
- S'assurer de l'utilisation correcte des types flottants pour les calculs.
- Le module doit être déterministe et conforme aux contraintes temps-réel.
