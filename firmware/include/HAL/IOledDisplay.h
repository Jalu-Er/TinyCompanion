/**
 * @file IOledDisplay.h
 * @brief Hardware Abstraction Layer interface for OLED graphics display rendering.
 * 
 * Responsibilities:
 * - Define basic screen buffer draw wrappers (circle, rect, line).
 * - Isolate applications from display library drivers (e.g. Adafruit_SH1106).
 * 
 * TODO:
 * - [ ] Implement concrete SH1106 I2C adapter using Wire or Adafruit SH1106 libraries.
 */

#pragma once
#include <stdint.h>

class IOledDisplay {
public:
    virtual ~IOledDisplay() {}
    
    /**
     * @brief Initialize physical screen hardware and buffers.
     */
    virtual void begin() = 0;
    
    /**
     * @brief Clear the internal screen layout buffers.
     */
    virtual void clear() = 0;
    
    /**
     * @brief Push the local buffer state onto the physical display registers.
     */
    virtual void display() = 0;

    // Page-buffer iteration methods
    virtual void firstPage() = 0;
    virtual bool nextPage() = 0;
    
    // Draw primitives
    virtual void drawPixel(int16_t x, int16_t y, uint8_t color) = 0;
    virtual void drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color) = 0;
    virtual void fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color) = 0;
    virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color) = 0;
    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color) = 0;
    virtual void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color) = 0;
    virtual void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color) = 0;
    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color) = 0;
    virtual void drawText(int16_t x, int16_t y, const char* text, uint8_t size, uint8_t color) = 0;
    virtual void drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, uint8_t color) = 0;

    // Dimension getters
    virtual uint16_t getWidth() const = 0;
    virtual uint16_t getHeight() const = 0;
};
