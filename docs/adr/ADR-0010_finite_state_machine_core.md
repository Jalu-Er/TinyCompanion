# ADR-0010: Finite State Machine (FSM) Core Design

*   **Status:** Accepted
*   **Date:** 2026-08-01

## Context
The companion needs to transit between multiple behaviors (Idle, Sleeping, Observing, Happy, Thinking, Alert) based on external environmental events. We must keep this behavioral brain scalable to allow adding future complex rules without creating a "giant switch statement" inside a single class, while respecting the ATmega328P's strict 2 KB RAM limit.

## Decision
Implement a statically allocated C++ **State Pattern**:
*   Define `IState` abstract interface and `StateContext` context interface.
*   Define concrete state classes (e.g. `IdleState`, `SleepingState`) and compile them into `StateMachine.cpp`.
*   All concrete states are instanced statically as local variables inside `StateMachine.cpp` (occupying static memory space).
*   State transition mappings are resolved using C++ polymorphism via vtables.
*   The `StateMachine` context class resolves routing lookups using a basic factory switch inside `transitionTo()` and broadcasts `STATE_CHANGED` semantic events to notify peripheral logic modules.

## Alternatives Considered
*   **Procedural Giant Switch State Machine:** A single block checking state variables and event flags (e.g. `if (state == IDLE && event == TOUCH) { ... }`). This was rejected because it becomes unmaintainable as state flags multiply and violates the Open/Closed Principle.
*   **Table-Driven State Transition Matrix:** Store state-event-nextState records in PROGMEM flash arrays. This was rejected because it does not support executing custom timing code (e.g. happy state countdowns) inside specific transitions without messy pointer maps.

## Consequences
*   **Pros:**
    *   **High Cohesion:** Logika perilaku untuk state tertentu (misal: happy timing limit) tersimpan terisolasi di dalam kelas state itu sendiri.
    *   **No Dynamic Allocations:** 100% aman dari resiko kebocoran heap (*heap leak*) karena semua objek statis instanced di compile-time.
    *   **Open/Closed:** Menambahkan state baru hanya butuh kelas baru yang mewarisi `IState` dan mendaftarkannya di routing context.
*   **Cons:**
    *   **Vtable Overhead:** C++ virtual methods menghasilkan tabel virtual (~8-12 bytes per kelas di RAM/Flash). Namun, ini sangat kecil dan masih sangat aman bagi sisa anggaran RAM Uno ($1.3$ KB terpakai dari $2$ KB).

## Future Migration Path
When porting to the ESP32 (v2.0), this State Pattern remains fully portable without modifications, and states can be compiled to use native dual-core execution schedules or thread-safe message queues.
