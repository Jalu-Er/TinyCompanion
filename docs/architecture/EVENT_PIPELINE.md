# Application Event Pipeline Architecture

This document describes the design, execution flow, diagnostic traces, statistics tracking, and future command structure of the **TinyCompanion** event communication framework.

---

## 1. Event Pipeline Lifecycle

The event flow operates deterministically under a cooperative cycle, avoiding any preemptive task interrupts or memory heap management.

```
 [ Hardware Sensor ]
         │ (Periodically Polled at 20 Hz)
         ▼
  [ SensorManager ]
         │ (Debounces & checks hysteresis)
         ▼
  [ Event Queue ]  ───► (FIFO ring buffer capacity = 8)
         │
 ┌───────┴───────┐ (Dequeued by poll task)
 ▼               ▼
[EventHistory]  [EventStatistics]  ───► [EventLogger (USB Console)]
                 │
                 ▼
          [EventDispatcher]
                 │
       ┌─────────┼─────────┐ (Deterministic dispatch)
       ▼         ▼         ▼
  [LED Mgr]  [OLED Mgr] [Audio Mgr] ───► [Future FSM]
```

1.  **Generation:** Concrete adapters read physical pins. `SensorManager` filters variations and pushes a new `Event` struct to the circular static `EventQueue`.
2.  **Telemetry Recording:** Inside the task runner callback, events are popped from the queue. The system automatically updates counters in `EventStatistics` and stores records in `EventHistory`.
3.  **Trace Log:** If compilation flags allow, `EventLogger` writes a semantic output log (such as `[4300 ms] EVENT UserDetected`) to USB Serial.
4.  **Distribution:** The `EventDispatcher` iterates through registered `IEventConsumer` interface pointers and calls `onEvent()` sequentially.

---

## 2. Command Architecture (Future Outlook)

It is critical to distinguish between event communication and system commands:

```
    Event              Decision             Command          Hardware Action
 (What happened)   (What should we do)  (Request to act)    (Actual pin shift)

[TOUCH_PRESSED]  ──► [EmotionEngine]  ──► [PLAY_CHIRP]  ──► [ArduinoBuzzer]
```

*   **Event (Semantic Input):** Represents a change in the environment that has already occurred (e.g. `TOUCH_PRESSED`, `USER_APPROACHING`). It is broadcasted as read-only telemetry.
*   **Decision (Business Logic):** The future `EmotionEngine` evaluates Valence, Arousal, and current FSM states. It decides what reaction is needed.
*   **Command (Request for Action):** An explicit, direct directive target issued to a controller manager (e.g., `PlayMelodyCmd`, `ChangeEyeAnimationCmd`).
*   **Hardware Action (Physical Output):** Concrete driver wrappers map the command parameter to write actions (e.g., `tone()`, `digitalWrite()`).

**Why business logic must never directly call hardware drivers:**
If the FSM or mood formulas invoked `digitalWrite()` or `tone()` directly:
1.  We could not mock drivers for unit tests without target boards.
2.  Porting to ESP32 or a desktop emulator would require rewriting the core behavioral code.
3.  We would have zero coordination over shared pin states, leading to signal conflicts.

---

## 3. Structural Design

### EventHistory circular buffer
Maintains a sliding trace of the 8 most recent events, overwriting the oldest entries automatically when full to prevent memory leaks or bounds overflows. It uses index offsets relative to `head` for lookup queries.

### EventStatistics counters
Maintains an array of `uint16_t` counters indexed directly by the `EventType` cast index. It tracks occurrences indefinitely, providing a baseline metric for the personality engine to determine "how active" the environment is.
