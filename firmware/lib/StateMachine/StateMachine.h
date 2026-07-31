/**
 * @file StateMachine.h
 * @brief High-level Finite State Machine (FSM) manager.
 * 
 * Responsibilities:
 * - Manage transitions between companion states (Boot, Idle, Observing, etc.).
 * - Implement StateContext interface for C++ State Pattern.
 * - Broadcast STATE_CHANGED events upon transiting.
 */

#pragma once
#include "StateContext.h"
#include "CompanionState.h"
#include "../EventSystem/Event.h"
#include "../EventSystem/EventQueue.h"

class IState; // Forward declaration

class StateMachine : public StateContext {
private:
    IState* currentState;
    EventQueue& eventQueue;

public:
    /**
     * @brief Constructs FSM and binds EventQueue output channel.
     */
    StateMachine(EventQueue& queue);

    /**
     * @brief Boots FSM to target initial Boot State.
     */
    void begin();

    /**
     * @brief Passes incoming event trigger down to the active State class.
     * @param[in] event Struct event.
     */
    void processEvent(const Event& event);

    /**
     * @brief Periodically ticks the active State's internal timers.
     * @param[in] dtMs Time elapsed in milliseconds.
     */
    void tick(uint32_t dtMs);

    /**
     * @brief Concrete implementation of StateContext transition contract.
     * @param[in] nextState Target behavior state enum index.
     */
    void transitionTo(CompanionState nextState) override;

    /**
     * @brief Read current state enum identifier.
     */
    CompanionState getCurrentState() const;
};
