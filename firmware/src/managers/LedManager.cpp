/**
 * @file LedManager.cpp
 * @brief Manages PWM rgb indicators according to valence/arousal variables.
 * 
 * Responsibilities:
 * - Calculate RGB breathing values.
 * 
 * TODO:
 * - [ ] Implement color mixing interpolators.
 */

#include "LedManager.h"

LedManager::LedManager(ILedAura& outputLed) : led(outputLed) {}

void LedManager::setEmotion(int8_t valence, int8_t arousal) {
    currentValence = valence;
    currentArousal = arousal;
}

void LedManager::tick(uint32_t dtMs) {
    pulseTimerMs += dtMs;
    // TODO: Compute sine-wave breathing intensity scaling based on valence/arousal color targets
    (void)dtMs;
}
