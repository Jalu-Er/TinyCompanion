/**
 * @file ArduinoTouch.h
 * @brief Concrete Arduino Uno adapter for TTP223 capacitive touch sensor.
 * 
 * Responsibilities:
 * - Implement ITouchSensor interface.
 * - Read digital pins to check touch status.
 * 
 * TODO:
 * - [ ] Map the physical touch pin config.
 */

#pragma once
#include <stdint.h>
#include "HAL/ITouchSensor.h"

class ArduinoTouch : public ITouchSensor {
private:
    uint8_t touchPin;

public:
    ArduinoTouch(uint8_t pin);
    
    bool isTouched() override;
};
