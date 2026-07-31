/**
 * @file EmotionEngine.cpp
 * @brief Implementations of the valence-arousal emotional model calculations.
 */

#include "EmotionEngine.h"

EmotionEngine::EmotionEngine(const PersonalityEngine& personalityEngine)
    : personality(personalityEngine) {
    currentEmotion.valence = 0;
    currentEmotion.arousal = 0;
    currentEmotion.trust = 20;      // Starts with cautious trust
    currentEmotion.curiosity = 50;  // Moderate initial interest
    currentEmotion.fatigue = 0;     // Fully rested on boot
}

int8_t EmotionEngine::clamp8(int16_t val, int8_t minVal, int8_t maxVal) {
    if (val < minVal) return minVal;
    if (val > maxVal) return maxVal;
    return static_cast<int8_t>(val);
}

uint8_t EmotionEngine::clampU8(int16_t val, uint8_t minVal, uint8_t maxVal) {
    if (val < minVal) return minVal;
    if (val > maxVal) return maxVal;
    return static_cast<uint8_t>(val);
}

void EmotionEngine::processStateAndEvent(CompanionState state, const Event& event) {
    const PersonalityProfile& profile = personality.getProfile();

    // exScale adjust responsiveness based on excitability scale
    uint8_t exScale = profile.excitability / 10; // Ex: 40/10 = 4 multiplier scaling
    uint8_t friendScale = profile.friendliness / 10;

    switch (event.type) {
        case EventType::TOUCH_PRESSED:
            currentEmotion.valence = clamp8(currentEmotion.valence + (5 * friendScale), -100, 100);
            currentEmotion.arousal = clamp8(currentEmotion.arousal + (3 * exScale), -100, 100);
            currentEmotion.trust = clampU8(currentEmotion.trust + 5, 0, 100);
            break;
            
        case EventType::USER_APPROACHING:
            currentEmotion.curiosity = clampU8(currentEmotion.curiosity + 15, 0, 100);
            currentEmotion.arousal = clamp8(currentEmotion.arousal + (4 * exScale), -100, 100);
            break;
            
        case EventType::USER_LEAVING:
            currentEmotion.curiosity = clampU8(currentEmotion.curiosity - 10, 0, 100);
            currentEmotion.arousal = clamp8(currentEmotion.arousal - (2 * exScale), -100, 100);
            break;
            
        case EventType::AMBIENT_DARK:
            // Darkness increases sleepiness
            currentEmotion.fatigue = clampU8(currentEmotion.fatigue + 20, 0, 100);
            currentEmotion.arousal = clamp8(currentEmotion.arousal - 15, -100, 100);
            break;

        case EventType::AMBIENT_BRIGHT:
            currentEmotion.arousal = clamp8(currentEmotion.arousal + 10, -100, 100);
            break;

        default:
            break;
    }

    // Override baseline states based on FSM boundaries
    if (state == CompanionState::SLEEPING) {
        currentEmotion.arousal = clamp8(currentEmotion.arousal - 10, -100, 100);
        currentEmotion.curiosity = clampU8(currentEmotion.curiosity - 5, 0, 100);
    } else if (state == CompanionState::ALERT) {
        currentEmotion.arousal = clamp8(currentEmotion.arousal + 20, -100, 100);
        currentEmotion.curiosity = clampU8(currentEmotion.curiosity + 10, 0, 100);
    }
}

void EmotionEngine::tick(uint32_t dtMs, CompanionState currentState) {
    const PersonalityProfile& profile = personality.getProfile();
    
    // Recovery multiplier based on recovery scale profile
    uint8_t decaySpeed = profile.recoveryRate / 10; // Ex: 30/10 = 3 units per step
    if (decaySpeed == 0) decaySpeed = 1;

    // 1. Decay valence toward baseline 0 (neutral)
    if (currentEmotion.valence > 0) {
        currentEmotion.valence = clamp8(currentEmotion.valence - decaySpeed, 0, 100);
    } else if (currentEmotion.valence < 0) {
        currentEmotion.valence = clamp8(currentEmotion.valence + decaySpeed, -100, 0);
    }

    // 2. Decay arousal toward baseline 0 (neutral)
    if (currentEmotion.arousal > 0) {
        currentEmotion.arousal = clamp8(currentEmotion.arousal - decaySpeed, 0, 100);
    } else if (currentEmotion.arousal < 0) {
        currentEmotion.arousal = clamp8(currentEmotion.arousal + decaySpeed, -100, 0);
    }

    // 3. Accumulate fatigue over time when awake, recover when asleep
    if (currentState == CompanionState::SLEEPING) {
        // Recover fatigue: -10 fatigue per second
        uint16_t recoveryVal = (dtMs * 10) / 1000;
        if (recoveryVal == 0 && dtMs >= 100) recoveryVal = 1;
        currentEmotion.fatigue = clampU8(currentEmotion.fatigue - recoveryVal, 0, 100);
    } else {
        // Sleepiness increases over time: +1 fatigue per 10 seconds
        uint16_t fatigueVal = dtMs / 10000;
        if (fatigueVal > 0) {
            currentEmotion.fatigue = clampU8(currentEmotion.fatigue + fatigueVal, 0, 100);
        }
    }
}

const EmotionState& EmotionEngine::getEmotionState() const {
    return currentEmotion;
}
