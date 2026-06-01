# Plan de Test Architectural : KhaNeSystem

## 1. Objectif
Garantir la sécurité "Flight-Critical" du drone KhaNeSystem par une approche de test robuste, déterministe et automatisée.

## 2. Stratégie par Niveaux
| Niveau | Cible | Outils | Objectif |
| :--- | :--- | :--- | :--- |
| **Unitaire** | Logique métier (PID, EKF, Bus) | `googletest` | Validation mathématique et logique. |
| **Intégration** | Couche HAL, RTOS, Drivers | `Fake/Mock` | Validation des interactions drivers. |
| **E2E (SIL)** | Boucle de vol complète | `sil_sim` (AirSim) | Validation dynamique en vol virtuel. |
| **HITL** | Hardware complet | STM32F4 + Sim | Validation temps-réel matériel. |

## 3. Qualité et Gouvernance (CI Gates)
- **Taux de couverture minimal** : 80% (Modules critiques : SensorFusion, FlightControl).
- **Zéro Warning** : Compilation avec `-Werror`.
- **Mémoire** : Vérification CI du dépassement CCM et des allocations dynamiques.

## 4. Priorités d'implémentation
1. **Infrastructure TU** : Setup `googletest` dans le pipeline.
2. **Tests Critiques** : Couverture totale `PidController` et `EventBus`.
3. **Tests de Simulation** : Automatisation des scénarios SIL.

*Murat, Master Test Architect.*
