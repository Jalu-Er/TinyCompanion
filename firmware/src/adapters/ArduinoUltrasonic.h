/**
 * @file ArduinoUltrasonic.h
 * @brief Concrete Arduino Uno adapter for HC-SR04 ultrasonic proximity measurement.
 * 
 * Responsibilities:
 * - Implement IUltrasonic interface.
 * - Trigger echo pulses on pins and measure timing responses.
 * 
 * TODO:
 * - [ ] Implement pin mode setup routines.
 * - [ ] Use Timer/Pin Change Interrupts for non-blocking timing measurement.
 */

#pragma once
#include <stdint.h>
#include "HAL/IUltrasonic.h"

class ArduinoUltrasonic : public IUltrasonic {
private:
    uint8_t trigPin;
    uint8_t echoPin;

public:
    ArduinoUltrasonic(uint8_t trigger, uint8_t echo);
    
    uint16_t getDistanceCm() override;
};
