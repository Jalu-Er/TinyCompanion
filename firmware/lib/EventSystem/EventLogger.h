/**
 * @file EventLogger.h
 * @brief Lightweight, optional logger outputting event trace statements to USB Serial.
 * 
 * Responsibilities:
 * - Print semantic names and millisecond timestamp markers.
 * - Allow disabling logger entirely via macro flags to conserve memory space.
 */

#pragma once
#include "Event.h"
#include <Arduino.h>

// Define/Undefine this macro to enable or completely disable trace compiles
#define ENABLE_EVENT_LOGGING

class EventLogger {
public:
    /**
     * @brief Prints event metadata trace to serial monitor. Rollover-safe.
     */
    static void log(EventType type, uint32_t timestamp) {
#ifdef ENABLE_EVENT_LOGGING
        Serial.print(F("["));
        Serial.print(timestamp);
        Serial.print(F(" ms] EVENT "));
        
        switch (type) {
            case EventType::NONE:                  Serial.println(F("NONE")); break;
            case EventType::TOUCH_PRESSED:         Serial.println(F("TouchPressed")); break;
            case EventType::TOUCH_RELEASED:        Serial.println(F("TouchReleased")); break;
            case EventType::USER_APPROACHING:      Serial.println(F("UserApproaching")); break;
            case EventType::USER_LEAVING:          Serial.println(F("UserLeaving")); break;
            case EventType::AMBIENT_DARK:          Serial.println(F("AmbientDark")); break;
            case EventType::AMBIENT_BRIGHT:        Serial.println(F("AmbientBright")); break;
            case EventType::TIME_PERIOD_MORNING:   Serial.println(F("MorningStarted")); break;
            case EventType::TIME_PERIOD_AFTERNOON: Serial.println(F("AfternoonStarted")); break;
            case EventType::TIME_PERIOD_EVENING:   Serial.println(F("EveningStarted")); break;
            case EventType::TIME_PERIOD_NIGHT:     Serial.println(F("NightStarted")); break;
            case EventType::EMOTION_CHANGED:       Serial.println(F("EmotionChanged")); break;
            case EventType::STATE_CHANGED:         Serial.println(F("StateChanged")); break;
            default:                               Serial.println(F("Unknown")); break;
        }
#else
        (void)type;
        (void)timestamp;
#endif
    }
};
