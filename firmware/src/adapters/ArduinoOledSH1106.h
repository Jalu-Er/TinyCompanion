/**
 * @file ArduinoOledSH1106.h
 * @brief Concrete Arduino Uno adapter for SH1106 I2C OLED screen output.
 * 
 * Responsibilities:
 * - Implement IOledDisplay interface.
 * - Map basic drawing primitives (rect, line, circle) to SH1106 screen registers.
 * 
 * TODO:
 * - [ ] Wrap SH1106 display driver libraries cleanly.
 */

#pragma once
#include <stdint.h>
#include <Adafruit_SH110X.h>
#include "HAL/IOledDisplay.h"

class ArduinoOledSH1106 : public IOledDisplay {
private:
    Adafruit_SH1106G oledDriver;

public:
    ArduinoOledSH1106();
    
    void begin() override;
    void clear() override;
    void display() override;
    
    void drawPixel(int16_t x, int16_t y, uint8_t color) override;
    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color) override;
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color) override;
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color) override;
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color) override;
    void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color) override;
    void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color) override;
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color) override;
    void drawText(int16_t x, int16_t y, const char* text, uint8_t size, uint8_t color) override;
    void drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, uint8_t color) override;

    uint16_t getWidth() const override;
    uint16_t getHeight() const override;
};
