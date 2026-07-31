/**
 * @file TimeTypes.h
 * @brief Common temporal structure representations used across logical modules.
 * 
 * Responsibilities:
 * - Define a platform-neutral datetime container.
 * - Supply utility layouts for simple scheduler comparisons.
 * 
 * TODO:
 * - [ ] Add day-of-week validation converters.
 */

#pragma once
#include <stdint.h>

struct TimeStruct {
    uint8_t hour;       // 0-23
    uint8_t minute;     // 0-59
    uint8_t second;     // 0-59
    uint8_t dayOfWeek;  // 1-7 (e.g. 1 = Sunday)
};
