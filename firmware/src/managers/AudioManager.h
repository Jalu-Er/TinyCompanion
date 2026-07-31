/**
 * @file AudioManager.h
 * @brief Manages non-blocking melody sweeps driven by behavior states.
 * 
 * Responsibilities:
 * - Play short musical soundscapes using tone sequences.
 * - Manage active notes using cooperative tick sweeps to prevent delays.
 * 
 * TODO:
 * - [ ] Read melody arrays directly from PROGMEM flash definitions.
 */

#pragma once
#include "HAL/IBuzzer.h"
#include "StateMachine/State.h"
#include "EventSystem/IEventConsumer.h"

struct ToneStep {
    uint16_t frequencyHz;
    uint16_t durationMs;
};

class AudioManager : public IEventConsumer {
private:
    IBuzzer& buzzer;
    CompanionState activeState = CompanionState::IDLE;
    
    bool isPlaying = false;
    uint32_t stepStartTimeMs = 0;
    uint16_t currentStepDurationMs = 0;
    uint8_t currentStepIndex = 0;
    uint8_t totalSteps = 0;

public:
    AudioManager(IBuzzer& outputBuzzer);
    
    /**
     * @brief Triggers a transition melody matches.
     * @param[in] state New companion status.
     */
    void setState(CompanionState state);
    
    /**
     * @brief Ticks sound sweep playback. Called at 50 Hz.
     * @param[in] dtMs Time elapsed since last tick.
     */
    void tick(uint32_t dtMs);

    // IEventConsumer implementation
    void onEvent(const Event& event) override;
};
