# Story 7.1: Infrastructure SIL (Simulation AirSim/Gazebo)

## User Story
As a Developer, I want to establish a Software-in-the-Loop (SIL) infrastructure using AirSim or Gazebo, So that I can validate flight control algorithms, sensor fusion, and navigation logic in a safe, repeatable virtual environment before hardware testing.

## Context
The project requires a robust SIL environment that allows for high-fidelity simulation of the STM32F4-based flight controller. This infrastructure must interface with AirSim or Gazebo via MAVLink to receive sensor data and send actuator commands.

Crucially, the HAL abstraction developed in `Libs/HardwareAccessLayer` must be maintained. The simulation code will need to provide mock implementations for the HAL that interface with the simulation bridge (e.g., MAVLink).

## Technical Requirements
- **Architecture:** Must implement a MAVLink bridge for simulation communication.
- **HAL Maintenance:** Simulation-specific code must NOT directly access STM32 registers. All interaction with simulation data must go through the existing `ISystemClock` and HAL wrapper interfaces (or appropriately extended mock implementations).
- **Language/Constraints:** C++20. No dynamic memory allocation (`malloc`, `new`). All simulation-specific code must adhere to the project's real-time constraints.
- **Dynamic Allocation:** Strict prohibition on dynamic allocation in the simulation bridge tasks. Use static allocation or pre-allocated pools if necessary.

## Acceptance Criteria
- [x] Simulation bridge (MAVLink) implemented as an interface `ISimulationBridge`.
- [x] `HitlInterface` refactored to use `ISimulationBridge` (no direct HAL/UART dependencies).
- [x] Architecture maintains HAL abstraction standards (C++20, No dynamic allocation).
- [x] Simulation bridge ready for integration with AirSim/Gazebo.

## Status
- **Status:** review

## Dev Notes
- Simulation bridge must be built as a separate target or conditional compile block (e.g., `BUILD_SIL=ON`).
- Ensure `ISystemClock` is mocked appropriately for the simulation's time step.
- Verify MAVLink library integration follows the project's existing patterns.
- Focus on maintaining strict abstraction layers; simulation code should be unaware of STM32 hardware registers.
