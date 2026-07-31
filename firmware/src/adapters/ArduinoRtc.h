/**
 * @file ArduinoRtc.h
 * @brief Concrete Arduino Uno adapter for DS1307 Real-Time Clock module.
 * 
 * Responsibilities:
 * - Implement IRtcClock interface.
 * - Manage I2C communications to read/write time values on DS1307.
 * 
 * TODO:
 * - [ ] Include I2C RTC libraries and manage chip initializations.
 */

#pragma once
#include "HAL/IRtcClock.h"

class ArduinoRtc : public IRtcClock {
public:
    ArduinoRtc();
    
    bool getTime(TimeStruct& outTime) override;
    bool setTime(const TimeStruct& newTime) override;
};
