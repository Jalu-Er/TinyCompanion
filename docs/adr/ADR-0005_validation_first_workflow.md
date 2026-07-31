# ADR-0005: Validation-First Development Workflow

*   **Status:** Accepted
*   **Date:** 2026-08-01

## Context
In complex embedded systems, integrating multiple hardware peripherals at the same time as high-level behavioral algorithms makes debugging extremely difficult. Pin conflicts, timing issues, or faulty wiring are easily confused with software logic bugs.

## Decision
Enforce a **Validation-First** engineering workflow:
*   Before writing application logic (e.g., eye rendering math, emotion triggers), validate each physical component independently.
*   Introduce the `IValidation` interface inside `include/validation/IValidation.h`.
*   Each test setup must be isolated in a dedicated validator module under `src/validation/` (e.g. `OledValidation`).
*   The `main.cpp` file delegates startup tests to these modules during integration testing.

## Alternatives Considered
*   **Big-Bang Integration:** Writing the entire code and testing everything at the end. This was rejected because debugging and resolving pin/timer conflicts at late stages is extremely expensive.

## Consequences
*   **Pros:** Isolates hardware bugs from software logic bugs early in development. Provides clear test manuals (`docs/testing/`).
*   **Cons:** Requires writing temporary diagnostic code and validator modules, which slightly increases initial development time.

## Future Revisions
None. This workflow is frozen for Version 1.
