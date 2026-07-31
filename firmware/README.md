# TinyCompanion Firmware

This directory contains the firmware source code, libraries, and tests for **TinyCompanion**, configured for VS Code and PlatformIO.

---

## Folder Architecture

*   `include/` - Pure C++ global headers, pin mappings, and Hardware Abstraction Layer (HAL) contracts.
*   `lib/` - Hardware-independent business logic engines (FSM, Emotion System, Animation maths, Event broker).
*   `src/` - Entry point (`main.cpp`), high-level manager classes, and concrete platform hardware adapters.
*   `test/` - Unit tests for testing engines on desktop hosts.

---

## Build Instructions

To build the firmware, open this directory in VS Code with PlatformIO installed and click the Build (check mark) button, or execute:
```bash
pio run
```

To upload to an Arduino Uno:
```bash
pio run --target upload
```
