/**
 * @file Event.h
 * @brief System semantic event definitions and payload structures.
 * 
 * Responsibilities:
 * - Define EventType enums to classify semantic system events.
 * - Restrict exposure of raw hardware metrics (centimeters, ADC levels) to logic.
 */

#pragma once
#include <stdint.h>

enum class EventType : uint8_t {
    NONE = 0,
    
    // Touch events (Edge detected)
    TOUCH_PRESSED,
    TOUCH_RELEASED,
    
    // Proximity events (Semantic zones)
    USER_APPROACHING, // Object entered near zone (<= 15cm)
    USER_LEAVING,     // Object left near zone (> 15cm)
    
    // Light events (Hysteresis filtered)
    AMBIENT_DARK,     // Dark environment threshold crossed
    AMBIENT_BRIGHT,   // Bright environment threshold crossed
    
    // RTC Period transitions
    TIME_PERIOD_MORNING,   // Morning period started
    TIME_PERIOD_AFTERNOON, // Afternoon period started
    TIME_PERIOD_EVENING,   // Evening period started
    TIME_PERIOD_NIGHT,     // Night period started
    
    // State indicators
    INITIALIZATION_FINISHED,
    EMOTION_CHANGED,
    STATE_CHANGED
};

struct Event {
    EventType type;
    union {
        uint16_t metadata; // Diagnostic or status flags (never raw ADC/cm values)
        struct {
            uint8_t val1;
            uint8_t val2;
        } bytes;
    } data;
};
