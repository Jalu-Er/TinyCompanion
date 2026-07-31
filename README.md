# TinyCompanion

[![Embedded Platform](https://img.shields.io/badge/Platform-PlatformIO-orange.svg)](https://platformio.org/)
[![Microcontroller](https://img.shields.io/badge/MCU-ATmega328P-blue.svg)](https://www.microchip.com/en-us/product/ATmega328P)
[![Framework](https://img.shields.io/badge/Framework-Arduino-green.svg)](https://www.arduino.cc/)

A production-quality C++ software architecture for an emotionally expressive Desktop Companion Pet. Designed originally for the 8-bit ATmega328P (Arduino Uno R3) with strict porting capacity to 32-bit platforms (ESP32) and future robotic actuators.

Repository: [https://github.com/Jalu-Er/TinyCompanion](https://github.com/Jalu-Er/TinyCompanion)

---

## 1. Project Overview
TinyCompanion is a desktop pet companion that lives beside a laptop. It has only a head (no walking or body movement in v1) and interacts with users through animated eyes on an OLED display, an RGB LED status aura, and passive buzzer melodic chirps. It changes its mood based on proximity, touch, and light variations, and exhibits daily routines synchronized with a real-time clock.

This repository serves as a **software engineering portfolio** demonstrating:
*   Strict OOP Separation of Concerns (SoC).
*   Platform independence via a Hardware Abstraction Layer (HAL).
*   Zero dynamic allocations to prevent memory leaks in micro-SRAM (2 KB limit).
*   Cooperative non-blocking task execution scheduling.

---

## 2. Project Goals
*   **Decoupled Architecture:** Business logic engines (FSM, Emotion, Personality) must remain completely independent of microcontroller registers and Arduino-specific APIs.
*   **Resource Efficiency:** Firmware must compile within 28 KB of Flash and utilize less than 1.5 KB of SRAM.
*   **Scalability:** Clear migration path to ESP32 RTOS and pan/tilt servo motors without rewriting core personality logic.

---

## 3. Hardware Requirements (Version 1)
*   **Microcontroller:** Arduino Uno R3 (ATmega328P)
*   **OLED Display:** SH1106 128x64 pixels (I2C)
*   **Clock Display:** TM1637 4-Digit Segment Display
*   **Real-Time Clock:** DS1307 RTC (I2C)
*   **Ultrasonic Sensor:** HC-SR04
*   **Touch Sensor:** TTP223 Capacitive
*   **Light Sensor:** Analog LDR Module
*   **Feedback Devices:** Passive Buzzer & 1x RGB LED (Common Cathode)

---

## 4. Planned Features
*   **Dynamic Eye Expressions:** Smooth procedural animation rendering (Blinking, Looking Around, Squinting).
*   **Russell Circumplex Emotion Engine:** 2D Valence-Arousal calculation decaying back to baseline over time.
*   **Adjustable Personality Profiles:** Swappable behavior coefficients (e.g., Excitability, Friendliness, Fear Threshold).
*   **RTC Routine Synchronization:** Sleeping, waking, yawning, and idling synchronized to daily time boundaries.
*   **Aura LED & Melody Chirps:** Color-coded RGB aura states and non-blocking multi-frequency sound effects.

---

## 5. Software Architecture
TinyCompanion employs a layered architecture:
*   **Presentation Layer:** `AnimationEngine`, `EyeRenderer`, managers for Led, Audio, and Screen.
*   **Engine Layer:** Platform-independent `StateMachine` and `EmotionEngine` that handle state transitions and Valence/Arousal scores.
*   **Infrastructure Layer:** Static `EventQueue` circular buffers and time handlers.
*   **HAL Layer:** Pure virtual C++ contracts (`IUltrasonic`, `IDisplay`, etc.).
*   **Adapter Layer:** Concrete platform wrappers utilizing the target SDK.

---

## 6. Folder Structure
The repository is structured to organize documents, firmwares, and assets cleanly:

```text
TinyCompanion/
├── docs/                          # Developer documentation
│   ├── SOFTWARE_DESIGN_DOCUMENT.md# Detailed design specifications
│   ├── ROADMAP.md                 # Development schedule
│   ├── MILESTONES.md              # Target release milestones
│   ├── CHANGELOG.md               # Version logs
│   ├── CONTRIBUTING.md            # Coding standards
│   └── ARCHITECTURE.md            # High-level design summary
├── firmware/                      # C++ source code & platform configuration
│   ├── include/                   # Global configuration and HAL headers
│   ├── src/                       # Main entry and hardware adapters
│   ├── lib/                       # Business logic libraries (Platform Independent)
│   ├── test/                      # Firmware test directories
│   ├── platformio.ini             # PlatformIO config
│   └── README.md                  # Firmware instructions
├── hardware/                      # Schematics and CAD cases (Planned)
├── assets/                        # Design media
│   ├── eyes/                      # Procedural shape drawings
│   ├── sounds/                    # Melody notes
│   ├── icons/                     # UI components
│   └── references/                # Hardware sheets
├── tools/                         # Helper scripts
├── LICENSE                        # MIT License
├── README.md                      # Project readme
└── .gitignore                     # Git tracking configuration
```

---

## 7. Build Instructions

### Prerequisites:
1.  Install [VS Code OSS](https://code.visualstudio.com/) or standard VS Code.
2.  Install the **PlatformIO IDE Extension** from the marketplace.

### Build Steps:
1.  Clone this repository:
    ```bash
    git clone https://github.com/Jalu-Er/TinyCompanion.git
    ```
2.  Open the `firmware/` folder in VS Code.
3.  PlatformIO will automatically download toolchains and libraries specified in `platformio.ini`.
4.  To compile the project:
    *   Click the **PlatformIO: Build** tick icon at the bottom status bar, or run:
    ```bash
    pio run -d firmware/
    ```
5.  To upload to an Arduino Uno:
    *   Connect the board to your USB port.
    *   Click the **PlatformIO: Upload** arrow icon, or run:
    ```bash
    pio run -d firmware/ -t upload
    ```

---

## 8. Development Status
*   **Current Phase:** Phase 1 (Project Skeleton)
*   **Stable Version:** None (Pre-release)
*   **Latest Milestones Completed:** v0.1 Project Foundation.

---

## 9. Future Roadmap
*   **Phase 2:** Implement concrete hardware adapters on Arduino Uno.
*   **Phase 3:** Create eye coordinate mathematical drawing routines.
*   **Phase 4:** Connect state triggers to time schedules and sensor boundaries.
*   **Phase 10:** Stable standalone `v1.0.0` launch.
*   **Phase 11+:** Integrate pan-tilt head servos and migrate code to ESP32 RTOS.

---

## 10. License
This project is licensed under the MIT License - see the [LICENSE](file:///home/jalue/Documents/PlatformIO/Projects/TinyCompanion/LICENSE) file for details.
