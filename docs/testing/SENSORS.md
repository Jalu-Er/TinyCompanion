# Hardware Validation Test Manual: Sensor Event Foundation

This document describes the validation setup, pin connections, thresholds, and execution procedures for testing the **Sensor Event Foundation** in **TinyCompanion**.

---

## 1. Objective
Verify that raw values from LDR, Touch (TTP223), Ultrasonic (HC-SR04), and RTC (DS1307) sensors are processed through debouncers, edge detectors, and hysteresis filters, and successfully converted into clean, decoupled semantic events (`TOUCH_PRESSED`, `USER_APPROACHING`, `AMBIENT_DARK`, etc.) published inside the `EventQueue`.

---

## 2. Wiring Configuration
Connect all enabled sensors to the Arduino Uno R3 as defined in `Config.h`:

### A. TTP223 Capacitive Touch Sensor
| Sensor Pin | Uno Pin | Description |
| :--- | :---: | :--- |
| **VCC** | 5V | Power Input |
| **GND** | GND | Ground |
| **SIG** (Signal) | 9 | Touch active-high signal line |

### B. Analog LDR (Light Dependent Resistor)
| Sensor Pin | Uno Pin | Description |
| :--- | :---: | :--- |
| **VCC** | 5V | Power Input |
| **GND** | GND | Ground |
| **OUT** (Analog) | A0 | Analog voltage level (0-1023) |

### C. HC-SR04 Ultrasonic Distance Sensor
| Sensor Pin | Uno Pin | Description |
| :--- | :---: | :--- |
| **VCC** | 5V | Power Input |
| **GND** | GND | Ground |
| **Trig** | 6 | Trigger output pulse line |
| **Echo** | 7 | Echo input timing line |

### D. DS1307 Real-Time Clock
| Sensor Pin | Uno Pin | Description |
| :--- | :---: | :--- |
| **VCC** | 5V | Power Input |
| **GND** | GND | Ground |
| **SDA** | A4 | I2C Serial Data |
| **SCL** | A5 | I2C Serial Clock |

---

## 3. Thresholds & Filter Choices

### A. LDR Light Hysteresis
*   **Default Target Threshold:** 750 (ADC values above this indicate darkness).
*   **Hysteresis Band:** 20 ADC steps.
*   **State Transitions:**
    *   `AMBIENT_DARK` is emitted when LDR reading rises above $750 + 20 = 770$.
    *   `AMBIENT_BRIGHT` is emitted when LDR reading drops below $750 - 20 = 730$.
*   *Justification:* Prevents oscillating event spam when ambient lighting hovers near 750.

### B. Proximity Zone (Distance) Hysteresis
*   **Default Target Limit:** 15 cm.
*   **Hysteresis Band:** 3 cm.
*   **State Transitions:**
    *   `USER_APPROACHING` is emitted when a range read is $\le 15\text{ cm}$.
    *   `USER_LEAVING` is emitted when a range read exceeds $15 + 3 = 18\text{ cm}$.
*   *Justification:* Prevents rapid approaching/leaving toggle events when a user stands exactly at 15 cm from the companion.

### C. RTC Period Boundaries
Time periods are split into 4 coarse semantic chunks:
*   **Morning:** 07:00 to 11:59 (Triggers `TIME_PERIOD_MORNING`)
*   **Afternoon:** 12:00 to 16:59 (Triggers `TIME_PERIOD_AFTERNOON`)
*   **Evening:** 17:00 to 21:59 (Triggers `TIME_PERIOD_EVENING`)
*   **Night:** 22:00 to 06:59 (Triggers `TIME_PERIOD_NIGHT`)
*   *Justification:* Transition events are triggered only once upon hour boundary crossing, preventing clock polling noise.

---

## 4. Validation Procedure
1. Verify the wiring layout.
2. Compile and upload the firmware target:
   ```bash
   pio run -t upload
   ```
3. Open the Serial Monitor at `115200` baud.
4. If no physical DS1307 hardware is found, the system outputs:
   `[RTC] Hardware DS1307 not found. Activating virtual time simulation.`
   And simulated time will cycle (1 virtual hour every 2 seconds). Watch the events:
   `[EVENT DETECTED] Type: TIME_PERIOD_MORNING`, `TIME_PERIOD_AFTERNOON`, etc.
5. Touch the TTP223 contact pad. The console must print:
   `[EVENT DETECTED] Type: TOUCH_PRESSED`
   Hold it down; no other events should emit. Release it:
   `[EVENT DETECTED] Type: TOUCH_RELEASED`
6. Cover the LDR module. The console prints `AMBIENT_DARK`. Expose it back to light to trigger `AMBIENT_BRIGHT`.
7. Move an object close to the HC-SR04 sensor ($< 15\text{ cm}$) to trigger `USER_APPROACHING`. Move it away ($> 18\text{ cm}$) to trigger `USER_LEAVING`.

---

## 5. Known Limitations
*   **PulseIn Blocking:** Proximity measurement relies on Arduino's `pulseIn()`. This blocks the CPU execution for up to 20 ms. While safe under the current cooperative timing mapping, we have logged this as technical debt to be resolved using non-blocking capture timers.
*   **Virtual Clock Accelerator:** The virtual RTC simulation runs at 1 hour per 2 seconds. Do not keep the simulation running in final production releases.
