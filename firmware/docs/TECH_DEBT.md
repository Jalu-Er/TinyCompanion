# Technical Debt Registry: TinyCompanion

This document registers and tracks the identified technical debt in **TinyCompanion**. Keeping tabs on technical debt ensures that we balance fast prototyping with long-term system stability.

---

## 1. High Priority Debt

### AVR SRAM Display Buffer Overhead
*   **Why it exists:** The Adafruit SH110X display driver allocates a full $128 \times 64$ bits (1024 bytes) memory buffer in the global SRAM scope.
*   **Impact:** Consumes **50% of the entire Uno RAM (2 KB)**. This leaves only 1024 bytes for the operating stack, FSM variables, event queues, and sensor filters, which is extremely tight.
*   **Proposed Resolution:** Strictly limit stack allocations in all other systems (no local large buffers). Pack all read-only string constants and tables into Flash using `PROGMEM`. In Phase 8 (Optimization), evaluate procedural page-buffer rendering loops if memory collisions occur.
*   **Priority:** High

---

## 2. Medium Priority Debt

### Non-Preemptive Cooperative Scheduler Limitations
*   **Why it exists:** The `Scheduler` class runs a single-threaded cooperative execution loop without preemptive task slices or hardware timer interrupts.
*   **Impact:** Any blocking task callback (e.g. ultrasonic read taking 20 ms, I2C refresh taking 39 ms) shifts the execution start of all subsequent periodic tasks (timing drift / jitter).
*   **Proposed Resolution:** Strictly audit task execution times. During migration to the ESP32 (v2.0), migrate this scheduler logic to native FreeRTOS task tasks mapping to hardware-controlled CPU cores.
*   **Priority:** Medium

### Ultrasonic PulseIn Blocking
*   **Why it exists:** The `ArduinoUltrasonic::getDistanceCm()` driver uses the standard Arduino blocking `pulseIn()` to capture the HC-SR04 echo pulse duration.
*   **Impact:** Blocks CPU execution for up to **20 ms** on each read (dependent on object distance). This disrupts the cooperative task runner timing and could cause touch sensor event misses.
*   **Proposed Resolution:** In Phase 8 (Optimization), rewrite the ultrasonic driver using Pin Change Interrupts (PCI) and a free hardware timer to measure echo durations asynchronously.
*   **Priority:** Medium

### I2C Serial Wire Bus Blocking
*   **Why it exists:** The standard Arduino `Wire` library uses blocking operations. Calling `display()` blocks the CPU execution for **39.1 ms** while transferring data over I2C.
*   **Impact:** During this 39 ms period, the CPU cannot poll other sensors or update the FSM. This makes it impossible to achieve a clean 30 FPS update rate and might cause touch sensor tap misses.
*   **Proposed Resolution:** Remap display updates to a lower frequency (e.g. 15 Hz) or write custom non-blocking I2C transfer adapters using AVR registers directly.
*   **Priority:** Medium

### RGB Led Pin 12 Non-PWM Allocation
*   **Why it exists:** The hardware pin map assigns the RGB LED Blue channel to Pin 12. On the Arduino Uno R3, Pin 12 is a digital-only pin and does not support hardware PWM (analogWrite).
*   **Impact:** The LedManager cannot fade the Blue channel smoothly using hardware PWM, resulting in abrupt color step jumps for any color mixtures containing blue.
*   **Proposed Resolution:** In Phase 2, swap Pin 12 with a free hardware PWM pin (e.g. swap with one of the buzzer or servo pins if they can run on digital-only or are timer-compatible), or write a lightweight software PWM driver.
*   **Priority:** Medium

---

## 3. Low Priority Debt

### Scheduler Time Resolution Precision
*   **Why it exists:** The task manager uses `millis()` tracking, which has a coarse resolution of 1 ms.
*   **Impact:** Cannot schedule sub-millisecond high-frequency operations (e.g. audio waveforms or fast encoder updates).
*   **Proposed Resolution:** Keep tasks strictly below 100 Hz, or rewrite execution markers to use `micros()`.
*   **Priority:** Low

### Virtual Call Table (vtable) Footprint
*   **Why it exists:** The strict Hardware Abstraction Layer (HAL) architecture relies on virtual method calls (`virtual ... = 0;`).
*   **Impact:** The AVR GCC compiler generates virtual tables (vtables) in Flash and RAM, adding a minor binary overhead (approx. 50-100 bytes per class).
*   **Proposed Resolution:** Keep interface counts minimal and clean. Do not split interfaces unnecessarily unless there is a clear porting benefit.
*   **Priority:** Low
