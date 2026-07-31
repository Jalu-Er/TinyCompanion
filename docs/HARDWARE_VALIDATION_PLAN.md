# Hardware Validation Plan: TinyCompanion

This document outlines the systematic test strategies and implementation plans for validating every target hardware component of **TinyCompanion** independently before any high-level business logic is deployed.

---

## 1. Hardware Validation Protocol

```
    [RGB LEDs & Buzzer] ──► [TM1637 Display] ──► [OLED SH1106]
             │                     │                   │
             ▼                     ▼                   ▼
      (Aura / Chimes)       (Numeric Debug)     (Visual Output)
             │                     │                   │
             +---------------------+-------------------+
                                   │
                                   ▼
    [DS1307 RTC] ───────► [Touch & LDR Sensors] ──► [HC-SR04 Proximity]
```

To prevent tracking down complex bugs later in development, each component must pass validation according to the specifications below.

---

## 2. Component Validation Details

### A. OLED SH1106 (128x64 I2C Graphic Display)
*   **Validation Objective:** Verify stable I2C communication, display memory offset rendering correctness, and clean non-blocking buffer write operations.
*   **Expected Behavior:** The screen initializes without visual noise, clears to pitch black, and displays test patterns (a 128x64 pixel border and two symetrical circles representing eyes) sharply.
*   **Manual Test Procedure:**
    1. Connect OLED pins to Uno I2C pins (SDA to A4, SCL to A5).
    2. Flash the OLED test firmware.
    3. Verify that the rendered border aligns exactly with the physical frame of the glass display.
*   **Success Criteria:** Outer border pixels (rows 0 and 63, columns 0 and 127) are fully illuminated. Circles are rendered symmetrically without horizontal image shifting or row shearing.
*   **Possible Failure Modes:**
    *   No screen output (I2C lockup or address mismatch).
    *   Horizontal pixel shift (common on SH1106 displays if initialized using standard SSD1306 drivers; SH1106 RAM starts at column 2).
*   **Debugging Strategy:** Run an I2C address scanner script (typical addresses are `0x3C` or `0x3D`). Verify the implementation of the driver library initialization offset configurations.

### B. TM1637 (4-Digit Segment Display)
*   **Validation Objective:** Verify 2-wire serial protocol commands, segment driver multiplexing, and digital brightness configurations.
*   **Expected Behavior:** Displays a running counter from `0000` to `9999` while pulsing the central colon indicator, adjusting brightness dynamically.
*   **Manual Test Procedure:**
    1. Connect CLK to pin 2 and DIO to pin 3.
    2. Run the segment test cycle.
    3. Observe digit integrity during fast count modifications.
*   **Success Criteria:** All segments (A through G) light up uniformly on all 4 digits without ghosting. The colon pulses at exactly 1 Hz.
*   **Possible Failure Modes:**
    *   Missing segment lines (e.g. number `8` looking like a `6` or `9`).
    *   Flickering or random segment drops due to high serial clock rates.
*   **Debugging Strategy:** Audit physical solder joints. Reduce communication speed in the serial adapter software loop.

### C. HC-SR04 (Ultrasonic Distance Sensor)
*   **Validation Objective:** Validate non-blocking echo timing trigger, interrupt/timer signal captures, and distance value conversion.
*   **Expected Behavior:** Accurately outputs target distance values between 2cm and 100cm without calling blocking APIs (like `pulseIn()`).
*   **Manual Test Procedure:**
    1. Connect TRIG to pin 6 and ECHO to pin 7.
    2. Place a flat obstacle at physical distances (e.g., 10cm, 30cm, 50cm, 80cm) measured by a physical ruler.
    3. Monitor the distance readouts printed to the serial logger.
*   **Success Criteria:** Logged readings match the physical distances with an accuracy of $\pm2\text{ cm}$. No software lockups occur when the sensor is disconnected.
*   **Possible Failure Modes:**
    *   Static readout values at `0` or `0xFFFF` (out of range/timeout).
    *   High signal jitter (measurement fluctuations > 5cm under static conditions).
    *   Task starvation (if using the blocking `pulseIn()` function).
*   **Debugging Strategy:** Swap blocking reads to timer register counter capture or pin change interrupt (PCI) flags. Implement a 5-sample median filter inside the `SensorManager` to discard outlier readings.

### D. TTP223 (Capacitive Touch Sensor)
*   **Validation Objective:** Confirm touch state transitions are read reliably, free from contact bounce or power noise.
*   **Expected Behavior:** High/Low output levels change instantly upon finger contact and release.
*   **Manual Test Procedure:**
    1. Connect touch sensor signal to pin 9.
    2. Touch and hold the sensor pad.
    3. Tap the sensor rapidly.
*   **Success Criteria:** Registers touch state changes on serial output within 50ms of contact, with zero phantom triggers when left untouched.
*   **Possible Failure Modes:**
    *   Constant HIGH state (high trace capacitance or bad grounding).
    *   High noise/rapid triggers (power supply ripples).
*   **Debugging Strategy:** Implement software debouncing (state must remain constant for >50ms to trigger a state machine event). Connect the module to a clean 3.3V/5V supply rail with bypass caps if necessary.

