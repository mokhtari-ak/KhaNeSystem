# Story 10.5: Contrôle de l'alimentation des périphériques

Status: ready-for-dev

## Story

As a Power Management System,
I want to control the power supply to various peripherals using GPIO/MOSFET switches,
so that I can minimize power consumption and enhance system safety by isolating failed components.

## Acceptance Criteria

1. GIVEN the system is powered on, WHEN a peripheral's power control command is received, THEN the corresponding GPIO/MOSFET switch must change state to match the requested power state (ON/OFF).
2. GIVEN a peripheral is malfunctioning (e.g., reported by SafetyTask), WHEN the failsafe policy triggers, THEN the peripheral's power must be switched OFF automatically to prevent system-wide impact.
3. GIVEN the system is in low-power mode, WHEN the system enters low-power state, THEN all non-essential peripherals must be switched OFF.
4. GIVEN a power-on request for a peripheral, WHEN the peripheral is powered on, THEN the system must ensure the power supply stabilizes before attempting communication with the peripheral.

## Tasks / Subtasks

- [ ] Task 1: Hardware Abstraction for Power Control (GPIO/MOSFET)
  - [ ] Implement `PowerSwitch` interface using `Result<T, E>` pattern
  - [ ] Implement STM32F4-specific HAL wrapper for power switches
- [ ] Task 2: Peripheral Power Controller Module
  - [ ] Develop `PeripheralPowerController` module
  - [ ] Integrate with `FaultBus` for automatic isolation on fault
- [ ] Task 3: Integration and Testing
  - [ ] Verify non-impact on critical flight systems (Safety isolation)
  - [ ] Ensure no dynamic allocation (use static allocation/pre-allocation)
  - [ ] Use `float` for all monitoring values
  - [ ] Perform unit testing for power control logic

## Dev Notes

- **Architecture:** `PowerManagement` module
- **Constraints:**
  - Strictly use `float` (no `double`).
  - No dynamic allocation (`malloc`/`new`/etc.).
  - Use `Result<T, E>` for all return types.
  - No direct inclusion of `stm32f4xx_hal.h` outside HAL wrappers.
- **Source:** `Modules/PowerManagement/`
- **Dependencies:** `Libs/HardwareAccessLayer/STM32F4HAL`

### Project Structure Notes

- Adhere to `Modules/` structure.
- Follow existing patterns for `HAL` and `Wrappers`.

### References

- [Source: GEMINI.md]
- [Source: Modules/PowerManagement/...]

## Dev Agent Record

### Agent Model Used

Gemini-CLI-Dev-Agent

### Debug Log References

### Completion Notes List

### File List
