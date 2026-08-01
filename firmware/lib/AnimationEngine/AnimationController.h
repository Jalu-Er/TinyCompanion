/**
 * @file AnimationController.h
 * @brief Tweening layer for continuous Expression variables.
 * 
 * Responsibilities:
 * - Handle transition interpolation (lerp) from start to target expressions.
 * - Perform calculations in standard C++ integer arithmetic (no floats).
 * - Decouple timing states from hardware components (testable offline).
 */

#pragma once
#include <stdint.h>
#include "ExpressionEngine/Expression.h"

class AnimationController {
private:
    Expression currentExpr;
    Expression targetExpr;
    Expression startExpr;

    uint32_t startTimeMs;
    uint32_t durationMs;
    bool isAnimatingFlag;

public:
    AnimationController();

    /**
     * @brief Sets target expression parameters and transition timeline.
     * @param[in] target Target expression state.
     * @param[in] duration Transition period in milliseconds.
     * @param[in] now Current system time index in milliseconds.
     */
    void setTarget(const Expression& target, uint32_t duration, uint32_t now);

    /**
     * @brief Computes interpolation step based on current tick timestamp.
     * @param[in] now Current system time index in milliseconds.
     */
    void tick(uint32_t now);

    /**
     * @brief Access the active interpolated expression status.
     * @return Reference to the current expression profile.
     */
    const Expression& current() const;

    /**
     * @brief Checks if transition animation is active.
     */
    bool isAnimating() const;
};
