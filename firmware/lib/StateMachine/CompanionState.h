/**
 * @file CompanionState.h
 * @brief Companion primary behavior states declaration.
 */

#pragma once
#include <stdint.h>

enum class CompanionState : uint8_t {
    BOOT = 0,
    IDLE,
    SLEEPING,
    OBSERVING,
    HAPPY,
    THINKING,
    ALERT,
    ERROR_STATE
};
