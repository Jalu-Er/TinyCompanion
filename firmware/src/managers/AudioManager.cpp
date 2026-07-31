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

void AudioManager::setState(CompanionState state) {
    if (state != activeState) {
        activeState = state;
        // TODO: Load matching melody steps based on the current state transition
    }
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
