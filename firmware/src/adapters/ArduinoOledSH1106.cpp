/**
 * @file ArduinoOledSH1106.cpp
 * @brief Concrete implementation for SH1106 I2C OLED screen output.
 * 
 * Responsibilities:
 * - Wrap library display calls.
 * 
 * TODO:
 * - [ ] Implement physical I2C setup and drawing.
 */

#include "ArduinoOledSH1106.h"

ArduinoOledSH1106::ArduinoOledSH1106() {
    // TODO: Constructor init
}

void ArduinoOledSH1106::begin() {
    // TODO: Init screen controller registers
}

void ArduinoOledSH1106::clear() {
    // TODO: Clear screen buffer
}

void ArduinoOledSH1106::display() {
    // TODO: Write buffer to display
}

void ArduinoOledSH1106::drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color) {
    (void)x0; (void)y0; (void)r; (void)color;
}

void ArduinoOledSH1106::fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color) {
    (void)x0; (void)y0; (void)r; (void)color;
}

void ArduinoOledSH1106::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color) {
    (void)x; (void)y; (void)w; (void)h; (void)color;
}

void ArduinoOledSH1106::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color) {
    (void)x; (void)y; (void)w; (void)h; (void)color;
}

void ArduinoOledSH1106::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color) {
    (void)x0; (void)y0; (void)x1; (void)y1; (void)color;
}
