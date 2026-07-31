/**
 * @file LedValidation.h
 * @brief Reusable verification module for validating RGB LED Aura states.
 * 
 * Responsibilities:
 * - Implement IValidation interface contract.
 * - Programmatically cycle through all emotional aura states (Idle, Happy, Thinking, Sleeping, Alert, Error).
 */

#pragma once
#include "HAL/ILedAura.h"
#include "validation/IValidation.h"

class LedValidation : public IValidation {
private:
    ILedAura& aura;

    // Helper to block execution while updating the blinking state non-blocking internally
    void runAuraDuration(AuraState state, uint32_t durationMs);

public:
    LedValidation(ILedAura& ledAura);

    void run() override;
    const char* name() const override;
    const char* description() const override;
};
