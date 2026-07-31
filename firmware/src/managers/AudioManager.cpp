/**
 * @file AudioManager.cpp
 * @brief Manages non-blocking melody sweeps driven by behavior states.
 * 
 * Responsibilities:
 * - Update buzzer steps.
 * 
 * TODO:
 * - [ ] Implement pitch sweeps.
 */

#include "AudioManager.h"

AudioManager::AudioManager(IBuzzer& outputBuzzer) : buzzer(outputBuzzer) {}

void AudioManager::updateExpression(const Expression& expr) {
    // Play active buzzer tone sweeps matching target SoundEffect
    (void)expr;
}

void AudioManager::tick(uint32_t dtMs) {
    if (!isPlaying) return;

    stepStartTimeMs += dtMs;
    if (stepStartTimeMs >= currentStepDurationMs) {
        currentStepIndex++;
        if (currentStepIndex >= totalSteps) {
            buzzer.stopTone();
            isPlaying = false;
        } else {
            // TODO: Play next step in melody array
            stepStartTimeMs = 0;
        }
    }
}

void AudioManager::onEvent(const Event& event) {
    // Placeholder to respond to semantic events and trigger chirp soundscapes on buzzer
    (void)event;
}
