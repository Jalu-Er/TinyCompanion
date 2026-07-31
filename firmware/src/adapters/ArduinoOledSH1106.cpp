/**
 * @file ArduinoOledSH1106.cpp
 * @brief Concrete implementation for SH1106 I2C OLED screen output.
 * 
 * Responsibilities:
 * - Wrap Adafruit_SH1106G driver library display calls.
 * - Map basic geometric primitives, pixels, and text to screen buffer coordinates.
 * 
 * TODO:
 * - [ ] Implement custom fonts or optimized layout buffers if required.
 */

#include "ArduinoOledSH1106.h"
#include <Wire.h>

ArduinoOledSH1106::ArduinoOledSH1106() 
    : oledDriver(128, 64, &Wire) {}

void ArduinoOledSH1106::begin() {
    // Standard I2C address for SH1106 is 0x3C
    oledDriver.begin(0x3C, true);
    oledDriver.clearDisplay();
    oledDriver.display();
}

void ArduinoOledSH1106::clear() {
    oledDriver.clearDisplay();
}

void ArduinoOledSH1106::display() {
    oledDriver.display();
}

void ArduinoOledSH1106::drawPixel(int16_t x, int16_t y, uint8_t color) {
    uint16_t mappedColor = (color != 0) ? SH110X_WHITE : SH110X_BLACK;
    oledDriver.drawPixel(x, y, mappedColor);
}

void ArduinoOledSH1106::drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color) {
    uint16_t mappedColor = (color != 0) ? SH110X_WHITE : SH110X_BLACK;
    oledDriver.drawCircle(x0, y0, r, mappedColor);
}

void ArduinoOledSH1106::fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color) {
    uint16_t mappedColor = (color != 0) ? SH110X_WHITE : SH110X_BLACK;
    oledDriver.fillCircle(x0, y0, r, mappedColor);
}

void ArduinoOledSH1106::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color) {
    uint16_t mappedColor = (color != 0) ? SH110X_WHITE : SH110X_BLACK;
    oledDriver.drawRect(x, y, w, h, mappedColor);
}

void ArduinoOledSH1106::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color) {
    uint16_t mappedColor = (color != 0) ? SH110X_WHITE : SH110X_BLACK;
    oledDriver.fillRect(x, y, w, h, mappedColor);
}

void ArduinoOledSH1106::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color) {
    uint16_t mappedColor = (color != 0) ? SH110X_WHITE : SH110X_BLACK;
    oledDriver.drawLine(x0, y0, x1, y1, mappedColor);
}

void ArduinoOledSH1106::drawText(int16_t x, int16_t y, const char* text, uint8_t size, uint8_t color) {
    uint16_t mappedColor = (color != 0) ? SH110X_WHITE : SH110X_BLACK;
    oledDriver.setCursor(x, y);
    oledDriver.setTextSize(size);
    oledDriver.setTextColor(mappedColor);
    oledDriver.print(text);
}

void ArduinoOledSH1106::drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, uint8_t color) {
    uint16_t mappedColor = (color != 0) ? SH110X_WHITE : SH110X_BLACK;
    oledDriver.drawBitmap(x, y, bitmap, w, h, mappedColor);
}
