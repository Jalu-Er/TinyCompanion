/**
 * @file SensorValidation.cpp
 * @brief Implementation of hardware sensor events validation runner.
 * 
 * Responsibilities:
 * - Poll SensorManager at 20 Hz and process generated semantic events.
 */

#include "SensorValidation.h"
#include <Arduino.h>

SensorValidation::SensorValidation(SensorManager& manager, EventQueue& queue)
    : sensorManager(manager), eventQueue(queue) {}

const char* SensorValidation::getEventName(EventType type) const {
    switch (type) {
        case EventType::NONE:                  return "NONE";
        case EventType::TOUCH_PRESSED:         return "TOUCH_PRESSED";
        case EventType::TOUCH_RELEASED:        return "TOUCH_RELEASED";
        case EventType::USER_APPROACHING:      return "USER_APPROACHING";
        case EventType::USER_LEAVING:          return "USER_LEAVING";
        case EventType::AMBIENT_DARK:          return "AMBIENT_DARK";
        case EventType::AMBIENT_BRIGHT:        return "AMBIENT_BRIGHT";
        case EventType::TIME_PERIOD_MORNING:   return "TIME_PERIOD_MORNING";
        case EventType::TIME_PERIOD_AFTERNOON: return "TIME_PERIOD_AFTERNOON";
        case EventType::TIME_PERIOD_EVENING:   return "TIME_PERIOD_EVENING";
        case EventType::TIME_PERIOD_NIGHT:     return "TIME_PERIOD_NIGHT";
        case EventType::EMOTION_CHANGED:       return "EMOTION_CHANGED";
        case EventType::STATE_CHANGED:         return "STATE_CHANGED";
        default:                               return "UNKNOWN_EVENT";
    }
}

void SensorValidation::run() {
    Serial.println(F("\n============================================="));
    Serial.println(F("Starting Semantic Sensor Events Test Runner"));
    Serial.println(F("Interact with touch, LDR, range, and RTC now..."));
    Serial.println(F("============================================="));

    eventQueue.clear();
    uint32_t lastPollTime = millis();

    while (true) {
        uint32_t now = millis();
        
        // Poll sensors at 20 Hz (every 50 ms)
        if (now - lastPollTime >= 50) {
            sensorManager.poll();
            lastPollTime = now;
        }

        // Dequeue and display events immediately
        Event ev;
        while (eventQueue.dequeue(ev)) {
            Serial.print(F("[EVENT DETECTED] Type: "));
            Serial.println(getEventName(ev.type));
        }

        delay(5); // Prevent CPU starvation on target systems
    }
}

const char* SensorValidation::name() const {
    return "Sensor Events";
}

const char* SensorValidation::description() const {
    return "Monitors touch, light, proximity, and RTC period events via the SensorManager and EventQueue.";
}
