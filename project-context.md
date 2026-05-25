# Project Context: KhaNeSystem

This file defines the foundational technical, architectural, and security constraints for the KhaNeSystem project. All agents must adhere to these rules.

## Core Technical Constraints
- **Target Architecture:** STM32F407VG microcontroller.
- **Language Standard:** C++20.
- **Real-Time Requirements:** The application is a hard real-time system using FreeRTOS.
- **Prohibited C++ Features:**
  - Exceptions (`-fno-exceptions`) are strictly prohibited.
  - Runtime Type Information (`-fno-rtti`) is strictly prohibited.
  - Dynamic memory allocation (e.g., `new`, `malloc`) is strictly prohibited in real-time tasks.
- **Coding Standards:** Must adhere to project-specific `clang-tidy` rules (see `.clang-tidy` or equivalent configuration).

## Architectural Constraints
- **Hardware Abstraction:** Direct hardware access must be abstracted.
  - **HAL Restriction:** The inclusion of `stm32f4xx_hal.h` (and its derivatives) is **STRICTLY PROHIBITED** outside of the `Libs/HardwareAccessLayer/STM32F4HAL/` directory.
  - Use the provided C++ wrappers located in `Libs/wrappers/` for hardware interactions.

## Security & Reliability
- **Safety:** Do not introduce code that exposes, logs, or commits secrets, API keys, or sensitive credentials.
- **Maintainability:** Prioritize explicit composition and delegation over complex inheritance. Maintain type safety throughout the codebase.
- **Validation:** Every change must be verified through project-specific build, linting, and testing procedures.

## Agent Guidelines
- Agents must follow the Research -> Strategy -> Execution lifecycle.
- Validation is mandatory for all tasks.
- Do not bypass the type system or disable warnings/errors.
