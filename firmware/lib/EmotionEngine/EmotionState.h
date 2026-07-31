/**
 * @file EmotionState.h
 * @brief Definition of the internal emotional variables state.
 */

#pragma once
#include <stdint.h>

struct EmotionState {
    int8_t valence;     // Bounded [-100 to 100]
    int8_t arousal;     // Bounded [-100 to 100]
    uint8_t trust;      // Bounded [0 to 100]
    uint8_t curiosity;  // Bounded [0 to 100]
    uint8_t fatigue;    // Bounded [0 to 100]
};
