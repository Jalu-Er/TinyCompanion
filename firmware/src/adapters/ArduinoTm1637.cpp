/**
 * @file ArduinoTm1637.cpp
 * @brief Concrete implementation for TM1637 4-digit display output.
 * 
 * Responsibilities:
 * - Update digits on segment output.
 * 
 * TODO:
 * - [ ] Implement segment conversion mapping arrays.
 */

#include "ArduinoTm1637.h"

ArduinoTm1637::ArduinoTm1637(uint8_t clk, uint8_t dio) 
    : clkPin(clk), dioPin(dio) {
    // TODO: Init display pin modes
}

void ArduinoTm1637::displayInteger(uint16_t value, bool showColon) {
    // TODO: Write segmented display values
    (void)value;
    (void)showColon;
}

void ArduinoTm1637::clear() {
    // TODO: Blank all segment digits
}
