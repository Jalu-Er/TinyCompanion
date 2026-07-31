/**
 * @file SensorValidation.h
 * @brief Software verification module validating semantic sensor event generation.
 * 
 * Responsibilities:
 * - Implement IValidation interface contract.
 * - Poll SensorManager and dequeue generated semantic events.
 * - Print event names to the Serial port.
 */

#pragma once
#include "validation/IValidation.h"
#include "managers/SensorManager.h"
#include "EventSystem/EventQueue.h"

class SensorValidation : public IValidation {
private:
    SensorManager& sensorManager;
    EventQueue& eventQueue;

    // Helper to map EventType enums to user-readable strings
    const char* getEventName(EventType type) const;

public:
    SensorValidation(SensorManager& manager, EventQueue& queue);

    void run() override;
    const char* name() const override;
    const char* description() const override;
};
