/**
 * @file LedValidation.cpp
 * @brief Implementation of RGB LED Aura state hardware validation runner.
 * 
 * Responsibilities:
 * - Implement IValidation interface contract.
 * - Cycle through Idle, Happy, Thinking, Sleeping, Alert, and Error.
 */

#include "LedValidation.h"
#include <Arduino.h>

LedValidation::LedValidation(ILedAura& ledAura) 
    : aura(ledAura) {}

void LedValidation::runAuraDuration(AuraState state, uint32_t durationMs) {
    Serial.print(F("Displaying AuraState ["));
    
    switch (state) {
        case AuraState::Idle:     Serial.print(F("Idle"));     break;
        case AuraState::Happy:    Serial.print(F("Happy"));    break;
        case AuraState::Thinking: Serial.print(F("Thinking")); break;
        case AuraState::Sleeping: Serial.print(F("Sleeping")); break;
        case AuraState::Alert:    Serial.print(F("Alert"));    break;
        case AuraState::Error:    Serial.print(F("Error"));    break;
    }
    Serial.println(F("]..."));

    aura.setAura(state);
    uint32_t start = millis();
    while (millis() - start < durationMs) {
        aura.update();
        delay(1); // Small CPU yield
    }
}

void LedValidation::run() {
    Serial.println(F("Starting RGB LED Aura validation sequence..."));
    
    // Demonstrate every aura state sequentially
    runAuraDuration(AuraState::Idle, 3000);     // 3 seconds Green solid
    runAuraDuration(AuraState::Happy, 4000);    // 4 seconds Cyan slow blink
    runAuraDuration(AuraState::Thinking, 4000); // 4 seconds Blue medium blink
    runAuraDuration(AuraState::Sleeping, 6000); // 6 seconds Blue breathing (very slow blink)
    runAuraDuration(AuraState::Alert, 4000);    // 4 seconds Yellow fast blink
    runAuraDuration(AuraState::Error, 4000);    // 4 seconds Red very fast alarm blink
    
    // Reset aura to off
    aura.clear();
    Serial.println(F("RGB LED Aura validation completed successfully."));
}

const char* LedValidation::name() const {
    return "RGB LED Aura";
}

const char* LedValidation::description() const {
    return "Validates red/green/blue pin outputs and blinks rates corresponding to Idle, Happy, Thinking, Sleeping, Alert, and Error.";
}
