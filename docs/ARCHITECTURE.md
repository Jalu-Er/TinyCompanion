# Architecture Overview: TinyCompanion

This document provides a concise architectural guide to **TinyCompanion** to help developers and reviewers understand the system organization quickly without reading the entire Software Design Document (SDD).

---

## 1. Core Architectural Pillars

The design of TinyCompanion is built upon three main pillars:
1.  **Platform Independence (Separation of Concerns):** The core logic engines (StateMachine, EmotionEngine, AnimationEngine) contain only platform-agnostic business logic. They do not import `<Arduino.h>` or rely on platform-specific registers.
2.  **Hardware Abstraction Layer (HAL):** Interfaces act as strict contracts. Driver implementations (under `src/adapters/`) implement these interfaces using platform APIs (Arduino Core, I2C libraries, etc.).
3.  **Event-Driven Communication:** Systems do not call each other directly to reduce coupling. Inputs generate events that are pushed into a static circular `EventQueue`. The main controller dequeues these events and updates the engines accordingly.

---

## 2. System Layering

The code is organized into discrete layers:

```
+-----------------------------------------------------------+
| Presentation Layer (AnimationEngine, EyeRenderer)         |
+-----------------------------------------------------------+
                             │
                             ▼
+-----------------------------------------------------------+
| Engine Layer (StateMachine, EmotionEngine)                |
+-----------------------------------------------------------+
                             │
                             ▼
+-----------------------------------------------------------+
| Infrastructure Layer (EventQueue, TimeManager)            |
+-----------------------------------------------------------+
                             │
                             ▼
+-----------------------------------------------------------+
| Hardware Abstraction Layer (HAL Interfaces)               |
+-----------------------------------------------------------+
                             ▲
                             │ (Implemented by)
+-----------------------------------------------------------+
| Adapter Layer (Arduino Uno Concrete Drivers)              |
+-----------------------------------------------------------+
```

---

## 3. Memory Preservation Policy
Since the target platform is the 8-bit ATmega328P with only **2 KB of SRAM**:
*   **Static Allocation:** All classes, queues, arrays, and task structures are allocated statically.
*   **PROGMEM Storage:** Immutable configurations (like personality coefficients, breathing sine tables, and audio melody steps) are stored in Program Space (Flash Memory) rather than RAM.
*   **Buffer Minimization:** The OLED display buffer modifications are computed procedurally and written directly without extra intermediate image buffers.

---

## 4. Key Subsystems

### Event Queue (`EventQueue`)
A static, circular ring-buffer storing `Event` structs. Sensor reads enqueue events (like `EventType::TOUCH_TRIGGERED`), and the main scheduler pops them sequentially to trigger state changes.

### State Machine (`StateMachine`)
The FSM translates events into behaviors. Active states include:
*   `SLEEPING` (Deep rest, breathing LED, clock display)
*   `WAKING_UP` (Wake animation, chimes)
*   `IDLE` (Default active, eye movement, look around)
*   `INTERACTIVE` (Joy reaction, high-pitched sounds, breathing yellow)
*   `SCARED_ALERT` (Wide eyes, warning buzzer beep)

### Emotion Engine (`EmotionEngine`)
Tracks emotional coordinates on a 2D Valence-Arousal plane. Sensor triggers shift these coordinates, while a time-based tick decays them back to baseline according to the selected personality profile.

---

## 5. Main Loop Execution Model
TinyCompanion operates a **cooperative multi-tasking scheduler** in `main.cpp`. The main loop checks elapsed milliseconds and runs tasks at specified intervals without utilizing blocking delays:

*   **20 Hz (50ms):** Poll inputs (Touch, Ultrasonic, Light).
*   **10 Hz (100ms):** Process Event Queue, update State Machine, update Emotion Engine.
*   **50 Hz (20ms):** Update audio sweeps.
*   **30 Hz (33ms):** Interpolate animation frames and render to OLED.
