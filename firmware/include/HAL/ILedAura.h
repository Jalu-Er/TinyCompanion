/**
 * @file ILedAura.h
 * @brief Hardware Abstraction Layer interface for RGB emotional indicator lights.
 * 
 * Responsibilities:
 * - Define predefined emotional aura states for ambient feedback.
 * - Supply an API expressing visual intent rather than direct pin control.
 */

#pragma once
#include <stdint.h>

enum class AuraState : uint8_t {
    Idle,
    Happy,
    Thinking,
    Sleeping,
    Alert,
    Error
};

class ILedAura {
public:
    virtual ~ILedAura() {}
    
    /**
     * @brief Transition the ambient lighting to a target emotional aura state.
     * @param[in] state The target aura state (Idle, Happy, Thinking, Sleeping, Alert, Error).
     */
    virtual void setAura(AuraState state) = 0;
    
    /**
     * @brief Turn off all light channels and reset state parameters.
     */
    virtual void clear() = 0;
    
    /**
     * @brief Execute periodic, non-blocking time checks to handle flashing or pulsing.
     */
    virtual void update() = 0;
};
