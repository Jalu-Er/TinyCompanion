# Hardware Validation Test Manual: OLED SH1106

This document describes the validation setup, test procedure, and troubleshooting guidelines for the SH1106 128x64 I2C OLED display module used in **TinyCompanion**.

---

## 1. Objective
Verify that the SH1106 OLED display initializes correctly over the I2C bus and is capable of drawing text, geometric primitives (lines, rectangles, circles), individual pixels, and custom binary bitmaps without memory layout shifts or visible flickering.

---

## 2. Wiring Configuration
Connect the OLED module to the Arduino Uno R3 using the standard hardware I2C pinouts:

| OLED Pin Name | Uno R3 Target Pin | Cable Color (Recommended) | Description |
| :--- | :--- | :--- | :--- |
| **VCC** | +5V or +3.3V | Red | Power Input |
| **GND** | GND | Black | Ground |
| **SDA** | A4 (or dedicated SDA pin) | Green / Blue | Serial Data Line |
| **SCL** | A5 (or dedicated SCL pin) | Yellow | Serial Clock Line |

---

## 3. Test Procedure
1. Configure and wiring the OLED display to the Uno board as shown above.
2. Compile and upload the validation firmware using PlatformIO:
   ```bash
   pio run -t upload
   ```
3. Watch the display sequence:
   * **Stage 1 (Startup):** Screen remains dark for 500ms, then clears.
   * **Stage 2 (Text):** Text `TinyCompanion` renders on row 15, and `OLED Validation` renders on row 35. This text persists for 2 seconds.
   * **Stage 3 (Primitives & Pixels):**
     * A boundary border outline is drawn around the 128x64 pixel workspace.
     * Horizontal lines are drawn at row 5 and row 58.
     * A hollow circle (left eye mockup) is drawn on the left.
     * A solid filled circle (right eye mockup) is drawn on the right.
     * A hollow rectangle is drawn in the center, with a single pixel cleared directly in its center (verifying pixel addressing).
   * **Stage 4 (Bitmap):** An 8x8 checkmark icon bitmap is drawn at the bottom center.
4. Open the Serial Monitor at `115200` baud rate to verify that the start and end validation flags are printed:
   ```text
   Starting OLED validation sequence...
   OLED validation patterns rendered successfully.
   ```

---

## 4. Expected Output Visual Layout

```
+------------------------------------------------------------+
|  ________________________________________________________  |  <- Top line
| /                                                        \ |
| |        ___                                             | |
| |      /     \                     #######               | |
| |     |   *   |      [Rect]        #######               | |  <- Eyes: Hollow left, Filled right
| |      \ ___ /                     #######               | |
| \________________________________________________________/ |
|                            \/                              |  <- Checkmark icon (bottom)
+------------------------------------------------------------+
```

---

## 5. Definition of Done
The test is considered successful only if:
* [x] The display registers clear cleanly on boot.
* [x] Text displays clearly without characters clipping.
* [x] All shapes (lines, rectangles, circles) render without distortion.
* [x] The screen boundary border wraps exactly around the visual edges of the screen (indicating no horizontal SH1106 memory offsets).
* [x] The checkmark bitmap renders without distortion.
* [x] Zero visible flickering occurs during frame updates.

---

## 6. Known Limitations
* **AVR RAM Consumption:** The Adafruit SH110X library allocates a full 1024-byte screen buffer in RAM. On the ATmega328P, this represents 50% of the entire RAM. While safe for this validation, Phase 8 (Optimization) will evaluate custom procedural page-buffer drawing routines if memory constraints conflict with other modules in future updates.

---

## 7. Debugging Checklist
* **Screen remains completely black:**
  * Verify wiring continuity of VCC and GND. Measure voltage at display pins (must be $>3.0\text{V}$).
  * Verify SDA and SCL are not swapped.
  * Check the display's I2C address. Run an I2C scanner sketch. The default address used is `0x3C`. If yours is `0x3D`, change it in `src/adapters/ArduinoOledSH1106.cpp`.
* **Visual snow or noisy output:**
  * Clean wire routing to avoid electromagnetic interference from high-frequency lines.
* **Image shifted horizontally (or garbage pixels on left margin):**
  * This indicates that an SSD1306 driver is being used. Ensure you have clean PlatformIO libraries targeting `Adafruit SH110X` and that `ArduinoOledSH1106.cpp` instantiates `Adafruit_SH1106G`.
* **Compilation errors:**
  * Run `pio lib update` to ensure modern Adafruit BusIO and GFX dependencies are matched.
