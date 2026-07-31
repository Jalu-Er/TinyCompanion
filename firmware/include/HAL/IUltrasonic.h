/**
 * @file IUltrasonic.h
 * @brief Hardware Abstraction Layer interface for proximity distance measuring.
 * 
 * Responsibilities:
 * - Abstract the physical ultrasonic ping trigger and calculation mechanics.
 * - Provide a consistent, non-blocking distance measurement query.
 * 
 * TODO:
 * - [ ] Implement non-blocking timer/interrupt measurements in concrete adapter.
 */

#pragma once
#include <stdint.h>

class IUltrasonic {
public:
    virtual ~IUltrasonic() {}
    
    /**
     * @brief Measures distance to the nearest target.
     * @return Distance in centimeters, or 0xFFFF if sensor times out / out of range.
     */
    virtual uint16_t getDistanceCm() = 0;
};
