/**
 * @file IState.h
 * @brief Abstract interface contract for concrete FSM state instances.
 * 
 * Responsibilities:
 * - Define standard life-cycles (onEnter, onExit, handleEvent, tick).
 */

#pragma once
#include "../EventSystem/Event.h"
#include "CompanionState.h"

class StateContext; // Forward declaration

class IState {
public:
    virtual ~IState() {}

    /**
     * @brief Called upon entering this state zone.
     */
    virtual void onEnter(StateContext& ctx) = 0;

    /**
     * @brief Called upon exiting this state zone.
     */
    virtual void onExit(StateContext& ctx) = 0;

    /**
     * @brief Resolves semantic events and schedules state transitions.
     */
    virtual void handleEvent(StateContext& ctx, const Event& event) = 0;

    /**
     * @brief Performs periodic logic ticks.
     */
    virtual void tick(StateContext& ctx, uint32_t dtMs) = 0;

    /**
     * @brief Retrieves the enum identifier corresponding to this state class.
     */
    virtual CompanionState getStateId() const = 0;
};
