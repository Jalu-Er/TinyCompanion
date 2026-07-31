/**
 * @file StateMachine.h
 * @brief High-level Finite State Machine (FSM) manager.
 * 
 * Responsibilities:
 * - Manage transitions between companion states (Idle, Sleeping, Scared).
 * - Process incoming events from the queue to trigger state changes.
 * - Call output hooks on state transition boundaries.
 * 
 * TODO:
 * - [ ] Implement state transition validation matrices.
 * - [ ] Coordinate with the EmotionEngine during state shifts.
 */

#pragma once
#include "State.h"
#include "../EventSystem/Event.h"
#include "../EmotionEngine/EmotionEngine.h"

class StateMachine {
private:
    CompanionState currentState = CompanionState::IDLE;
    EmotionEngine& emotionEngine;

public:
    StateMachine(EmotionEngine& emotion) : emotionEngine(emotion) {}

    /**
     * @brief Evaluates an incoming event and changes state if a valid transition condition matches.
     */
    void processEvent(const Event& event) {
        // TODO: Handle state transitions based on the EventType
        (void)event;
    }

    /**
     * @brief Periodic update loop tick for active state operations (e.g. idle timeout checks).
     * @param[in] dtMs Delta time in milliseconds since last update.
     */
    void tick(uint32_t dtMs) {
        // TODO: Update current state countdowns or state-based updates
        (void)dtMs;
    }

    /**
     * @brief Retrieve the current active state.
     */
    CompanionState getCurrentState() const { return currentState; }
};
