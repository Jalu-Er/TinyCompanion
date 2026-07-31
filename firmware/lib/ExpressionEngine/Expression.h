/**
 * @file Expression.h
 * @brief Definition of the abstract companion expression model.
 */

#pragma once
#include <stdint.h>
#include "HAL/ILedAura.h" // For AuraState enum mapping

enum class EyeShape : uint8_t {
    NORMAL = 0,
    HAPPY,
    SAD,
    ANGRY,
    SLEEPY,
    ALERT,
    SQUINT
};

enum class SoundEffect : uint8_t {
    NONE = 0,
    CHIRP,
    HAPPY_GIGGLE,
    SAD_WHIMPER,
    SLEEP_SNORE,
    ALERT_CHIME,
    ERROR_ALARM
};

struct Expression {
    EyeShape eyeShape;
    uint8_t pupilRadius;    // Abstract pupil radius size scale [0 to 10]
    uint8_t eyelidOpen;     // Eyelid open percentage [0 to 100]
    uint8_t blinkIntervalS; // Scheduled blink loop frequency in seconds
    AuraState aura;
    SoundEffect sound;
};
