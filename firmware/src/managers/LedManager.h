/**
 * @file LedManager.h
 * @brief Manages PWM rgb indicators according to valence/arousal variables.
 * 
 * Responsibilities:
 * - Map active Valence and Arousal variables to target colors.
 * - Compute non-blocking color fades and breathing pulses.
 * 
 * TODO:
 * - [ ] Implement sine-wave breathing tables.
 */

#pragma once
#include "HAL/ILedAura.h"
#include "EventSystem/IEventConsumer.h"
#include "ExpressionEngine/Expression.h"

class LedManager : public IEventConsumer {
private:
    ILedAura& led;
    
    int8_t currentValence = 0;
    int8_t currentArousal = 0;
    
    uint16_t pulseTimerMs = 0;

public:
    LedManager(ILedAura& outputLed);
    
    /**
     * @brief Pushes new emotional state markers to transition LED fades.
     * @param[in] valence Happiness scale.
     * @param[in] arousal Energy/excitement scale.
     */
    void setEmotion(int8_t valence, int8_t arousal);
    
    /**
     * @brief Computes dynamic aura fades (e.g. breathing effect). Called at Logic frequency.
     * @param[in] dtMs Elapsed duration since last execution.
     */
    void tick(uint32_t dtMs);

    // IEventConsumer implementation
    void onEvent(const Event& event) override;

    /**
     * @brief Resolves target led aura configurations directly from abstract expressions.
     */
    void updateExpression(const Expression& expr);
};
