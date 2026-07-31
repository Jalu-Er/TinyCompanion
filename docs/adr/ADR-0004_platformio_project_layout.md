# ADR-0004: PlatformIO Project Layout

*   **Status:** Accepted
*   **Date:** 2026-08-01

## Context
TinyCompanion is built under VS Code and PlatformIO. The default PlatformIO firmware folder has four main locations: `include/`, `lib/`, `src/`, and `test/`. Ensuring clean compilation requires aligning our directories with PlatformIO's Library Dependency Finder (LDF) rules.

## Decision
Align code modules with the standard PlatformIO directory structure:
*   `include/`: Global configuration files (`Config.h`) and HAL interface contracts (`HAL/`).
*   `lib/`: Logic modules (EventSystem, StateMachine, EmotionEngine). This allows them to be treated as individual libraries.
*   `src/`: Main setup entry point (`main.cpp`), adapters implementing the HAL (`adapters/`), and orchestrators (`managers/`).
*   Configure `-I lib/` inside the build flags in `platformio.ini` to resolve nested relative paths cleanly.

## Alternatives Considered
*   **Monolithic Src Folder:** Storing all headers and source files in `src/`. This was rejected because it reduces modularity and makes it difficult to run native unit tests on PCs.
*   **Separated Include Folder:** Placing logic libraries headers in `include/` and sources in `src/`. This was rejected because it separates class definitions from their implementations, increasing navigation complexity.

## Consequences
*   **Pros:** High modularity. The layout conforms to standard PlatformIO practices. Easier to write unit tests for modules under `lib/`.
*   **Cons:** Requires compile-time flags (`-I lib/`) to resolve nested folders without losing namespace clarity.

## Future Revisions
None planned.
