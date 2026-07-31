/**
 * @file EventStatistics.h
 * @brief Accumulates semantic event counters to assist dynamic engines.
 * 
 * Responsibilities:
 * - Hold individual event counter values statically in RAM.
 */

#pragma once
#include "Event.h"

class EventStatistics {
private:
    static constexpr uint8_t NUM_EVENTS = 16; // Suited for target EventType count
    uint16_t counters[NUM_EVENTS];

public:
    EventStatistics() {
        clear();
    }

    /**
     * @brief Increments counter for the given EventType.
     */
    void recordEvent(EventType type) {
        uint8_t idx = static_cast<uint8_t>(type);
        if (idx < NUM_EVENTS) {
            counters[idx]++;
        }
    }

    /**
     * @brief Fetches current counter value for the target EventType.
     */
    uint16_t getCounter(EventType type) const {
        uint8_t idx = static_cast<uint8_t>(type);
        if (idx < NUM_EVENTS) {
            return counters[idx];
        }
        return 0;
    }

    /**
     * @brief Reset all counters to zero.
     */
    void clear() {
        for (uint8_t i = 0; i < NUM_EVENTS; ++i) {
            counters[i] = 0;
        }
    }
};
