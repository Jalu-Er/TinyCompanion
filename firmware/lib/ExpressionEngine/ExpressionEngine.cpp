/**
 * @file ExpressionEngine.cpp
 * @brief Implementations of the emotional expression mapper.
 */

#include "ExpressionEngine.h"

Expression ExpressionEngine::calculateExpression(const EmotionState& emotion, CompanionState state) {
    Expression expr;
    
    // 1. Establish basic baseline values
    expr.eyeShape = EyeShape::NORMAL;
    expr.pupilRadius = 5;
    expr.eyelidOpen = 100;
    expr.blinkIntervalS = 4;
    expr.aura = AuraState::Idle;
    expr.sound = SoundEffect::NONE;

    // 2. Absolute FSM State Dominance overrides
    if (state == CompanionState::BOOT) {
        expr.eyelidOpen = 0;
        expr.aura = AuraState::Thinking;
        return expr;
    }
    
    if (state == CompanionState::SLEEPING) {
        expr.eyeShape = EyeShape::SLEEPY;
        expr.eyelidOpen = 0;
        expr.blinkIntervalS = 0; // No blinks during sleep
        expr.aura = AuraState::Sleeping;
        expr.sound = SoundEffect::SLEEP_SNORE;
        return expr;
    }
    
    if (state == CompanionState::ERROR_STATE) {
        expr.eyeShape = EyeShape::SAD;
        expr.eyelidOpen = 40;
        expr.aura = AuraState::Error;
        expr.sound = SoundEffect::ERROR_ALARM;
        return expr;
    }

    // 3. Continuous valence/arousal dynamic mappings
    if (emotion.valence > 40) {
        expr.eyeShape = EyeShape::HAPPY;
        expr.aura = AuraState::Happy;
        expr.blinkIntervalS = 3; // Blinks slightly faster when happy
        
        // High excitement triggers laugh sounds
        if (emotion.arousal > 30) {
            expr.sound = SoundEffect::HAPPY_GIGGLE;
        } else {
            expr.sound = SoundEffect::CHIRP;
        }
    } 
    else if (emotion.valence < -40) {
        expr.eyeShape = EyeShape::SAD;
        expr.eyelidOpen = 70; // Eyes slightly drooped
        
        if (emotion.arousal > 40) {
            expr.eyeShape = EyeShape::ANGRY;
            expr.aura = AuraState::Alert;
        } else {
            expr.sound = SoundEffect::SAD_WHIMPER;
        }
    } 
    else if (emotion.arousal > 50) {
        expr.eyeShape = EyeShape::ALERT;
        expr.pupilRadius = 8; // Dilated pupils under shock/alarm
        expr.aura = AuraState::Alert;
        expr.sound = SoundEffect::ALERT_CHIME;
    } 
    else if (emotion.curiosity > 70) {
        expr.eyeShape = EyeShape::SQUINT;
        expr.pupilRadius = 3; // Contracted focus pupils
        expr.aura = AuraState::Thinking;
    } 
    else if (emotion.fatigue > 70) {
        expr.eyeShape = EyeShape::SLEEPY;
        expr.eyelidOpen = 30; // Half-closed heavy eyelids
        expr.blinkIntervalS = 6; // Slow lazy blinking
    }

    return expr;
}
