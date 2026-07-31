/**
 * @file Scheduler.h
 * @brief Lightweight, non-blocking cooperative scheduler for periodic tasks.
 * 
 * Responsibilities:
 * - Hold a static list of tasks without dynamic allocation.
 * - Execute task callbacks at designated time intervals.
 * - Provide rollover-safe tick comparison.
 */

#pragma once
#include <stdint.h>
#include <Arduino.h> // Required for millis() timing reference

typedef void (*TaskCallback)();

struct Task {
    const char* name;
    uint32_t intervalMs;
    TaskCallback callback;
    uint32_t lastExecutionTime;
};

class Scheduler {
private:
    static constexpr uint8_t MAX_TASKS = 8; // Suited for AVR SRAM constraints
    Task tasks[MAX_TASKS];
    uint8_t taskCount = 0;

public:
    Scheduler() = default;

    /**
     * @brief Adds a new periodic task to the scheduler list.
     * @param[in] name Descriptive task identifier.
     * @param[in] intervalMs Periodic call frequency interval.
     * @param[in] callback Function pointer to execute.
     * @return true if added successfully, false if capacity limit reached.
     */
    bool addTask(const char* name, uint32_t intervalMs, TaskCallback callback) {
        if (taskCount >= MAX_TASKS) {
            return false; // Exceeded limit, drop task registration
        }
        tasks[taskCount] = { name, intervalMs, callback, 0 };
        taskCount++;
        return true;
    }

    /**
     * @brief Evaluates time boundaries and triggers ready callbacks. Rollover-safe.
     */
    void tick() {
        uint32_t now = millis();
        for (uint8_t i = 0; i < taskCount; ++i) {
            if (now - tasks[i].lastExecutionTime >= tasks[i].intervalMs) {
                // Execute the periodic logic
                tasks[i].callback();
                // Update execution stamp (rollover-safe alignment)
                tasks[i].lastExecutionTime = now;
            }
        }
    }
};
