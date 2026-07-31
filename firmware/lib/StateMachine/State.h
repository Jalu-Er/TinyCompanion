/**
 * @file State.h
 * @brief Behavior state definitions.
 * 
 * Responsibilities:
 * - Define the list of primary companion behavior states.
 * 
 * TODO:
 * - [ ] Ensure all states match target animation profiles.
 */

#pragma once
#include <stdint.h>

enum class CompanionState : uint8_t {
    SLEEPING = 0,
    WAKING_UP,
    IDLE,
    INTERACTIVE,
    SCARED_ALERT,
    LOW_POWER
};
