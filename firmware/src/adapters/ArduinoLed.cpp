/**
 * @file ArduinoLed.cpp
 * @brief Concrete implementation for RGB Led emotional aura lighting.
 * 
 * Responsibilities:
 * - Direct analogWrite values to physical RGB pins.
 * 
 * TODO:
 * - [ ] Implement pin mode configs and write commands.
 */

#include "ArduinoLed.h"

ArduinoLed::ArduinoLed(uint8_t rPin, uint8_t gPin, uint8_t bPin) 
    : redPin(rPin), greenPin(gPin), bluePin(bPin) {
    // TODO: Setup LED pins
}

void ArduinoLed::setColor(uint8_t r, uint8_t g, uint8_t b) {
    // TODO: Write analog values to RGB outputs
    (void)r;
    (void)g;
    (void)b;
}
