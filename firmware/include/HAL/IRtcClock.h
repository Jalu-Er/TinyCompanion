/**
 * @file IRtcClock.h
 * @brief Hardware Abstraction Layer interface for Real-Time Clock modules.
 * 
 * Responsibilities:
 * - Abstract clock updates from RTC devices (e.g., DS1307).
 * - Expose current datetime states without vendor library dependencies.
 * 
 * TODO:
 * - [ ] Implement concrete adapter integrating RTClib and DS1307.
 */

#pragma once
#include "Common/TimeTypes.h"

class IRtcClock {
public:
    virtual ~IRtcClock() {}
    
    /**
     * @brief Reads the current time and stores it in the provided struct.
     * @param[out] outTime Location to write the retrieved time metrics.
     * @return true if successful, false if RTC communication failed.
     */
    virtual bool getTime(TimeStruct& outTime) = 0;

    /**
     * @brief Set/adjust the internal RTC time.
     * @param[in] newTime Struct containing updated time attributes.
     * @return true on success, false on communication failure.
     */
    virtual bool setTime(const TimeStruct& newTime) = 0;
};
