/**
 * @file EmotionEngine.h
 * @brief Russell's 2D Valence-Arousal plane calculations and discrete emotion mapping.
 * 
 * Responsibilities:
 * - Maintain emotional variables (valence, arousal, trust, curiosity, fatigue).
 * - Process FSM state updates and semantic events to modify internal values.
 * - Calculate emotional decay over cooperative tick intervals.
 */

#pragma once
#include <stdint.h>
#include "EmotionState.h"
#include "../PersonalityEngine/PersonalityEngine.h"
#include "../StateMachine/CompanionState.h"
#include "../EventSystem/Event.h"

class EmotionEngine {
private:
    EmotionState currentEmotion;
    const PersonalityEngine& personality;

    // Helper to clamp values within boundaries without floating point
    int8_t clamp8(int16_t val, int8_t minVal, int8_t maxVal);
    uint8_t clampU8(int16_t val, uint8_t minVal, uint8_t maxVal);

public:
    EmotionEngine(const PersonalityEngine& personalityEngine);

    /**
     * @brief Modifies emotional state values based on event updates and current FSM state.
     * @param[in] state Current active FSM state code.
     * @param[in] event Struct event.
     */
    void processStateAndEvent(CompanionState state, const Event& event);

    /**
     * @brief Ticks emotional values decay and sleeping fatigue recovery.
     * @param[in] dtMs Cooperative task loop step duration in milliseconds.
     */
    void tick(uint32_t dtMs, CompanionState currentState);

    /**
     * @brief Read the current internal emotional variables state.
     */
    const EmotionState& getEmotionState() const;
};
