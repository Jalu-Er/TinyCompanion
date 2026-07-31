/**
 * @file EventHistory.h
 * @brief Circular history buffer storing recent semantic events.
 * 
 * Responsibilities:
 * - Buffer recent events with timestamps statically in RAM.
 * - Overwrite oldest entries automatically when buffer fills.
 * - Expose read interfaces for diagnostic inspections.
 */

#pragma once
#include "Event.h"

struct HistoryEntry {
    EventType type;
    uint32_t timestamp;
};

class EventHistory {
private:
    static constexpr uint8_t HISTORY_SIZE = 8;
    HistoryEntry history[HISTORY_SIZE];
    uint8_t head = 0;
    uint8_t count = 0;

public:
    EventHistory() {
        for (uint8_t i = 0; i < HISTORY_SIZE; ++i) {
            history[i] = { EventType::NONE, 0 };
        }
    }

    /**
     * @brief Pushes a new record into the history trace, overwriting the oldest if full.
     */
    void record(EventType type, uint32_t timestamp) {
        uint8_t index = (head + count) % HISTORY_SIZE;
        if (count == HISTORY_SIZE) {
            // Buffer full, overwrite the oldest index and shift head forward
            history[head] = { type, timestamp };
            head = (head + 1) % HISTORY_SIZE;
        } else {
            history[index] = { type, timestamp };
            count++;
        }
    }

    /**
     * @brief Returns the count of occupied history records.
     */
    uint8_t getCount() const { return count; }

    /**
     * @brief Fetches a history record. Index 0 represents the oldest record.
     * @param[in] index History index relative to head (0 to count-1).
     * @param[out] outEntry Copy target for the trace data.
     * @return true on success, false if index out of bounds.
     */
    bool getEntry(uint8_t index, HistoryEntry& outEntry) const {
        if (index >= count) {
            return false;
        }
        uint8_t actualIndex = (head + index) % HISTORY_SIZE;
        outEntry = history[actualIndex];
        return true;
    }
};
