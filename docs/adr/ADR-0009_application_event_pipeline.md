# ADR-0009: Application Event Pipeline

*   **Status:** Accepted
*   **Date:** 2026-08-01

## Context
High-level state engines (like the FSM) and output managers (LED, Display, Audio) need to respond dynamically to environmental events. Coupling managers directly to the `SensorManager` or `EventQueue` creates structural spaghetti. We need a clean, uniform communication pipeline that delivers events to all consumers deterministically.

Furthermore, we must implement event metrics tracking (history trace, statistics counters) to assist debugging and provide data to the personality engine in the future. These mechanisms must fit within the ATmega328P's severe 2 KB SRAM limits.

## Decision
Introduce a statically allocated **Event Pipeline**:
*   All consumers implement the `IEventConsumer` abstract interface.
*   The `EventDispatcher` holds a static array of `IEventConsumer` interface pointers (capacity = 6).
*   During sensor polling, popped events are registered in `EventHistory` and `EventStatistics` and forwarded via `EventDispatcher::dispatch()`.
*   An `EventLogger` helper is added, which can be completely disabled using a compile macro to save Flash memory space.

## Alternatives Considered
*   **Dynamic Pub/Sub Callback Registration:** Utilizing `std::function` or observer list vectors. This was rejected because dynamic vectors risk memory fragmentation, and `std::function` template instantiations consume substantial Flash space on the Uno.
*   **Scattered Polling:** Each manager accessing the `EventQueue` directly. This was rejected because an event popped by one manager would become unavailable to the others, preventing multi-cast event routing.

## Consequences
*   **Pros:**
    *   **Multi-cast Support:** Every registered manager receives a copy of every event popped from the queue.
    *   **Zero Heap Allocation:** 100% deterministic static sizing.
    *   **Diagnostic Safety:** Compile-time toggle on `EventLogger` allows developers to compile with verbose serial logging or fully strip the logger code to save space.
*   **Cons:**
    *   **Linear Execution Overhead:** Dispatched callbacks execute sequentially on a single thread. Consumer overrides must remain non-blocking.

## Future Migration Path
When porting to the ESP32 (v2.0), the `EventDispatcher` can be migrated to FreeRTOS message queues or event groups, allowing consumers to run on separate threads without changing the manager interface contracts.
