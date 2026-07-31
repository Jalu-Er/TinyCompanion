/**
 * @file ArduinoLed.h
 * @brief Concrete Arduino Uno adapter for RGB Led emotional aura lighting.
 * 
 * Responsibilities:
 * - Implement ILedAura interface.
 * - Manage active digital outputs based on current AuraState.
 * - Compute non-blocking time-based blinking schedules internally.
 */

#pragma once
#include <stdint.h>
#include "HAL/ILedAura.h"

class ArduinoLed : public ILedAura {
private:
    uint8_t redPin;
    uint8_t greenPin;
    uint8_t bluePin;
    
    AuraState currentAura;
    uint32_t lastUpdateTime;
    bool blinkState; // Controls blinking toggle state

    // Internal helper to apply raw digital values to the hardware pins
    void writePins(bool r, bool g, bool b);

public:
    ArduinoLed(uint8_t rPin, uint8_t gPin, uint8_t bPin);
    
    void setAura(AuraState state) override;
    void clear() override;
    void update() override;
};
