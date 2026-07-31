/**
 * @file StateContext.h
 * @brief Abstract interface allowing states to trigger context transitions.
 * 
 * Responsibilities:
 * - Decouple specific State transitions from concrete StateMachine parameters.
 */

#pragma once
#include "CompanionState.h"

class StateContext {
public:
    virtual ~StateContext() {}

    /**
     * @brief Transitions the state machine context to a different state zone.
     * @param[in] nextState The target CompanionState enum value.
     */
    virtual void transitionTo(CompanionState nextState) = 0;
};