### E. DS1307 (Real-Time Clock)
*   **Validation Objective:** Verify DS1307 oscillator configuration, register clock increments, and battery retention backup.
*   **Expected Behavior:** Retains date and time values after power cuts, incrementing clock seconds accurately.
*   **Manual Test Procedure:**
    1. Write target time to the RTC registers.
    2. Disconnect power to the microcontroller for 60 seconds.
    3. Reconnect power and verify the time value matches the correct elapsed period.
*   **Success Criteria:** Date and time persist across power cuts. Clock increments seconds accurately without resetting to the default UNIX epoch.
*   **Possible Failure Modes:**
    *   Clock halts (the DS1307's Clock Halt [CH] bit in register 0x00 is set).
    *   Time resets to zero on power cycle (depleted CR2032 coin cell battery or missing VBAT trace connection).
*   **Debugging Strategy:** Read and verify register `0x00` to ensure the `CH` bit is cleared to zero. Measure the backup battery voltage (must be >2.7V).

### F. LDR (Light Dependent Resistor)
*   **Validation Objective:** Verify analog ADC conversions and LDR sensitivity limits under changing light conditions.
*   **Expected Behavior:** ADC values scale smoothly with brightness levels, generating high values in the dark and low values in bright light.
*   **Manual Test Procedure:**
    1. Connect LDR output to analog pin A0.
    2. Log values in a brightly lit room.
    3. Cover the sensor completely with a dark card or finger and log values again.
*   **Success Criteria:** Raw analog reads shift between a minimum range (e.g. < 200 in bright environments) and a maximum range (e.g. > 800 in complete darkness) with smooth transitions.
*   **Possible Failure Modes:**
    *   Static reads at `0` or `1023` (incorrect pull-up/pull-down resistor value or short circuit).
    *   High analog ADC ripple (power supply interference).
*   **Debugging Strategy:** Verify the voltage divider circuit (ensure a 10k Ohm resistor is wired to ground/power appropriately). Put in place a software exponential moving average filter.

### G. RGB LEDs (Emotional Aura)
*   **Validation Objective:** Validate PWM duty cycles, color mixing gradients, and common cathode pin assignments.
*   **Expected Behavior:** Emits pure red, green, and blue light sequences, blending them smoothly to form white and custom gradients.
*   **Manual Test Procedure:**
    1. Connect Red to pin 11, Green to pin 13, and Blue to pin 12.
    2. Run a cycle that sweeps duty cycles: Red -> Green -> Blue -> Mixed White -> Off.
*   **Success Criteria:** Color blending is uniform, and analog writes scale dimming linearly without flickering.
*   **Possible Failure Modes:**
    *   No output or very dim lights (incorrect resistor values limiting current).
    *   Non-linear dimming on pin 12 (Uno's pin 12 is a non-hardware PWM pin, meaning analogWrite may behave as standard digital output unless handled by software PWM or remapped to PWM pins like 3, 5, 6, 9, 10).
*   **Debugging Strategy:** If analogWrite does not work smoothly on pin 12, use a software-based PWM routine or change the pin configuration in `Config.h` to a hardware PWM pin (e.g. swap pin 12 with a free PWM pin).

### H. Passive Buzzer
*   **Validation Objective:** Validate PWM frequency generation, volume limits, and non-blocking melody step ticks.
*   **Expected Behavior:** Generates clear audio frequencies (200Hz - 2000Hz) and stops playing immediately when commanded.
*   **Manual Test Procedure:**
    1. Connect Buzzer to pin 8.
    2. Play a short octave scale non-blocking.
*   **Success Criteria:** Clear tone generation with distinct pitch differences. Tones stop immediately without residual clicking sounds.
*   **Possible Failure Modes:**
    *   Monotone beep (an active buzzer is wired instead of a passive buzzer).
    *   No sound or clicking (incorrect timer configurations).
*   **Debugging Strategy:** Verify the buzzer component type (passive buzzers require an oscillating signal to sound). Ensure the driver does not use blocking APIs (like standard `delay()`).

---

## 3. Optimal Implementation Order

We will implement the validation firmware drivers in the following phases to ensure each step builds upon a verified foundation:

### Phase 1: Basic Actuators (RGB LEDs & Passive Buzzer)
*   *Rationale:* These basic outputs do not require complex timing protocols. They provide instant visual and acoustic signals that can be used to notify developers of the status of subsequent sensor tests without needing a serial connection.

### Phase 2: Debug Output (TM1637 Segment Display)
*   *Rationale:* A 4-digit segment display provides a simple output channel to display raw sensor counts (like distance or light levels) directly on hardware, removing the constant need for a serial console interface.

### Phase 3: Primary Graphics (OLED SH1106 Display)
*   *Rationale:* This represents the main user feedback channel (the pet's eyes). Validating this early allows visual indicators and eye shape diagnostics to run during inputs testing.

### Phase 4: Time Keeping (DS1307 RTC)
*   *Rationale:* Provides a shared clock reference. Once the screens are validated, we can show real-time clocks on TM1637 and OLED directly to prove I2C stability.

### Phase 5: Simple Inputs (TTP223 Touch & LDR Light)
*   *Rationale:* Simple digital and analog reads. Easy to validate and map directly to verified outputs (e.g., LDR values altering OLED brightness, touch inputs triggering buzzer chimes).

### Phase 6: Precision Inputs (HC-SR04 Proximity)
*   *Rationale:* The most timing-sensitive component. Requires robust non-blocking interrupt logic. Validating this last prevents timing interference from affecting other simpler modules during early diagnostics.
