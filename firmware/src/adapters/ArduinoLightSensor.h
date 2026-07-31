/**
 * @file ArduinoLightSensor.h
 * @brief Concrete Arduino Uno adapter for Analog LDR light sensor.
 * 
 * Responsibilities:
 * - Implement ILightSensor interface.
 * - Read analog pins to track ambient illumination.
 * 
 * TODO:
 * - [ ] Map the physical LDR analog channel config.
 */

#pragma once
#include "HAL/ILightSensor.h"

class ArduinoLightSensor : public ILightSensor {
private:
    uint8_t ldrPin;

public:
    ArduinoLightSensor(uint8_t pin);
    
    uint16_t getLightLevel() override;
};
