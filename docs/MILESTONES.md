# Milestones: TinyCompanion Project

This document lists the release milestones and target features for each major iteration of **TinyCompanion**.

---

## 🏁 Development Milestones

### v0.1 - Project Skeleton (Current)
*   [x] Define directory structure.
*   [x] Establish Hardware Abstraction Layer (HAL) interface contracts.
*   [x] Create Event Queue system header definitions.
*   [x] Define core business logic headers (StateMachine, EmotionEngine, AnimationEngine).
*   [x] Write project configuration templates (`Config.h`, `platformio.ini`).
*   [x] Publish basic developer guides and architecture summaries.

### v0.2 - Hardware Abstraction Layer
*   [ ] Implement concrete Uno adapters for general I/O pins (touch and LEDs).
*   [ ] Implement non-blocking ultrasonic range finder driver.
*   [ ] Integrate TM1637 driver.
*   [ ] Integrate SH1106 OLED driver using highly optimized I2C (no buffer-copy allocation where possible).
*   [ ] Interface DS1307 RTC.
*   [ ] Implement buzzer melody scheduler.

### v0.3 - Eye Rendering
*   [ ] Implement the `EyeRenderer` drawing basic shapes.
*   [ ] Write math logic for procedural pupil displacement.
*   [ ] Program blinking animations via clipping rectangles.
*   [ ] Optimize memory consumption of buffer draw calls.

### v0.4 - Behavior Engine
*   [ ] Construct the FSM logic class.
*   [ ] Map events from the Event Queue to state transitions.
*   [ ] Wire time-of-day checks to SLEEPING/WAKING states.
*   [ ] Implement state transition callbacks.

### v0.5 - Emotion Engine
*   [ ] Program the 2D Valence-Arousal coordinate system.
*   [ ] Implement decay timers based on personality profiles.
*   [ ] Map Valence-Arousal coordinates to discrete emotions (Happy, Scared, Sad, Bored, Sleepy).

### v0.6 - Sensor Integration
*   [ ] Hook up the median filter to the Ultrasonic driver.
*   [ ] Apply the exponential decay filter to the LDR.
*   [ ] Implement stateful touch debouncer.
*   [ ] Connect all filters to fire events into the circular Event Queue.

### v0.7 - Daily Routine
*   [ ] Sync time triggers to transition states.
*   [ ] Define custom behaviors matching states (e.g., active times, evening drowsiness).

### v0.8 - Optimization
*   [ ] Audit memory maps and ensure SRAM usage stays under 1.5 KB.
*   [ ] Relocate static assets and strings to Flash memory using `PROGMEM`.
*   [ ] Optimize I2C bus lines to run reliably at 400kHz.

### v0.9 - Testing
*   [ ] Build mock environments on desktop PC for logic validation.
*   [ ] Execute integration stress tests on Uno hardware.

### v1.0 - Stable Release
*   [ ] Assemble physical companion hardware enclosure.
*   [ ] Tag release `v1.0.0` with fully functional standalone features.

---

## 🚀 Future Upgrades

### v1.1 - Servo Support
*   [ ] Introduce pan/tilt head servo interfaces into the HAL.
*   [ ] Coordinate physical head movement vectors with eye looking directions.

### v1.2 - VL53L0X Support
*   [ ] Implement ToF driver adapter.
*   [ ] Swap distance calculation from ultrasonic echo to laser time-of-flight.

### v1.3 - MPU6050 Support
*   [ ] Add I2C Gyro/Accelerometer driver.
*   [ ] Fire shake and drop events to trigger dizzy or scared states.

### v2.0 - ESP32 Migration
*   [ ] Swap compiler platform to ESP32.
*   [ ] Implement HAL adapters using the ESP32 Espressif SDK / FreeRTOS.
*   [ ] Add WiFi manager and REST/WebSocket API capabilities.
