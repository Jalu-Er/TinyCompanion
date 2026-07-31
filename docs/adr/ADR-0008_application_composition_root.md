# ADR-0008: Application Composition Root

*   **Status:** Accepted
*   **Date:** 2026-08-01

## Context
TinyCompanion is built using dependency injection (DI) to keep hardware drivers decoupled from business managers. However, without a single unified location to wire these dependencies, classes would end up instantiating their own dependencies or passing them down through multiple layers, leading to "dependency clutter" and tight coupling.

## Decision
Establish the `TinyCompanionApp` class as the **Composition Root** of the application.
*   All concrete adapter instantiations (e.g. `ArduinoTouch`, `ArduinoLightSensor`) and global infrastructure classes (e.g. `EventQueue`) are declared as private fields inside `TinyCompanionApp`.
*   Constructor dependency injection is resolved entirely inside the `TinyCompanionApp` member initializer list.
*   `main.cpp` is stripped of all configuration parameters and only instantiates the app container.

## Alternatives Considered
*   **Service Locator Pattern:** A global register where managers request objects (e.g. `Locator::getTouchSensor()`). This was rejected because it hides class dependencies, making the code harder to follow, and introduces static lifetime tracking issues.
*   **Scattered Initialization:** Instantiating objects inside various files and using `extern` to share them. This was rejected because it creates "spaghetti dependencies" and makes testing interfaces in isolation very difficult.

## Consequences
*   **Pros:**
    *   **Unified Wiring:** There is exactly one file (`TinyCompanionApp.cpp`) to modify when changing how pins, drivers, or queues are wired.
    *   **Portability:** Swapping the entire hardware interface (e.g. for a simulation target) only requires writing a different Composition Root class, leaving the managers unchanged.
    *   **Clear Ownership:** The application object owns the lifecycle of all drivers and managers statically.
*   **Cons:**
    *   `TinyCompanionApp.h` must include all adapter headers, making it act as a compile dependency hub.

## Future Revisions
None planned.
