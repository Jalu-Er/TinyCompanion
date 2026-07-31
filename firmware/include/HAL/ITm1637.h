/**
 * @file ITm1637.h
 * @brief Hardware Abstraction Layer interface for TM1637 4-digit display output.
 * 
 * Responsibilities:
 * - Abstract multi-segment output routines for numeric display.
 * - Provide basic time colon toggle interface hooks.
 * 
 * TODO:
 * - [ ] Implement concrete TM1637 segment logic mapping values to physical digits.
 */

#pragma once
#include <stdint.h>

class ITm1637 {
public:
    virtual ~ITm1637() {}
    
    /**
     * @brief Displays a four digit integer on the segment screen.
     * @param[in] value Integer range to show [0-9999].
     * @param[in] showColon Toggle status of the center time colon.
     */
    virtual void displayInteger(uint16_t value, bool showColon) = 0;

    /**
     * @brief Blanks/turns off the display digits.
     */
    virtual void clear() = 0;
};
