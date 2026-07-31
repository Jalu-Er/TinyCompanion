/**
 * @file IEventConsumer.h
 * @brief Abstract interface contract for application event consumers.
 * 
 * Responsibilities:
 * - Define virtual onEvent callback structure.
 */

#pragma once
#include "Event.h"

class IEventConsumer {
public:
    virtual ~IEventConsumer() {}

    /**
     * @brief Triggered by the EventDispatcher when an event is processed.
     * @param[in] event Struct containing event type and metadata.
     */
    virtual void onEvent(const Event& event) = 0;
};
