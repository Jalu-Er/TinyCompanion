/**
 * @file ITouchSensor.h
 * @brief Hardware Abstraction Layer interface for touch interaction detection.
 * 
 * Responsibilities:
 * - Abstract capacitive touch pin reading and raw electrical states.
 * - Expose basic presence or active touch condition checks.
 * 
 * TODO:
 * - [ ] Implement concrete class mapping to TTP223 digital pin.
 */

#pragma once

class ITouchSensor {
public:
    virtual ~ITouchSensor() {}
    
    /**
     * @brief Checks if the touch surface is currently stimulated.
     * @return true if touched, false otherwise.
     */
    virtual bool isTouched() = 0;
};
