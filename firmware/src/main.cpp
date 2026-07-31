/**
 * @file main.cpp
 * @brief Entry point selecting and executing the semantic sensor events validation test runner.
 * 
 * Responsibilities:
 * - Instantiate concrete sensor and clock adapters.
 * - Establish EventQueue and SensorManager bindings.
 * - Delegate execution to the SensorValidation runner.
 * 
 * TODO:
 * - [ ] Connect state machine and output presentation managers in next phase.
 */

#include <Arduino.h>
#include "Config.h"
#include "adapters/ArduinoUltrasonic.h"
#include "adapters/ArduinoTouch.h"
#include "adapters/ArduinoLightSensor.h"
#include "adapters/ArduinoRtc.h"
#include "EventSystem/EventQueue.h"
#include "managers/SensorManager.h"
#include "validation/SensorValidation.h"

// 1. Concrete hardware adapters (using Config.h pinouts)
static ArduinoUltrasonic ultrasonic(PIN_TRIG, PIN_ECHO);
static ArduinoTouch touch(PIN_TOUCH);
static ArduinoLightSensor light(PIN_LDR);
static ArduinoRtc rtc;

// 2. Event infrastructure
static EventQueue eventQueue;

// 3. Sensor manager orchestrator
static SensorManager sensorManager(ultrasonic, touch, light, rtc, eventQueue);

// 4. Test validation runner
static SensorValidation sensorValidator(sensorManager, eventQueue);

void setup() {
    Serial.begin(115200);
    while(!Serial); // Wait for Serial console on USB
    Serial.println(F("System booting..."));

    // Execute the sensor validation test suite (blocks inside run())
    sensorValidator.run();
}

void loop() {
    // Execution will not reach here as validator blocks internally
    delay(100);
}
