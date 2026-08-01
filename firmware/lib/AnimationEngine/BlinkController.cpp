/**
 * @file BlinkController.cpp
 * @brief Procedural eye blink system overlay implementation.
 */

#include "BlinkController.h"

BlinkController::BlinkController(LcgPrng& prngInstance)
    : prng(prngInstance),
      state(BlinkState::IDLE),
      lastStateChangeMs(0),
      nextBlinkTimeMs(0),
      closingDurationMs(80),
      closedDurationMs(50),
      openingDurationMs(90),
      minIntervalMs(3000),
      maxIntervalMs(8000),
      isBlinkingFlag(false),
      currentBlinkFactor(100) {}

void BlinkController::scheduleNextBlink(uint32_t now) {
    uint32_t range = maxIntervalMs - minIntervalMs;
    uint32_t offset = prng.next() % range;
    nextBlinkTimeMs = now + minIntervalMs + offset;
}

void BlinkController::setSeed(uint32_t seed) {
    prng.setSeed(seed);
}

void BlinkController::forceBlink(uint32_t now) {
    if (isBlinkingFlag) {
        return; // Avoid corrupting active blink
    }
    state = BlinkState::CLOSING;
    lastStateChangeMs = now;
    isBlinkingFlag = true;
    currentBlinkFactor = 100;
}

void BlinkController::tick(uint32_t now, EyeShape currentShape) {
    // Suppress blinking for SLEEPY state or unrecognized states (Error fallback)
    bool isErrorShape = (static_cast<uint8_t>(currentShape) > 6);
    if (currentShape == EyeShape::SLEEPY || isErrorShape) {
        state = BlinkState::IDLE;
        isBlinkingFlag = false;
        currentBlinkFactor = 100;
        scheduleNextBlink(now);
        return;
    }

    if (nextBlinkTimeMs == 0) {
        scheduleNextBlink(now);
    }

    uint32_t elapsed = now - lastStateChangeMs;

    switch (state) {
        case BlinkState::IDLE:
            currentBlinkFactor = 100;
            if (static_cast<int32_t>(now - nextBlinkTimeMs) >= 0) {
                state = BlinkState::CLOSING;
                lastStateChangeMs = now;
                isBlinkingFlag = true;
            }
            break;

        case BlinkState::CLOSING:
            if (elapsed >= closingDurationMs) {
                state = BlinkState::CLOSED;
                lastStateChangeMs = now;
                currentBlinkFactor = 0;
            } else {
                currentBlinkFactor = 100 - (elapsed * 100) / closingDurationMs;
            }
            break;

        case BlinkState::CLOSED:
            currentBlinkFactor = 0;
            if (elapsed >= closedDurationMs) {
                state = BlinkState::OPENING;
                lastStateChangeMs = now;
            }
            break;

        case BlinkState::OPENING:
            if (elapsed >= openingDurationMs) {
                state = BlinkState::IDLE;
                lastStateChangeMs = now;
                isBlinkingFlag = false;
                currentBlinkFactor = 100;
                scheduleNextBlink(now);
            } else {
                currentBlinkFactor = (elapsed * 100) / openingDurationMs;
            }
            break;
    }
}

Expression BlinkController::applyOverlay(const Expression& base) const {
    Expression result = base;
    result.eyelidOpen = (static_cast<uint16_t>(base.eyelidOpen) * currentBlinkFactor) / 100;
    return result;
}

bool BlinkController::isBlinking() const {
    return isBlinkingFlag;
}

BlinkState BlinkController::getState() const {
    return state;
}

uint32_t BlinkController::getNextBlinkTime() const {
    return nextBlinkTimeMs;
}
