/**
 * @file GazeController.h
 * @brief Procedural OLED 2D gaze/pupil movement overlay controller.
 * 
 * Responsibilities:
 * - Manage autonomous non-blocking gaze states (CENTER_HOLD, LOOKING, HOLDING, RETURNING).
 * - Schedule randomized hold periods and movement transitions using shared LcgPrng.
 * - Interpolate pupilOffsetX/Y coordinates linearly over time.
 * - Suppress gaze behavior for Sleeping and Error expressions.
 */

#pragma once
#include <stdint.h>
#include "ExpressionEngine/Expression.h"
#include "LcgPrng.h"

enum class GazeState : uint8_t {
    CENTER_HOLD = 0,
    LOOKING,
    HOLDING,
    RETURNING
};

enum class GazePosition : uint8_t {
    CENTER = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class GazeController {
private:
    LcgPrng& prng;
    GazeState state;
    uint32_t lastStateChangeMs;
    uint32_t nextStateDurationMs;

    // Current and target position coordinates
    int8_t currentX;
    int8_t currentY;
    int8_t startX;
    int8_t startY;
    int8_t targetX;
    int8_t targetY;

    // Movement limits
    static constexpr int8_t OFFSET_LIMIT_X = 4;
    static constexpr int8_t OFFSET_LIMIT_Y = 3;

    /**
     * @brief Resolves target coordinates based on GazePosition enum.
     */
    void getCoordinates(GazePosition pos, int8_t& outX, int8_t& outY) const;

    /**
     * @brief Transition to a new gaze state and reset timers.
     */
    void transitionTo(GazeState newState, uint32_t duration, uint32_t now);

public:
    GazeController(LcgPrng& prngInstance);

    /**
     * @brief Updates the internal gaze state machine timeline.
     * @param[in] now Current system time index in milliseconds.
     * @param[in] currentShape Shape of active render frame.
     */
    void tick(uint32_t now, EyeShape currentShape);

    /**
     * @brief Overlays gaze coordinates over base expression without mutation.
     * @param[in] base Stored base expression.
     * @return New Expression with overlaid pupilOffsetX and pupilOffsetY.
     */
    Expression applyOverlay(const Expression& base) const;

    /**
     * @brief Sets manual gaze target for deterministic testing.
     */
    void forceGaze(GazePosition pos, uint32_t durationMs, uint32_t now);

    /**
     * @brief Query active internal gaze state.
     */
    GazeState getState() const;

    /**
     * @brief Query active coordinates.
     */
    void getActiveOffsets(int8_t& outX, int8_t& outY) const;
};
