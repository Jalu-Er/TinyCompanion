/**
 * @file ArduinoRtc.h
 * @brief Concrete Arduino Uno adapter for DS1307 Real-Time Clock module.
 * 
 * Responsibilities:
 * - Implement IRtcClock interface.
 * - Manage I2C DS1307 commands using adafruit/RTClib.
 * - Supply virtual time progression if physical RTC is disconnected.
 */

#pragma once
#include <RTClib.h>
#include "HAL/IRtcClock.h"

class ArduinoRtc : public IRtcClock {
private:
    RTC_DS1307 rtcDriver;
    bool hardwarePresent;

    // Simulation helpers
    uint32_t lastSimTime;
    uint8_t simHour;
    uint8_t simMinute;

public:
    ArduinoRtc();
    
    bool getTime(TimeStruct& outTime) override;
    bool setTime(const TimeStruct& newTime) override;
};
