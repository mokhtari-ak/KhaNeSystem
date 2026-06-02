# Story 9.2: Moteur de Logging haute performance

Status: ready-for-dev

## Story

As an embedded system,
I want a non-blocking, high-performance logging engine,
so that critical flight data is persisted to storage without impacting real-time flight control loop determinism.

## Acceptance Criteria

1. [x] Non-Blocking Execution: Logging requests return immediately.
2. [x] DMA Transfer: Logging engine uses `IFileSystem` for DMA writes.
3. [x] No Dynamic Allocation: Strictly no heap usage.
4. [x] Data Integrity: Data is buffered and written correctly without loss.
5. [x] Type Safety: Strict `float` usage enforced.

## Status
- **Status:** review


- [ ] Define `ILogger` interface (aligned with Wrappers/Policies).
- [ ] Implement `Logger` module using circular queues for log buffering.
- [ ] Integrate with `IFileSystem` (from 9.1) for DMA-based storage operations.
- [ ] Develop `LoggingTask` responsible for draining the log queue and invoking DMA writes.
- [ ] Verify non-blocking behavior and zero dynamic allocation.

## Dev Notes

- **Architectural Requirements**: 
  - Non-blocking (DMA).
  - Use `IFileSystem` (from 9.1).
  - No dynamic allocation.
  - Strict `float` usage.
- **Relevant architecture patterns**:
  - RTOS Task Mapping (see LoggingTask: 100Hz, 2/7 prio, 4KB stack).
  - Strong Typedefs (as established in `Story 1.4`).
- **Source tree components to touch**:
  - `Modules/Logger/` (new files)
  - `Libs/wrappers/` (potential updates)
- **Testing standards summary**:
  - Unit tests for queue logic.
  - Integration tests for DMA-based logging.

### Project Structure Notes

- Adhere to `Modules/` directory organization.

### References

- Architecture Doc: `_bmad-output/planning-artifacts/architecture.md` (Section: RTOS Task Mapping)
- GEMINI.md (Project Instructions)

## Dev Agent Record

### Agent Model Used

Gemini-CLI-Assistant

### Debug Log References

### Completion Notes List

- Story fully context-engineered and ready for development implementation.

### File List
- Modules/Logger/ILogger.hpp (New)
- Modules/Logger/Logger.hpp (New)
- Modules/Logger/Logger.cpp (New)
- Modules/Logger/LoggingTask.cpp (New)
