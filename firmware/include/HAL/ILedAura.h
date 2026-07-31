/**
 * @file ILedAura.h
 * @brief Hardware Abstraction Layer interface for RGB emotional indicator lights.
 * 
 * Responsibilities:
 * - Abstract hardware control of three individual PWM output channels (R, G, B).
 * - Handle color channel mapping without application layout awareness.
 * 
 * TODO:
 * - [ ] Implement concrete PWM fading mapping on Uno pins 11, 12, 13.
 */

#pragma once
#include <stdint.h>

class ILedAura {
public:
    virtual ~ILedAura() {}
    
    /**
     * @brief Set specific values for RGB color intensities.
     * @param[in] r Red intensity value (0-255).
     * @param[in] g Green intensity value (0-255).
     * @param[in] b Blue intensity value (0-255).
     */
    virtual void setColor(uint8_t r, uint8_t g, uint8_t b) = 0;
};
