/**
 * @file ArduinoLightSensor.cpp
 * @brief Concrete implementation for Analog LDR light sensor.
 * 
 * Responsibilities:
 * - Read LDR analog levels.
 * 
 * TODO:
 * - [ ] Implement analog reading functions.
 */

#include "ArduinoLightSensor.h"

ArduinoLightSensor::ArduinoLightSensor(uint8_t pin) : ldrPin(pin) {
    // TODO: Initialize LDR pin
}

uint16_t ArduinoLightSensor::getLightLevel() {
    // TODO: Read LDR analog channel
    return 0;
}
