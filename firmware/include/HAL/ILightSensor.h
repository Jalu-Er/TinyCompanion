/**
 * @file ILightSensor.h
 * @brief Hardware Abstraction Layer interface for ambient light detection.
 * 
 * Responsibilities:
 * - Abstract analog sensor pins measuring surrounding luminosity.
 * - Return raw intensity ratings to allow logic threshold transitions.
 * 
 * TODO:
 * - [ ] Implement concrete adapter wrapping Arduino's analogRead() for the LDR pin.
 */

#pragma once
#include <stdint.h>

class ILightSensor {
public:
    virtual ~ILightSensor() {}
    
    /**
     * @brief Reads raw light levels from the sensor.
     * @return Ambient light values (higher indicates darker ambient settings).
     */
    virtual uint16_t getLightLevel() = 0;
};
