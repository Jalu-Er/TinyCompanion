/**
 * @file Event.h
 * @brief System event definitions and payload definitions.
 * 
 * Responsibilities:
 * - Define EventType enums to classify system events.
 * - Map payload fields using memory-efficient unions (AVR-safe).
 * 
 * TODO:
 * - [ ] Verify that all planned sensor states map to EventTypes.
 */

#pragma once
#include <stdint.h>

enum class EventType : uint8_t {
    NONE = 0,
    TOUCH_TRIGGERED,
    TOUCH_RELEASED,
    OBJECT_DETECTED_NEAR,  // Proximity <= 15cm
    OBJECT_DETECTED_FAR,   // Proximity > 15cm && <= 80cm
    OBJECT_LOST,           // Proximity > 80cm
    LIGHT_LEVEL_DARK,      // LDR below Dark Threshold
    LIGHT_LEVEL_BRIGHT,    // LDR above Dark Threshold
    TICK_MINUTE,           // Triggered by RTC once per minute
    EMOTION_CHANGED,
    STATE_CHANGED
};

struct Event {
    EventType type;
    union {
        uint16_t rawValue; // Optional raw data (distance, light level, etc.)
        struct {
            uint8_t val1;
            uint8_t val2;
        } bytes;
    } data;
};
