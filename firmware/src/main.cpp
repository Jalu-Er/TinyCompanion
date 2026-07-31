/**
 * @file main.cpp
 * @brief Benchmark application for evaluating the SH1106 I2C OLED display rendering performance.
 * 
 * Responsibilities:
 * - Initialize OLED display via the HAL adapter.
 * - Run sequential benchmark screens (Pixels, Lines, Rects, Circles, Bitmaps, Text).
 * - Measure execution times for draw operations and display buffer I2C transfer.
 * - Print precise measurements to the Serial logger in microseconds.
 * 
 * TODO:
 * - [ ] Connect back sensors and state logic engines once evaluation is approved.
 */

#include <Arduino.h>
#include "Config.h"
#include "adapters/ArduinoOledSH1106.h"

// Concrete adapter instance
static ArduinoOledSH1106 oled;

// Simple 8x8 checkmark validation bitmap stored in flash memory
const uint8_t checkmarkBitmap[] PROGMEM = {
    0b00000000,
    0b00000001,
    0b00000011,
    0b00010110,
    0b01011100,
    0b00111000,
    0b00010000,
    0b00000000
};

// Forward declarations of benchmark helper functions
void runPixelBenchmark();
void runLineBenchmark();
void runRectBenchmark();
void runCircleBenchmark();
void runBitmapBenchmark();
void runTextBenchmark();
void reportMetrics(const char* label, uint32_t drawTimeUs, uint32_t transferTimeUs);

void setup() {
    Serial.begin(115200);
    while(!Serial); // Wait for Serial console on USB
    Serial.println(F("\n==========================================="));
    Serial.println(F("TinyCompanion OLED Rendering Benchmark Boot"));
    Serial.println(F("==========================================="));

    // Initialize OLED hardware
    oled.begin();
    delay(500);
}

void loop() {
    Serial.println(F("\n--- Starting New Benchmark Cycle ---"));

    runPixelBenchmark();
    delay(2000);

    runLineBenchmark();
    delay(2000);

    runRectBenchmark();
    delay(2000);

    runCircleBenchmark();
    delay(2000);

    runBitmapBenchmark();
    delay(2000);

    runTextBenchmark();
    delay(4000);
}

void reportMetrics(const char* label, uint32_t drawTimeUs, uint32_t transferTimeUs) {
    uint32_t totalTimeUs = drawTimeUs + transferTimeUs;
    float maxFps = 1000000.0f / (float)totalTimeUs;

    Serial.print(F("Benchmark ["));
    Serial.print(label);
    Serial.println(F("]:"));
    
    Serial.print(F("  Draw Time:     "));
    Serial.print(drawTimeUs);
    Serial.println(F(" us"));
    
    Serial.print(F("  Transfer Time: "));
    Serial.print(transferTimeUs);
    Serial.println(F(" us"));
    
    Serial.print(F("  Total Cycle:   "));
    Serial.print(totalTimeUs);
    Serial.println(F(" us"));
    
    Serial.print(F("  Theoretical FPS: "));
    Serial.println(maxFps, 2);
    Serial.println(F("-------------------------------------------"));
}

void runPixelBenchmark() {
    uint32_t startDraw = micros();
    oled.clear();
    
    // Draw 100 random pixels
    randomSeed(12345); // Seed for deterministic random layouts
    for (int i = 0; i < 100; ++i) {
        int16_t x = random(0, 128);
        int16_t y = random(0, 64);
        oled.drawPixel(x, y, 1);
    }
    uint32_t endDraw = micros();
    
    uint32_t startTransfer = micros();
    oled.display();
    uint32_t endTransfer = micros();

    reportMetrics("100 Random Pixels", (endDraw - startDraw), (endTransfer - startTransfer));
}

void runLineBenchmark() {
    uint32_t startDraw = micros();
    oled.clear();
    
    // Draw 20 geometric lines
    for (int i = 0; i < 20; ++i) {
        int16_t x0 = i * 6;
        int16_t y0 = 0;
        int16_t x1 = 127 - (i * 6);
        int16_t y1 = 63;
        oled.drawLine(x0, y0, x1, y1, 1);
    }
    uint32_t endDraw = micros();
    
    uint32_t startTransfer = micros();
    oled.display();
    uint32_t endTransfer = micros();

    reportMetrics("20 Geometric Lines", (endDraw - startDraw), (endTransfer - startTransfer));
}

void runRectBenchmark() {
    uint32_t startDraw = micros();
    oled.clear();
    
    // Draw 5 hollow rectangles and 5 filled rectangles
    for (int i = 0; i < 5; ++i) {
        oled.drawRect(i * 10, i * 5, 20, 15, 1);
        oled.fillRect(127 - (i * 10) - 20, 63 - (i * 5) - 15, 20, 15, 1);
    }
    uint32_t endDraw = micros();
    
    uint32_t startTransfer = micros();
    oled.display();
    uint32_t endTransfer = micros();

    reportMetrics("10 Rectangles (Hollow/Filled)", (endDraw - startDraw), (endTransfer - startTransfer));
}

void runCircleBenchmark() {
    uint32_t startDraw = micros();
    oled.clear();
    
    // Draw 5 hollow circles and 5 filled circles
    for (int i = 0; i < 5; ++i) {
        oled.drawCircle(32, 32, i * 6 + 2, 1);
        oled.fillCircle(96, 32, i * 4 + 2, 1);
    }
    uint32_t endDraw = micros();
    
    uint32_t startTransfer = micros();
    oled.display();
    uint32_t endTransfer = micros();

    reportMetrics("10 Circles (Hollow/Filled)", (endDraw - startDraw), (endTransfer - startTransfer));
}

void runBitmapBenchmark() {
    uint32_t startDraw = micros();
    oled.clear();
    
    // Draw 10 bitmaps on the display grid
    for (int i = 0; i < 10; ++i) {
        int16_t x = i * 12 + 5;
        int16_t y = (i % 2 == 0) ? 15 : 40;
        oled.drawBitmap(x, y, checkmarkBitmap, 8, 8, 1);
    }
    uint32_t endDraw = micros();
    
    uint32_t startTransfer = micros();
    oled.display();
    uint32_t endTransfer = micros();

    reportMetrics("10 Checkmark Bitmaps (8x8)", (endDraw - startDraw), (endTransfer - startTransfer));
}

void runTextBenchmark() {
    uint32_t startDraw = micros();
    oled.clear();
    
    // Print 3 lines of text
    oled.drawText(0, 5, "TinyCompanion Project", 1, 1);
    oled.drawText(0, 25, "Rendering Benchmark", 1, 1);
    oled.drawText(0, 45, "FPS: Measuring...", 1, 1);
    uint32_t endDraw = micros();
    
    uint32_t startTransfer = micros();
    oled.display();
    uint32_t endTransfer = micros();

    reportMetrics("3 Lines of Static Text", (endDraw - startDraw), (endTransfer - startTransfer));
}
