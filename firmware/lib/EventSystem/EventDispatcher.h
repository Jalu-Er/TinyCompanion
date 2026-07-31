/**
 * @file EventDispatcher.h
 * @brief Dispatcher delivering queued events to registered consumers.
 * 
 * Responsibilities:
 * - Maintain a static list of IEventConsumer references (no dynamic heap allocations).
 * - Distribute events deterministically to all registered consumers.
 */

#pragma once
#include "IEventConsumer.h"

class EventDispatcher {
private:
    static constexpr uint8_t MAX_CONSUMERS = 6; // Fits the target module layout
    IEventConsumer* consumers[MAX_CONSUMERS];
    uint8_t consumerCount = 0;

public:
    EventDispatcher() {
        for (uint8_t i = 0; i < MAX_CONSUMERS; ++i) {
            consumers[i] = nullptr;
        }
    }

    /**
     * @brief Registers an event consumer callback interface.
     * @param[in] consumer Pointer to class implementing IEventConsumer.
     * @return true if added, false if max consumer capacity exceeded.
     */
    bool registerConsumer(IEventConsumer* consumer) {
        if (consumerCount >= MAX_CONSUMERS || consumer == nullptr) {
            return false;
        }
        consumers[consumerCount] = consumer;
        consumerCount++;
        return true;
    }

    /**
     * @brief Pushes an event to all registered consumer instances.
     * @param[in] event The event to distribute.
     */
    void dispatch(const Event& event) {
        for (uint8_t i = 0; i < consumerCount; ++i) {
            if (consumers[i] != nullptr) {
                consumers[i]->onEvent(event);
            }
        }
    }
};
