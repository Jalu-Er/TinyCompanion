# Hardware Validation Test Manual: RGB LED Aura

This document describes the validation setup, test procedure, and limitations for the RGB common cathode LED module used in **TinyCompanion**.

---

## 1. Objective
Verify that the RGB LED module is correctly wired and can transition between all predefined ambient `AuraState` settings (Idle, Happy, Thinking, Sleeping, Alert, Error) utilizing non-blocking software blinking control.

---

## 2. Wiring Configuration
Connect the common cathode RGB LED module to the Arduino Uno R3 using the global pin definitions in `Config.h`:

| LED Pin Name | Uno R3 Target Pin | Hardware Capability | Description |
| :--- | :---: | :---: | :--- |
| **R** (Red) | 11 | PWM-capable | Red color channel input |
| **G** (Green) | 13 | Digital-only | Green color channel input |
| **B** (Blue) | 12 | Digital-only | Blue color channel input |
| **Cathode** (GND) | GND | - | Common ground line |

*Important:* Make sure to place inline resistors (e.g. $220\Omega$ to $330\Omega$) on the Red, Green, and Blue pin lines to protect the LED channels and prevent excess current draw from the ATmega328P pins.

---

## 3. Validation Procedure
1. Wire the RGB LED to the Uno board as shown above.
2. Compile and upload the validation firmware using PlatformIO:
   ```bash
   pio run -t upload
   ```
3. Observe the LED behavior without looking at the Serial Monitor. The LED must transition through the following states automatically:
   *   **Phase 1 (Idle):** Solid green for 3 seconds.
   *   **Phase 2 (Happy):** Cyan (Green + Blue) slow blinking (800ms) for 4 seconds.
   *   **Phase 3 (Thinking):** Blue medium blinking (300ms) for 4 seconds.
   *   **Phase 4 (Sleeping):** Blue very slow breathing simulation blinking (2000ms) for 6 seconds.
   *   **Phase 5 (Alert):** Yellow (Red + Green) fast warning blinking (150ms) for 4 seconds.
   *   **Phase 6 (Error):** Red very fast panic alarm blinking (80ms) for 4 seconds.
   *   **Phase 7 (Finished):** The LED turns off completely.

---

## 4. Expected Output Logging
Verify the progression status via the Serial Monitor at `115200` baud:
```text
System booting...
Starting RGB LED Aura validation sequence...
Displaying AuraState [Idle]...
Displaying AuraState [Happy]...
Displaying AuraState [Thinking]...
Displaying AuraState [Sleeping]...
Displaying AuraState [Alert]...
Displaying AuraState [Error]...
RGB LED Aura validation completed successfully.
```

---

## 5. Limitations

### A. Non-PWM Blue LED Allocation
The Blue channel is connected to Pin 12, which does not support hardware PWM on the ATmega328P. As a consequence, smooth fading transitions or breathing pulses on the Blue channel cannot be generated using simple hardware `analogWrite()`.

### B. Solid Color Blinking Only
Because we prioritize non-blocking scheduling and must work around Pin 12's non-PWM status, the Aura subsystem does not use PWM intensity fading. Colors are toggled strictly ON/OFF.

---

## 6. Future PWM Enhancements
*   **Pin Reallocation:** In later updates, Pin 12 (Blue LED) can be swapped with a PWM-capable pin (such as Pin 9 or Pin 10, which are currently allocated to touch or buzzer targets) if smooth analog color fades are required.
*   **Software PWM:** Alternatively, a high-frequency timer-interrupt routine can be written to handle Software PWM on Pin 12, resolving the hardware fading issue without reallocating pins.
