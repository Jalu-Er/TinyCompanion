/**
 * @file ArduinoLightSensor.cpp
 * @brief Concrete implementation for Analog LDR light sensor.
 * 
 * Responsibilities:
 * - Configure analog pin as INPUT.
 * - Perform analogRead to return current light level intensity (0-1023).
 */

#include "ArduinoLightSensor.h"
#include <Arduino.h>

ArduinoLightSensor::ArduinoLightSensor(uint8_t pin) : ldrPin(pin) {
    pinMode(ldrPin, INPUT);
}

uint16_t ArduinoLightSensor::getLightLevel() {
    return analogRead(ldrPin);
}
