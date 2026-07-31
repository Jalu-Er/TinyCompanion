# ADR-0002: Hardware Abstraction Layer (HAL)

*   **Status:** Accepted
*   **Date:** 2026-08-01

## Context
TinyCompanion's core logic (state machine, emotion equations, animations) represents the product's primary value. Coupling this logic to specific Arduino Uno hardware pinouts or AVR registries would prevent migrating the code to a more powerful microcontroller (like ESP32) in later phases without a complete rewrite.

## Decision
Introduce a pure C++ virtual interface abstraction layer (HAL) inside `firmware/include/HAL/`. 
*   Business logic modules depend strictly on abstract interface contracts (e.g., `IOledDisplay`, `IUltrasonic`).
*   Microcontroller-specific details are handled in concrete driver classes (e.g. `ArduinoOledSH1106`) located under `src/adapters/`.
*   Direct dependencies on `<Arduino.h>` are restricted to the adapters layer and the startup entry file (`main.cpp`).

## Alternatives Considered
*   **Direct Arduino Framework Calls:** Calling `analogRead()` or `digitalWrite()` directly inside the state machine. This was rejected because it causes tight coupling and prevents native PC-based unit testing.
*   **Compile-Time Polymorphism (Templates):** Using templates to resolve driver types. This was rejected because it introduces high header complexity and worsens code readability, which is critical for portfolio review.

## Consequences
*   **Pros:** Strict portability. The code can be compiled under ESP32 or simulated on a desktop PC by swapping the concrete adapters.
*   **Cons:** AVR compilers must handle virtual method dispatch (vtables), which introduces a minor Flash overhead (around 50-100 bytes per interface).

## Future Revisions
Review performance costs on virtual calls if Flash memory consumption approaches the 32 KB ATmega328P limit.
