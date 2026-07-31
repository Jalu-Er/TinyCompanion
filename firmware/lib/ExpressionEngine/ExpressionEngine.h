/**
 * @file ExpressionEngine.h
 * @brief Converts internal emotional variables into an abstract companion expression.
 * 
 * Responsibilities:
 * - Map continuous Valence-Arousal values to discrete visual eye parameters.
 * - Map states and sleep levels to blinking rates and aura patterns.
 */

#pragma once
#include "Expression.h"
#include "../EmotionEngine/EmotionState.h"
#include "../StateMachine/CompanionState.h"

class ExpressionEngine {
public:
    ExpressionEngine() = default;

    /**
     * @brief Evaluates emotion scales and outputs abstract Expression configurations.
     * @param[in] emotion Reference to active Valence-Arousal scales.
     * @param[in] state Active behavioral status.
     */
    Expression calculateExpression(const EmotionState& emotion, CompanionState state);
};
