/**
 * @file ArduinoLed.cpp
 * @brief Concrete implementation for RGB Led emotional aura lighting.
 * 
 * Responsibilities:
 * - Configure digital pins as OUTPUT.
 * - Map AuraStates to specific RGB pins and handle blinking loops.
 */

#include "ArduinoLed.h"
#include <Arduino.h>

ArduinoLed::ArduinoLed(uint8_t rPin, uint8_t gPin, uint8_t bPin) 
    : redPin(rPin), greenPin(gPin), bluePin(bPin), 
      currentAura(AuraState::Idle), lastUpdateTime(0), blinkState(true) {
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    
    // Default startup state is off
    writePins(false, false, false);
}

void ArduinoLed::writePins(bool r, bool g, bool b) {
    digitalWrite(redPin, r ? HIGH : LOW);
    digitalWrite(greenPin, g ? HIGH : LOW);
    digitalWrite(bluePin, b ? HIGH : LOW);
}

void ArduinoLed::setAura(AuraState state) {
    if (currentAura != state) {
        currentAura = state;
        blinkState = true;
        lastUpdateTime = millis();
        // Trigger immediate update
        update();
    }
}

void ArduinoLed::clear() {
    writePins(false, false, false);
    // Silent state reset
    currentAura = AuraState::Idle; 
    blinkState = false;
}

void ArduinoLed::update() {
    uint32_t now = millis();
    uint32_t interval = 0;
    bool isBlinking = false;
    
    // Target base colors for each state
    bool targetR = false;
    bool targetG = false;
    bool targetB = false;

    switch (currentAura) {
        case AuraState::Idle:
            targetG = true; // Solid Green
            isBlinking = false;
            break;
            
        case AuraState::Happy:
            targetG = true;
            targetB = true; // Cyan
            isBlinking = true;
            interval = 800; // Slow blink
            break;
            
        case AuraState::Thinking:
            targetB = true; // Blue
            isBlinking = true;
            interval = 300; // Medium blink
            break;
            
        case AuraState::Sleeping:
            targetB = true; // Slow breathing simulated by slow blink (no PWM)
            isBlinking = true;
            interval = 2000; // Very slow blink
            break;
            
        case AuraState::Alert:
            targetR = true;
            targetG = true; // Yellow
            isBlinking = true;
            interval = 150; // Fast blink
            break;
            
        case AuraState::Error:
            targetR = true; // Red
            isBlinking = true;
            interval = 80; // Very fast alarm blink
            break;
    }

    if (isBlinking) {
        if (now - lastUpdateTime >= interval) {
            blinkState = !blinkState;
            lastUpdateTime = now;
        }
        
        // Output pins based on toggle state
        if (blinkState) {
            writePins(targetR, targetG, targetB);
        } else {
            writePins(false, false, false);
        }
    } else {
        // Output solid target state
        writePins(targetR, targetG, targetB);
    }
}
