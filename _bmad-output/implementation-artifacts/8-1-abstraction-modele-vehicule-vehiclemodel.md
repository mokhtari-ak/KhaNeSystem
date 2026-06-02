# Story 8.1: Abstraction du modèle de véhicule (VehicleModel Interface)

Status: ready-for-dev

## Story

As a Control System Architect,
I want a unified `VehicleModel` interface,
so that control algorithms (PID, Navigation) remain decoupled from specific vehicle dynamics (Quadcopter vs. Fixed-Wing).

## Acceptance Criteria

1. [x] Define a C++20 `VehicleModel` concept/interface in `Modules/FlightControl/`.
2. [x] Ensure the interface provides methods for predicting state evolution based on control inputs (actuator signals).
3. [x] Demonstrate implementation compatibility for both Quadcopter and Fixed-Wing dynamics models.
4. [x] Ensure the interface supports only `float` arithmetic (no `double` allowed).
5. [x] Ensure the interface does not use dynamic allocation (`malloc`/`new`).

## Status
- **Status:** review

## Tasks / Subtasks

- [x] Define `VehicleModel` C++20 Concept/Interface
- [x] Implement `QuadcopterModel` prototype
- [x] Implement `FixedWingModel` prototype
- [x] Integration tests in SIL environment (if applicable)

## Dev Notes

- **Language**: C++20, no exceptions, no RTTI.
- **Floating Point**: Strict `float` only.
- **Dynamic Allocation**: Prohibited.
- **HAL Abstraction**: Adhere to `Libs/HardwareAccessLayer/STM32F4HAL` for any underlying hardware interaction, though this model should ideally be pure math/logic.
- **Decoupling Strategy**: The FlightController should interact only with the `VehicleModel` interface, not specific dynamic model implementations.

### Project Structure Notes

- New code to be placed in `Modules/FlightControl/`.

### References

- [Source: project-context.md] (Prohibited patterns: HAL usage, FPU, dynamic allocation)
- [Source: _bmad-output/planning-artifacts/architecture.md]

## Dev Agent Record

### Agent Model Used

Gemini

### Debug Log References

### Completion Notes List

### File List

- `Modules/FlightControl/VehicleModel.hpp` (New)
- `Modules/FlightControl/QuadcopterModel.hpp` (New)
- `Modules/FlightControl/FixedWingModel.hpp` (New)
