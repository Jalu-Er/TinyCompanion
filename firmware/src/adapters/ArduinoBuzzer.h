/**
 * @file ArduinoBuzzer.h
 * @brief Concrete Arduino Uno adapter for Passive Buzzer audio rendering.
 * 
 * Responsibilities:
 * - Implement IBuzzer interface.
 * - Map frequency sweeps to hardware pin outputs.
 * 
 * TODO:
 * - [ ] Map the physical buzzer pin layout.
 */

#pragma once
#include <stdint.h>
#include "HAL/IBuzzer.h"

class ArduinoBuzzer : public IBuzzer {
private:
    uint8_t buzzerPin;

public:
    ArduinoBuzzer(uint8_t pin);
    
    void playTone(uint16_t frequencyHz) override;
    void stopTone() override;
};
