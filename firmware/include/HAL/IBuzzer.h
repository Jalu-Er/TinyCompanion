/**
 * @file IBuzzer.h
 * @brief Hardware Abstraction Layer interface for passive buzzer sound systems.
 * 
 * Responsibilities:
 * - Abstract PWM timers generating audio pitch tones.
 * - Supply non-blocking control contracts for sound frequencies and silence triggers.
 * 
 * TODO:
 * - [ ] Implement concrete tone generators using Uno hardware PWM timers or tone() utilities.
 */

#pragma once
#include <stdint.h>

class IBuzzer {
public:
    virtual ~IBuzzer() {}
    
    /**
     * @brief Generates a frequency signal on the buzzer pin.
     * @param[in] frequencyHz Audio frequency in Hertz.
     */
    virtual void playTone(uint16_t frequencyHz) = 0;
    
    /**
     * @brief Silences the buzzer.
     */
    virtual void stopTone() = 0;
};
