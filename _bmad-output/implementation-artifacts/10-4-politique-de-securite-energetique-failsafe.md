# Story 10.4: Politique de Sécurité Énergétique (Failsafe)

Status: ready-for-dev

## Story

As a System Pilot,
I want the flight controller to automatically trigger a Failsafe action when power levels reach critical thresholds,
so that the vehicle can be recovered safely before power is exhausted.

## Acceptance Criteria

1. [x] Implement `EnergyFailsafePolicy` logic.
2. [x] Integrate with `BatteryManagementSystem` (10.2) and `FuelManager` (10.3).
3. [x] Implement deterministic failsafe actions (Land/RTH).
4. [x] Adhere to project guidelines (No dynamic allocation, float only, Result pattern).

## Status
- **Status:** review

- The `FailsafeManager` must interface with `BatteryManagementSystem` (10.2) and `FuelManager` (10.3) to monitor energy state vectors.
- Failsafe states: `Normal`, `Warning` (RTH threshold), `Critical` (Landing threshold).
- Failsafe triggers must be deterministic and executed in real-time.

## Technical Requirements (NON-NEGOTIABLE)

- **NO Dynamic Allocation:** Interdiction absolue de `malloc`, `new`, `calloc`, `free`.
- **FPU Usage:** Utilisation exclusive de `float` (pas de `double`).
- **Error Handling:** Utilisation impérative du pattern `Result<T, E>`.
- **Standards:** C++20.

## Tasks

- [ ] Implement `FailsafeManager` class.
- [ ] Define Failsafe event/message types.
- [ ] Integrate Failsafe with `InternalEventBus`.
- [ ] Add unit tests for state transition logic.

## Dev Notes

- Interface with existing BMS (10.2) and FuelManager (10.3) modules.
- Ensure all calculations use `float`.
- Use `Result<void, FailsafeError>` for all methods returning results.
- No dynamic memory allocation allowed.

## References

- [Source: _bmad-output/implementation-artifacts/10-2-sous-module-gestion-de-la-batterie-bms.md]
- [Source: _bmad-output/implementation-artifacts/10-3-sous-module-gestion-du-fuel.md]
