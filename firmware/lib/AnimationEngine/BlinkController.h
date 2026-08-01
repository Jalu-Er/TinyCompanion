/**
 * @file BlinkController.h
 * @brief Procedural eye blink system overlay for visual expression layer.
 * 
 * Responsibilities:
 * - Manage autonomous non-blocking blink states (IDLE, CLOSING, CLOSED, OPENING).
 * - Schedule pseudo-random blink intervals using a lightweight internal PRNG.
 * - Suppress blinks for specific expressions (e.g. Sleeping/Error).
 * - Expose deterministic testing hooks (forcing blinks, seed injection).
 */

#pragma once
#include <stdint.h>
#include "ExpressionEngine/Expression.h"

#include "LcgPrng.h"

enum class BlinkState : uint8_t {
    IDLE = 0,
    CLOSING,
    CLOSED,
    OPENING
};

class BlinkController {
private:
    LcgPrng& prng;
    BlinkState state;
    uint32_t lastStateChangeMs;
    uint32_t nextBlinkTimeMs;

    // Timing parameters in milliseconds
    uint32_t closingDurationMs;
    uint32_t closedDurationMs;
    uint32_t openingDurationMs;
    uint32_t minIntervalMs;
    uint32_t maxIntervalMs;

    bool isBlinkingFlag;
    uint8_t currentBlinkFactor;

    /**
     * @brief Schedules the timestamp index for the next procedural blink.
     * @param[in] now Current time index in milliseconds.
     */
    void scheduleNextBlink(uint32_t now);

public:
    BlinkController(LcgPrng& prngInstance);

    /**
     * @brief Sets seed for LCG random generator to ensure deterministic test runs.
     */
    void setSeed(uint32_t seed);

    /**
     * @brief Triggers an immediate blink execution.
     * @param[in] now Current time index in milliseconds.
     */
    void forceBlink(uint32_t now);

    /**
     * @brief Updates the internal blink state machine timeline.
     * @param[in] now Current system time index in milliseconds.
     * @param[in] currentShape Eshape of active render frame.
     */
    void tick(uint32_t now, EyeShape currentShape);

    /**
     * @brief Overlays blink state calculations over base expression without mutation.
     * @param[in] base Stored base expression from normal transitions.
     * @return Overlaid expression to pass to the renderer.
     */
    Expression applyOverlay(const Expression& base) const;

    /**
     * @brief Query whether eye blink is currently active.
     */
    bool isBlinking() const;

    /**
     * @brief Query active internal blink state machine enum.
     */
    BlinkState getState() const;

    /**
     * @brief Query scheduled timestamp for the next blink.
     */
    uint32_t getNextBlinkTime() const;
};
