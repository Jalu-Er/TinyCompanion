/**
 * @file ArduinoTm1637.h
 * @brief Concrete Arduino Uno adapter for TM1637 4-digit display output.
 * 
 * Responsibilities:
 * - Implement ITm1637 interface.
 * - Manage I/O pins to drive segment display outputs.
 * 
 * TODO:
 * - [ ] Setup interface pin mappings.
 */

#pragma once
#include "HAL/ITm1637.h"

class ArduinoTm1637 : public ITm1637 {
private:
    uint8_t clkPin;
    uint8_t dioPin;

public:
    ArduinoTm1637(uint8_t clk, uint8_t dio);
    
    void displayInteger(uint16_t value, bool showColon) override;
    void clear() override;
};
