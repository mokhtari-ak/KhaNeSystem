# Project Instructions: KhaNeSystem

## Prohibited Patterns
- **Include Directives:** The direct inclusion of `stm32f4xx_hal.h` is STRICTLY PROHIBITED outside of the directory `Libs/HardwareAccessLayer/STM32F4HAL/`.
  - Violations will be flagged by CI using clang-tidy.
- **Dynamic Allocation:** `malloc`, `new`, `calloc`, `free` are prohibited in all flight-critical modules.
- **FPU usage:** All calculations must use `float`. `double` is prohibited to prevent software-emulation overhead on the STM32F407VG.

## Development Standards
- **Memory:** All DMA buffers must NOT be placed in the CCM (Core Coupled Memory).
- **Errors:** All functions returning values must use the `Result<T, E>` pattern instead of throwing exceptions or returning raw integer error codes.
- **Language:** C++20 standard. No exceptions, no RTTI.

## Existing Libraries
- **Wrappers (`Libs/wrappers`):** The libraries `Wrappers`, `WrapperTypes`, and `WrapperPolicies` already exist. Do NOT re-implement these; verify their current implementation, ensure they align with the C++20 concepts and standards, and exploit them.
- **HAL (`Libs/HardwareAccessLayer/STM32F4HAL`):** The `STM32F4HAL` library exists. It must be verified for correctness, safety, and adherence to the project standards. Correct it where necessary rather than creating a new HAL abstraction from scratch.
