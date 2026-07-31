/**
 * @file ArduinoLed.h
 * @brief Concrete Arduino Uno adapter for RGB Led emotional aura lighting.
 * 
 * Responsibilities:
 * - Implement ILedAura interface.
 * - Map PWM duty values to physical Red, Green, and Blue pins.
 * 
 * TODO:
 * - [ ] Verify that red, green, blue pins are mapped to PWM-capable Uno pins.
 */

#pragma once
#include <stdint.h>
#include "HAL/ILedAura.h"

class ArduinoLed : public ILedAura {
private:
    uint8_t redPin;
    uint8_t greenPin;
    uint8_t bluePin;

public:
    ArduinoLed(uint8_t rPin, uint8_t gPin, uint8_t bPin);
    
    void setColor(uint8_t r, uint8_t g, uint8_t b) override;
};
