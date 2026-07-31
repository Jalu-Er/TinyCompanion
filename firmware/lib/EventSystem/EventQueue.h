/**
 * @file EventQueue.h
 * @brief Thread-safe (static circular buffer) event queue interface.
 * 
 * Responsibilities:
 * - Buffer system events without using dynamic heap allocation.
 * - Manage head and tail pointers safely under concurrency ticks.
 * 
 * TODO:
 * - [ ] Implement critical sections if ISR calls enqueue() directly.
 */

#pragma once
#include "Event.h"

class EventQueue {
private:
    static constexpr uint8_t QUEUE_SIZE = 8; // Small buffer suited for AVR SRAM
    Event queue[QUEUE_SIZE];
    uint8_t head = 0;
    uint8_t tail = 0;
    uint8_t count = 0;

public:
    EventQueue() = default;

    /**
     * @brief Pushes a new event onto the queue.
     * @return true if enqueued successfully, false if the buffer is full.
     */
    bool enqueue(const Event& event) {
        if (count >= QUEUE_SIZE) {
            return false; // Queue full, drop event
        }
        queue[tail] = event;
        tail = (tail + 1) % QUEUE_SIZE;
        count++;
        return true;
    }

    /**
     * @brief Pops an event from the queue.
     * @param[out] outEvent Location to copy the popped event data.
     * @return true if an event was retrieved, false if the queue was empty.
     */
    bool dequeue(Event& outEvent) {
        if (count == 0) {
            return false; // Empty
        }
        outEvent = queue[head];
        head = (head + 1) % QUEUE_SIZE;
        count--;
        return true;
    }

    /**
     * @brief Checks if the queue contains no elements.
     */
    bool isEmpty() const { return count == 0; }

    /**
     * @brief Resets queue indices.
     */
    void clear() { head = 0; tail = 0; count = 0; }
};
