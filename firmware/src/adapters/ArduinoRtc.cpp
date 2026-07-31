/**
 * @file ArduinoRtc.cpp
 * @brief Concrete implementation for DS1307 Real-Time Clock module.
 * 
 * Responsibilities:
 * - Read/Write DS1307 time values via I2C.
 * - Auto-fallback to virtual time ticking if DS1307 is missing.
 */

#include "ArduinoRtc.h"
#include <Wire.h>

ArduinoRtc::ArduinoRtc() 
    : hardwarePresent(false), lastSimTime(0), simHour(7), simMinute(0) {
    // Initialize Wire bus if it hasn't been initialized
    Wire.begin();

    // Check if hardware DS1307 RTC is available and running
    if (rtcDriver.begin()) {
        if (rtcDriver.isrunning()) {
            hardwarePresent = true;
        } else {
            // Force start RTC if communication succeeded but oscillator was stopped
            rtcDriver.adjust(DateTime(F(__DATE__), F(__TIME__)));
            hardwarePresent = true;
        }
    }

    if (!hardwarePresent) {
        Serial.println(F("[RTC] Hardware DS1307 not found. Activating virtual time simulation."));
        lastSimTime = millis();
    }
}

bool ArduinoRtc::getTime(TimeStruct& outTime) {
    if (hardwarePresent) {
        DateTime now = rtcDriver.now();
        outTime.hour = now.hour();
        outTime.minute = now.minute();
        outTime.second = now.second();
        // RTClib: 0 = Sunday, 1 = Monday ... -> TimeStruct: 1 = Sunday, 2 = Monday ...
        outTime.dayOfWeek = now.dayOfTheWeek() + 1;
        return true;
    } else {
        // Virtual clock simulation tick: 1 virtual hour per 2 real-world seconds
        uint32_t now = millis();
        if (now - lastSimTime >= 2000) {
            simHour = (simHour + 1) % 24;
            lastSimTime = now;
            Serial.print(F("[RTC Simulator] Virtual hour advanced to: "));
            Serial.print(simHour);
            Serial.println(F(":00"));
        }
        outTime.hour = simHour;
        outTime.minute = simMinute;
        outTime.second = 0;
        outTime.dayOfWeek = 2; // Fixed to Monday
        return true;
    }
}

bool ArduinoRtc::setTime(const TimeStruct& newTime) {
    if (hardwarePresent) {
        // Map TimeStruct parameters to RTClib DateTime structure
        // Default year/month/day used as padding since we only track time periods
        rtcDriver.adjust(DateTime(2026, 8, 1, newTime.hour, newTime.minute, newTime.second));
        return true;
    } else {
        // Adjust virtual clock parameters
        simHour = newTime.hour;
        simMinute = newTime.minute;
        lastSimTime = millis();
        return true;
    }
}
