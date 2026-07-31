/**
 * @file main.cpp
 * @brief Entry point selecting and executing the RGB LED Aura validation test runner.
 * 
 * Responsibilities:
 * - Instantiate concrete hardware LED adapters.
 * - Execute the ambient lighting validation routine.
 * 
 * TODO:
 * - [ ] Switch between different hardware validators dynamically if needed.
 */

#include <Arduino.h>
#include "Config.h"
#include "adapters/ArduinoLed.h"
#include "validation/LedValidation.h"

// Concrete adapter instance for RGB LEDs mapped to red, green, and blue pins
static ArduinoLed ledAura(LED_RED, LED_GREEN, LED_BLUE);

// Validation runner instance for LED Aura states
static LedValidation ledValidator(ledAura);

void setup() {
    Serial.begin(115200);
    while(!Serial); // Wait for Serial console on USB
    Serial.println(F("System booting..."));

    // Execute the LED validation sequence
    ledValidator.run();
}

void loop() {
    // Keep system idle
    delay(500);
}
