/**
 * @file GazeController.cpp
 * @brief Procedural OLED 2D gaze/pupil movement overlay controller implementation.
 */

#include "GazeController.h"

GazeController::GazeController(LcgPrng& prngInstance)
    : prng(prngInstance),
      state(GazeState::CENTER_HOLD),
      lastStateChangeMs(0),
      nextStateDurationMs(3000),
      currentX(0),
      currentY(0),
      startX(0),
      startY(0),
      targetX(0),
      targetY(0) {}

void GazeController::getCoordinates(GazePosition pos, int8_t& outX, int8_t& outY) const {
    switch (pos) {
        case GazePosition::LEFT:
            outX = -OFFSET_LIMIT_X;
            outY = 0;
            break;
        case GazePosition::RIGHT:
            outX = OFFSET_LIMIT_X;
            outY = 0;
            break;
        case GazePosition::UP:
            outX = 0;
            outY = -OFFSET_LIMIT_Y;
            break;
        case GazePosition::DOWN:
            outX = 0;
            outY = OFFSET_LIMIT_Y;
            break;
        default: // GazePosition::CENTER
            outX = 0;
            outY = 0;
            break;
    }
}

void GazeController::transitionTo(GazeState newState, uint32_t duration, uint32_t now) {
    state = newState;
    lastStateChangeMs = now;
    nextStateDurationMs = duration;
    startX = currentX;
    startY = currentY;
}

void GazeController::tick(uint32_t now, EyeShape currentShape) {
    bool isErrorShape = (static_cast<uint8_t>(currentShape) > 6);
    if (currentShape == EyeShape::SLEEPY || isErrorShape) {
        state = GazeState::CENTER_HOLD;
        currentX = 0;
        currentY = 0;
        startX = 0;
        startY = 0;
        targetX = 0;
        targetY = 0;
        lastStateChangeMs = now;
        nextStateDurationMs = 3000;
        return;
    }

    uint32_t elapsed = now - lastStateChangeMs;

    switch (state) {
        case GazeState::CENTER_HOLD:
            currentX = 0;
            currentY = 0;
            if (elapsed >= nextStateDurationMs) {
                GazePosition targetPos = static_cast<GazePosition>(1 + (prng.next() % 4));
                getCoordinates(targetPos, targetX, targetY);

                uint32_t lookDuration = 200 + (prng.next() % 200);
                transitionTo(GazeState::LOOKING, lookDuration, now);
            }
            break;

        case GazeState::LOOKING:
            if (elapsed >= nextStateDurationMs) {
                currentX = targetX;
                currentY = targetY;
                
                uint32_t holdDuration = 800 + (prng.next() % 1200);
                transitionTo(GazeState::HOLDING, holdDuration, now);
            } else {
                int32_t deltaX = targetX - startX;
                currentX = startX + (deltaX * static_cast<int32_t>(elapsed)) / static_cast<int32_t>(nextStateDurationMs);

                int32_t deltaY = targetY - startY;
                currentY = startY + (deltaY * static_cast<int32_t>(elapsed)) / static_cast<int32_t>(nextStateDurationMs);
            }
            break;

        case GazeState::HOLDING:
            currentX = targetX;
            currentY = targetY;
            if (elapsed >= nextStateDurationMs) {
                targetX = 0;
                targetY = 0;
                uint32_t returnDuration = 150 + (prng.next() % 200);
                transitionTo(GazeState::RETURNING, returnDuration, now);
            }
            break;

        case GazeState::RETURNING:
            if (elapsed >= nextStateDurationMs) {
                currentX = 0;
                currentY = 0;

                uint32_t centerHoldDuration = 1500 + (prng.next() % 2500);
                transitionTo(GazeState::CENTER_HOLD, centerHoldDuration, now);
            } else {
                int32_t deltaX = targetX - startX;
                currentX = startX + (deltaX * static_cast<int32_t>(elapsed)) / static_cast<int32_t>(nextStateDurationMs);

                int32_t deltaY = targetY - startY;
                currentY = startY + (deltaY * static_cast<int32_t>(elapsed)) / static_cast<int32_t>(nextStateDurationMs);
            }
            break;
    }
}

Expression GazeController::applyOverlay(const Expression& base) const {
    Expression result = base;
    result.pupilOffsetX = currentX;
    result.pupilOffsetY = currentY;
    return result;
}

void GazeController::forceGaze(GazePosition pos, uint32_t durationMs, uint32_t now) {
    getCoordinates(pos, targetX, targetY);
    if (durationMs == 0) {
        currentX = targetX;
        currentY = targetY;
        transitionTo(GazeState::HOLDING, 1000, now);
    } else {
        transitionTo(GazeState::LOOKING, durationMs, now);
    }
}

GazeState GazeController::getState() const {
    return state;
}

void GazeController::getActiveOffsets(int8_t& outX, int8_t& outY) const {
    outX = currentX;
    outY = currentY;
}
