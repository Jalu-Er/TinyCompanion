# Roadmap: TinyCompanion Project

This document outlines the long-term roadmap and phases for the **TinyCompanion** project. The target is to build a production-quality, low-power, emotionally expressive desktop pet.

---

## Visi Jangka Panjang (Long-Term Vision)
TinyCompanion bertujuan untuk menghadirkan teman meja kerja digital yang mandiri, ekspresif, dan responsif. Desain perangkat lunak dipisahkan secara ketat dari platform mikrokontroler sehingga dapat terus berevolusi dari papan pengembangan 8-bit (AVR/Arduino Uno) ke papan pengembangan 32-bit (ESP32) dengan konektivitas IoT secara mulus.

---

## Tahapan Pengembangan (Development Phases)

### Phase 1: Project Skeleton (Current)
*   **Goal:** Establish the complete project layout, configuration structure, HAL interfaces, and directory tree.
*   **Deliverables:** PlatformIO configuration, empty C++ class declarations, and comprehensive developer documentation.

### Phase 2: Hardware Abstraction Layer (HAL)
*   **Goal:** Define and implement concrete hardware drivers wrapping the Arduino Uno R3 SDK for all peripherals.
*   **Deliverables:** Adapters for SH1106 OLED, TM1637, HC-SR04, TTP223, LDR, DS1307 RTC, passive buzzer, and RGB LEDs.

### Phase 3: Animation Engine
*   **Goal:** Develop the procedural rendering mathematical model for eye shapes.
*   **Deliverables:** Non-blocking Fixed-Point linear interpolator (`lerp`), blink triggers, eye saccade generators, and geometry rendering on OLED.

### Phase 4: Behavior Engine
*   **Goal:** Construct the Finite State Machine (FSM) to orchestrate operational modes.
*   **Deliverables:** Transition logic between `SLEEPING`, `WAKING_UP`, `IDLE`, `INTERACTIVE`, and `SCARED_ALERT` states.

### Phase 5: Emotion Engine
*   **Goal:** Implement the 2D Valence-Arousal dynamic model.
*   **Deliverables:** Real-time decay computations, emotional coefficient shifts, and state mapping algorithms.

### Phase 6: Sensor Integration
*   **Goal:** Hook up the physical inputs to the scheduler and trigger the event broker.
*   **Deliverables:** Exponential filters for LDR, median filter arrays for Ultrasonic distance tracking, and debouncers for touch sensor inputs.

### Phase 7: Interaction System
*   **Goal:** Harmonize the inputs and outputs to create dynamic companion responses.
*   **Deliverables:** Coordination of buzzer melodies, LED auras, and OLED eye shapes responding to environment events.

### Phase 8: Optimization
*   **Goal:** Minimize resource footprint on ATmega328P.
*   **Deliverables:** Relocate string constants to Flash (`PROGMEM`), optimize shared I2C bus speeds, and evaluate static stack limits.

### Phase 9: Testing
*   **Goal:** Execute modular tests and verify behavioral flows.
*   **Deliverables:** Desktop mocking tests for logic engines and hardware tests validating interface stability.

### Phase 10: Release v1.0
*   **Goal:** Final compilation, assembly, and deployment of a fully functioning TinyCompanion.
*   **Deliverables:** Release tag `v1.0.0` with stable builds.

---

## Rencana Pengembangan Masa Depan (Future Extensions)

*   **Servo Support (v1.1):** Introduce pan/tilt servos (SG90) to enable head gestures, linking head alignment to the eye rendering targets.
*   **VL53L0X Time-of-Flight (v1.2):** Upgrade the ultrasonic sensor to a high-precision laser rangefinder for smaller enclosures.
*   **MPU6050 Accelerometer/Gyro (v1.3):** Add vibration and tilt detection to allow the companion to feel being picked up or shaken.
*   **ESP32 Migration (v2.0):** Port the platform to ESP32 to support FreeRTOS, WiFi integration (remote feed/interaction), and Bluetooth diagnostics.
