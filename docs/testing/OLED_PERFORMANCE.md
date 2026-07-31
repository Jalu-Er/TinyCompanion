# OLED Rendering and Performance Evaluation Report

This document presents the methodology, actual hardware measurements, observations, and design recommendations gathered during the **OLED Rendering Evaluation Milestone (v0.2.1a)** for **TinyCompanion**.

---

## 1. Benchmark Methodology

The benchmark was executed on the target **Arduino Uno R3 (ATmega328P @ 16MHz)** connected to a **1.3" SH1106 I2C OLED display**. The compiler flags in PlatformIO were configured with `-std=c++14`, `-Wall`, `-Wextra`, and optimization set to release mode.

### Execution Loop:
Each benchmark screen is rendered in sequence. The CPU measures timing using `micros()` directly before and after operations:
1.  **Draw Time ($T_{draw}$):** The time spent by the CPU calculating geometry positions, plotting bytes, and modifying the local RAM buffer (between `oled.clear()` and `oled.display()`).
2.  **Transfer Time ($T_{transfer}$):** The time spent pushing the 1024-byte display buffer over the I2C bus at **400 kHz** (measured within the `oled.display()` execution).
3.  **Total Cycle Time ($T_{total}$):** $T_{draw} + $T_{transfer}$.
4.  **Theoretical FPS:** Maximum achievable frame rate if the CPU only executed that specific rendering cycle continuously:

$$\text{Theoretical FPS} = \frac{1,000,000}{T_{total} \text{ (in us)}}$$

---

## 2. Hardware Measurements (AVR @ 16MHz, I2C @ 400kHz)

The table below summarizes the actual performance metrics collected from the physical Arduino Uno board via serial logging:

| Benchmark Screen | Draw Time ($T_{draw}$) | Transfer Time ($T_{transfer}$) | Total Cycle Time ($T_{total}$) | Theoretical FPS |
| :--- | :---: | :---: | :---: | :---: |
| **100 Random Pixels** | 19,964 us | 39,116 us | 59,080 us | 16.93 FPS |
| **20 Geometric Lines** | 25,632 us | 39,140 us | 64,772 us | 15.44 FPS |
| **10 Rectangles (Hollow/Filled)** | 31,084 us | 39,132 us | 70,216 us | 14.24 FPS |
| **10 Circles (Hollow/Filled)** | 42,584 us | 39,104 us | 81,688 us | 12.24 FPS |
| **10 Checkmark Bitmaps (8x8)** | 3,532 us | 39,140 us | 42,672 us | 23.43 FPS |
| **3 Lines of Static Text (~60 chars)**| 19,736 us | 39,120 us | 58,856 us | 16.99 FPS |

---

## 3. Engineering Observations

### A. The I2C Transfer Bottleneck
The most critical finding is the **transfer time constancy**. Across all benchmarks, the I2C transfer time remains locked at **~39.1 ms**. 
*   **Why?** The `Adafruit_SH110X` library employs a full frame buffer of $128 \times 64 / 8 = 1024$ bytes. Every call to `display()` transmits the entire buffer over I2C.
*   **Impact:** Even if the drawing time were reduced to **0 us**, the absolute hardware rendering ceiling on Arduino Uno R3 over I2C at 400kHz is **25.5 FPS**. 

### B. Primitives Rendering Cost
*   **Pixels and Bitmaps:** Pixel drawing is relatively fast (~200 us per pixel). Drawing 8x8 bitmaps is extremely efficient (~350 us per bitmap) because it maps sequential bytes directly to buffer page rows without complex trig/coordinate conversions.
*   **Circles and Lines:** Drawing circles is the most CPU-intensive operation (~4.25 ms per circle). This is due to the repetitive integer arithmetic in Bresenham's circle algorithm combined with the 16MHz clock limit of the 8-bit AVR CPU.

### C. Visible Flickering and Rendering Smoothness
*   **Flicker:** Because `Adafruit_SH110X` uses a double-buffered layout (draws to RAM buffer first, then pushes to display), **there is zero visible flickering** during shape updates.
*   **Smoothness:** Since total cycle times for complex scenes (e.g. circles) exceed 80 ms, rendering smooth 30 FPS animations is physically impossible on this hardware configuration. However, a stable cooperative refresh rate of **15 FPS** is achievable and sufficient for emotional eye expressions.

---

## 4. Design Review: Adafruit GFX vs. U8g2

We evaluated whether the project should stick with **Adafruit GFX** or switch to **U8g2**.

### Comparison Matrix:

| Metric | Adafruit GFX (Adafruit_SH110X) | U8g2 (Page Buffer Mode) |
| :--- | :--- | :--- |
| **SRAM Footprint** | **High** (1024 bytes buffer + class overhead) | **Low** (128 bytes page buffer + class overhead) |
| **Flash Footprint** | **Low** (~7 KB code overhead) | **High** (~12-18 KB depending on font arrays) |
| **CPU Load** | **Low** (Draws shapes once per refresh cycle) | **Extremely High** (Re-draws shapes 8 times per frame) |
| **API Complexity**| Simple, standard coordinate commands | Picture Loop structure `firstPage()/nextPage()` |

### Technical Analysis:
1.  **RAM Savings vs. CPU Overhead:**
    `U8g2`'s Page Buffer mode is tempting because it saves **896 bytes of SRAM**. However, Page Buffer mode forces the application to execute the draw logic **8 times** to render a single frame. 
    If a circle rendering scene takes 42 ms in GFX, executing it 8 times in U8g2 would take $42 \text{ ms} \times 8 = 336 \text{ ms}$! This would drop the frame rate to **under 3 FPS** and completely choke the cooperative task scheduler, causing touch and distance sensors to miss events.
2.  **Code Maintenance:**
    GFX permits a linear, readable rendering function. U8g2 page loops require structured nesting that complicates state checking and logic splits in multi-tasking setups.

### Recommendation:
**Maintain Adafruit GFX (with Adafruit_SH110X) for Version 1 (Uno R3).**
*   **Reasoning:** Minimizing CPU load is crucial to keep the main task runner cooperative and responsive. The 1024 bytes SRAM budget is manageable if we strictly enforce zero dynamic allocations and pack all read-only tables into Flash memory (`PROGMEM`).
*   **Future Path:** When migrating to ESP32 (v2.0), the 1024-byte RAM limit becomes negligible (ESP32 has 520 KB SRAM). The Adafruit GFX API is highly compatible with ESP32-optimized display drivers (like TFT_eSPI or ESP32-SH1106 wrappers), facilitating an easy porting path.

---

## 5. Optimization Strategy for the Eye Renderer

To achieve smooth visual expressions within the 25 FPS ceiling, the future **Eye Renderer** must implement the following optimizations:

1.  **Zero Floating-Point Math:** Easing curves and pupil scaling interpolations must use fixed-point arithmetic or integer shifts (`<<`, `>>`).
2.  **Procedural Clipping Shapes:** Eyelid blinks should not be rendered via complex curves. Instead, draw filled circles for eyes, then draw horizontal filled black rectangles (`fillRect`) over them to clip the eyelids. This leverages the extremely fast rectangle-drawing routines.
3.  **Low-Frequency Updates:** The animation tick scheduler can be set to **15 Hz or 20 Hz** instead of 30 Hz. This guarantees the CPU has at least 30-50 ms of free time in every loop to service sensor polling and FSM logic.
