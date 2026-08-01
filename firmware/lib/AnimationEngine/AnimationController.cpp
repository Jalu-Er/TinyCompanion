/**
 * @file AnimationController.cpp
 * @brief Tweening layer for continuous Expression variables.
 */

#include "AnimationController.h"

AnimationController::AnimationController()
    : startTimeMs(0), durationMs(0), isAnimatingFlag(false) {
    currentExpr.eyeShape = EyeShape::NORMAL;
    currentExpr.pupilRadius = 5;
    currentExpr.eyelidOpen = 100;
    currentExpr.blinkIntervalS = 4;
    currentExpr.aura = AuraState::Idle;
    currentExpr.sound = SoundEffect::NONE;

    targetExpr = currentExpr;
    startExpr = currentExpr;
}

void AnimationController::setTarget(const Expression& target, uint32_t duration, uint32_t now) {
    // Prevent animation restart if the target is identical to active target
    if (targetExpr.eyeShape == target.eyeShape &&
        targetExpr.pupilRadius == target.pupilRadius &&
        targetExpr.eyelidOpen == target.eyelidOpen &&
        targetExpr.aura == target.aura &&
        targetExpr.sound == target.sound &&
        isAnimatingFlag) {
        return;
    }

    targetExpr = target;
    startExpr = currentExpr; // Smooth interruption
    startTimeMs = now;
    durationMs = duration;

    // Discrete state variables update instantly at start of transition
    currentExpr.eyeShape = target.eyeShape;
    currentExpr.aura = target.aura;
    currentExpr.sound = target.sound;
    currentExpr.blinkIntervalS = target.blinkIntervalS;

    if (durationMs == 0) {
        currentExpr.pupilRadius = target.pupilRadius;
        currentExpr.eyelidOpen = target.eyelidOpen;
        isAnimatingFlag = false;
    } else {
        isAnimatingFlag = true;
    }
}

void AnimationController::tick(uint32_t now) {
    if (!isAnimatingFlag) {
        return;
    }

    uint32_t elapsed = now - startTimeMs;

    if (elapsed >= durationMs) {
        currentExpr.pupilRadius = targetExpr.pupilRadius;
        currentExpr.eyelidOpen = targetExpr.eyelidOpen;
        isAnimatingFlag = false;
    } else {
        // Safe integer-based linear interpolation
        int32_t deltaPupil = static_cast<int32_t>(targetExpr.pupilRadius) - startExpr.pupilRadius;
        currentExpr.pupilRadius = startExpr.pupilRadius + (deltaPupil * static_cast<int32_t>(elapsed)) / static_cast<int32_t>(durationMs);

        int32_t deltaEyelid = static_cast<int32_t>(targetExpr.eyelidOpen) - startExpr.eyelidOpen;
        currentExpr.eyelidOpen = startExpr.eyelidOpen + (deltaEyelid * static_cast<int32_t>(elapsed)) / static_cast<int32_t>(durationMs);
    }
}

const Expression& AnimationController::current() const {
    return currentExpr;
}

bool AnimationController::isAnimating() const {
    return isAnimatingFlag;
}
