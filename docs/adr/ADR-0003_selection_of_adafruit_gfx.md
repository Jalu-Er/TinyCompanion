# ADR-0003: Selection of Adafruit GFX

*   **Status:** Accepted
*   **Date:** 2026-08-01

## Context
TinyCompanion requires a graphic display library to render eye expressions on the SH1106 OLED display. The library must run on the resource-constrained ATmega328P (2 KB RAM) and support basic drawing primitives (circles, rectangles, lines), text rendering, and bitmaps.

## Decision
Retain `Adafruit GFX` (via `Adafruit_SH110X`) as the primary graphics driver library.
*   The library uses a full frame-buffer model (1024 bytes of SRAM) that is simple to program and offers fast drawing operations in memory.
*   To keep the system responsive, the visual update rate is limited to 15-20 Hz, giving the CPU sufficient time to poll other sensors.

## Alternatives Considered
*   **U8g2 (Page Buffer Mode):** U8g2 allows allocating only 128 bytes of RAM (saving 896 bytes). However, Page Buffer Mode requires drawing the screen 8 times per frame (picture loop). On a 16MHz AVR CPU, calculating complex procedural eye animations 8 times would choke processor bandwidth, degrading overall system FPS.
*   **Custom Micro-Driver:** Writing a custom I2C graphics layout from scratch. This was rejected to keep the codebase maintainable and focus engineering hours on behavior systems rather than writing core display driver controllers.

## Consequences
*   **Pros:** Straightforward C++ drawing coordinates. High rendering smoothness with zero flickering (double-buffered in RAM).
*   **Cons:** Consumes 50% of the Uno's SRAM (1 KB buffer). This requires strict RAM management in other software modules (e.g. no dynamic arrays).

## Future Revisions
*   If RAM exhaustion occurs, optimize GFX usage or re-evaluate lightweight page buffers.
*   The selection will be reviewed when migrating to ESP32, where the 1 KB memory footprint is negligible.
