# Story 9.1: FileSystem Abstraction

## User Story Statement
As a system developer, I want a `FileSystem` interface that abstracts storage media access (LittleFS/FatFS), so that I can implement filesystem-dependent features without being tied to specific hardware or underlying filesystem implementations.

## Architectural Requirements
- **Interface Definition**: Define a `FileSystem` C++20 interface with the following methods:
  - `Result<FileHandle, Error> open(const char* path, OpenMode mode)`
  - `Result<size_t, Error> read(FileHandle handle, void* buffer, size_t size)`
  - `Result<size_t, Error> write(FileHandle handle, const void* buffer, size_t size)`
  - `Result<void, Error> seek(FileHandle handle, size_t position)`
  - `Result<void, Error> close(FileHandle handle)`
- **Abstraction**: Must maintain strict HAL abstraction. Direct hardware access (registers, raw HAL calls) is forbidden within the filesystem abstraction layer.
- **HAL Layering**: All interactions must go through the existing `Libs/HardwareAccessLayer/STM32F4HAL/` library.

## Acceptance Criteria (GIVEN/WHEN/THEN)
- **GIVEN** a storage medium initialized with a filesystem (LittleFS or FatFS),
  **WHEN** `FileSystem::open` is called with a valid path,
  **THEN** a valid `FileHandle` should be returned.
- **GIVEN** an open file handle,
  **WHEN** `FileSystem::write` is called with a buffer,
  **THEN** the data should be correctly written to the medium, and the number of bytes written should be returned.
- **GIVEN** an open file handle,
  **WHEN** `FileSystem::read` is called,
  **THEN** the requested amount of data should be read into the buffer, and the number of bytes read should be returned.
- **GIVEN** an open file handle,
  **WHEN** `FileSystem::seek` is called,
  **THEN** the file pointer should be moved to the specified position.

## Technical Constraints & Guardrails
- **Dynamic Allocation**: ABSOLUTELY PROHIBITED. No `malloc`, `new`, `calloc`, or `free` are allowed.
- **Arithmetic**: Only `float` is allowed for calculations. `double` is strictly prohibited. All integer arithmetic should be explicit.
- **Result Pattern**: MUST use the project's `Result<T, E>` pattern for all function returns that can fail.
- **Compliance**: Must adhere to C++20 standard, no exceptions, no RTTI.

## Developer Context
- This story is part of the Storage abstraction effort.
- Ensure the interface is extensible to support different backend implementations (LittleFS vs. FatFS) using C++20 concepts/polymorphism.
- The developer MUST NOT re-implement existing wrappers. Verify `Libs/wrappers` for existing utilities.

## Status
Status: review
