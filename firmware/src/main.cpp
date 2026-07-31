/**
 * @file main.cpp
 * @brief Entry point selecting and executing the OLED hardware validation test runner.
 * 
 * Responsibilities:
 * - Instantiate concrete hardware display adapters.
 * - Delegate hardware checks to the dedicated validation module.
 * 
 * TODO:
 * - [ ] Connect behavior schedulers and sensor managers in subsequent milestones.
 */

#include <Arduino.h>
#include "adapters/ArduinoOledSH1106.h"
#include "validation/OledValidation.h"

// Concrete adapter instance
static ArduinoOledSH1106 oledDisplay;

// Validation runner instance
static OledValidation oledValidator(oledDisplay);

void setup() {
    Serial.begin(115200);
    while(!Serial); // Wait for Serial console on USB
    Serial.println(F("System booting..."));

    // Execute the OLED validation sequence
    oledValidator.runTest();
}

void loop() {
    // Keep display stable without blocking loop operations
    delay(500);
}
