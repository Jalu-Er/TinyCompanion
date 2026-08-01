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
    : u8g2(U8G2_R0, U8X8_PIN_NONE) {}

void ArduinoOledSH1106::begin() {
    u8g2.begin();
}

void ArduinoOledSH1106::clear() {
    u8g2.clearDisplay();
}

void ArduinoOledSH1106::display() {
    // In page buffer mode, transfer occurs dynamically during loop nextPage() ticks
}

void ArduinoOledSH1106::firstPage() {
    u8g2.firstPage();
}

bool ArduinoOledSH1106::nextPage() {
    return u8g2.nextPage();
}

void ArduinoOledSH1106::drawPixel(int16_t x, int16_t y, uint8_t color) {
    u8g2.setDrawColor(color != 0 ? 1 : 0);
    u8g2.drawPixel(x, y);
}

void ArduinoOledSH1106::drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color) {
    u8g2.setDrawColor(color != 0 ? 1 : 0);
    u8g2.drawCircle(x0, y0, r);
}

void ArduinoOledSH1106::fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color) {
    u8g2.setDrawColor(color != 0 ? 1 : 0);
    u8g2.drawDisc(x0, y0, r);
}

void ArduinoOledSH1106::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color) {
    u8g2.setDrawColor(color != 0 ? 1 : 0);
    u8g2.drawFrame(x, y, w, h);
}

void ArduinoOledSH1106::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color) {
    u8g2.setDrawColor(color != 0 ? 1 : 0);
    u8g2.drawBox(x, y, w, h);
}

void ArduinoOledSH1106::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color) {
    u8g2.setDrawColor(color != 0 ? 1 : 0);
    u8g2.drawRFrame(x, y, w, h, r);
}

void ArduinoOledSH1106::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color) {
    u8g2.setDrawColor(color != 0 ? 1 : 0);
    u8g2.drawRBox(x, y, w, h, r);
}

void ArduinoOledSH1106::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color) {
    u8g2.setDrawColor(color != 0 ? 1 : 0);
    u8g2.drawLine(x0, y0, x1, y1);
}

void ArduinoOledSH1106::drawText(int16_t x, int16_t y, const char* text, uint8_t size, uint8_t color) {
    (void)size;
    u8g2.setDrawColor(color != 0 ? 1 : 0);
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.drawStr(x, y, text);
}

void ArduinoOledSH1106::drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, uint8_t color) {
    u8g2.setDrawColor(color != 0 ? 1 : 0);
    u8g2.drawBitmap(x, y, (w + 7) / 8, h, bitmap);
}

uint16_t ArduinoOledSH1106::getWidth() const {
    return u8g2.getDisplayWidth();
}

uint16_t ArduinoOledSH1106::getHeight() const {
    return u8g2.getDisplayHeight();
}
