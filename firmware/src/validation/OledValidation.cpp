/**
 * @file OledValidation.cpp
 * @brief Concrete execution code for OLED SH1106 display hardware validation tests.
 * 
 * Responsibilities:
 * - Implement IValidation interface contract.
 * - Render static verification texts, shapes, lines, pixels, and checkmark bitmaps.
 * 
 * TODO:
 * - [ ] Implement more advanced benchmark tests if required.
 */

#include "OledValidation.h"
#include <Arduino.h>

// Simple 8x8 checkmark bitmap stored in flash memory
const uint8_t checkmarkValidation[] PROGMEM = {
    0b00000000,
    0b00000001,
    0b00000011,
    0b00010110,
    0b01011100,
    0b00111000,
    0b00010000,
    0b00000000
};

OledValidation::OledValidation(IOledDisplay& oledDisplay) 
    : display(oledDisplay) {}

void OledValidation::run() {
    Serial.println(F("Initializing OLED validation tests..."));

    // 1. Initialize display
    display.begin();
    delay(500);

    // 2. Display static test text
    display.clear();
    display.drawText(10, 15, "TinyCompanion", 1, 1);
    display.drawText(10, 35, "OLED Abstraction", 1, 1);
    display.display();
    delay(2000);

    // 3. Clear screen
    display.clear();
    display.display();
    delay(500);

    // 4. Draw geometric primitives and pixel test
    // Screen boundary border
    display.drawRect(0, 0, display.getWidth(), display.getHeight(), 1);
    
    // Custom lines
    display.drawLine(5, 5, display.getWidth() - 6, 5, 1);
    display.drawLine(5, display.getHeight() - 6, display.getWidth() - 6, display.getHeight() - 6, 1);

    // Circle primitives
    display.drawCircle(32, 32, 12, 1);
    display.fillCircle(96, 32, 10, 1);

    // Rectangle primitives
    display.drawRect(58, 20, 12, 18, 1);
    display.fillRoundRect(58, 40, 12, 10, 2, 1);

    // Individual pixel drawing
    display.drawPixel(64, 29, 0); // Clear a single pixel in the center of the rect

    // 5. Render simple verification bitmap
    display.drawBitmap(60, 52, checkmarkValidation, 8, 8, 1);

    // Push buffer to display
    display.display();
    Serial.println(F("OLED abstraction test patterns rendered successfully."));
}

const char* OledValidation::name() const {
    return "OLED SH1106";
}

const char* OledValidation::description() const {
    return "Validates the SH1106 128x64 display I2C connection, text output, and primitive coordinates.";
}
