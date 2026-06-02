# Story 10.1: Driver Moniteur de puissance (ADC/I2C/SMBus)

Status: ready-for-dev

## Story

As a System Monitor,
I want a power monitor driver that interfaces with hardware via ADC or I2C/SMBus,
so that I can accurately report system power consumption and battery health to ensure safe flight operations.

## Acceptance Criteria

1. [x] Implement `IPowerMonitor` interface.
2. [x] Implement `AdcPowerMonitor` driver using `IAdcDriver`.
3. [x] Ensure accurate data scaling (raw ADC to Volts/Amps).
4. [x] Adhere to project guidelines (No dynamic allocation, float only, Result pattern).

## Status
- **Status:** review

## Tasks / Subtasks

- [ ] Design PowerMonitor abstraction
- [ ] Implement I2C/SMBus/ADC hardware interface using STM32F4HAL
- [ ] Implement data acquisition logic (float conversions)
- [ ] Implement error handling using Result<T, E>
- [ ] Create unit tests for PowerMonitor

## Dev Notes

- **Architectural Constraints:**
  - Strictly use `float` for calculations (no `double`).
  - No dynamic allocation (`malloc`, `new`, etc.) in this module.
  - MUST use `Result<T, E>` for all error reporting.
  - Use `STM32F4HAL` wrappers for hardware access.
  - No direct inclusion of `stm32f4xx_hal.h` outside of `Libs/HardwareAccessLayer/STM32F4HAL/`.
- **Source tree:**
  - Driver implementation should be under `Modules/Sensors/`.
  - Interfaces/Wrappers should be under `Libs/wrappers/`.
- **Testing:**
  - Create tests in `tests/` and use mocks for I2C/ADC hardware interface to verify logic and error handling.

### References

- [Source: GEMINI.md#Project Instructions]
- [Source: architecture.md#SystemClock Specification] (for time management patterns)
