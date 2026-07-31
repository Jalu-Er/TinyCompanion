/**
 * @file main.cpp
 * @brief Test application for validating the SH1106 I2C OLED display driver.
 * 
 * Responsibilities:
 * - Initialize OLED hardware via the HAL adapter.
 * - Run a visual validation sequence displaying text, primitives, pixels, and bitmaps.
 * - Ensure clean compilation and minimal footprint on Uno.
 * 
 * TODO:
 * - [ ] Connect other managers and sensors in later validation milestones.
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

void setup() {
    Serial.begin(115200);
    Serial.println(F("Starting OLED validation sequence..."));

    // 1. Initialize display
    oled.begin();
    delay(500);

    // 2. Display static test text
    oled.clear();
    oled.drawText(10, 15, "TinyCompanion", 1, 1);
    oled.drawText(10, 35, "OLED Validation", 1, 1);
    oled.display();
    delay(2000);

    // 3. Clear screen
    oled.clear();
    oled.display();
    delay(500);

    // 4. Draw geometric primitives and pixel test
    // Screen boundary border
    oled.drawRect(0, 0, 128, 64, 1);
    
    // Custom lines
    oled.drawLine(5, 5, 122, 5, 1);
    oled.drawLine(5, 58, 122, 58, 1);

    // Circle primitives
    oled.drawCircle(32, 32, 12, 1);
    oled.fillCircle(96, 32, 10, 1);

    // Rectangle primitives
    oled.drawRect(58, 22, 12, 20, 1);

    // Individual pixel drawing
    oled.drawPixel(64, 32, 0); // Clear a single pixel in the center of the rect

    // 5. Render simple verification bitmap
    oled.drawBitmap(60, 48, checkmarkBitmap, 8, 8, 1);

    // Push buffer to display
    oled.display();
    Serial.println(F("OLED validation patterns rendered successfully."));
}

void loop() {
    // Keep display stable without blocking loop operations
    delay(100);
}
