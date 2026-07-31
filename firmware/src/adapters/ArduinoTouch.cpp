/**
 * @file ArduinoTouch.cpp
 * @brief Concrete implementation for TTP223 capacitive touch sensor.
 * 
 * Responsibilities:
 * - Configure touch digital pin as INPUT.
 * - Perform pin reads to detect capacitive touch active-high transitions.
 */

#include "ArduinoTouch.h"
#include <Arduino.h>

ArduinoTouch::ArduinoTouch(uint8_t pin) : touchPin(pin) {
    pinMode(touchPin, INPUT);
}

bool ArduinoTouch::isTouched() {
    return (digitalRead(touchPin) == HIGH);
}
