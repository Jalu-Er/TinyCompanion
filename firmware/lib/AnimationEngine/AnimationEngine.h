/**
 * @file AnimationEngine.h
 * @brief Procedural calculations for eye parameters and dynamic transition interpolation.
 * 
 * Responsibilities:
 * - Calculate coordinates of pupils and eyelids.
 * - Perform non-blocking interpolation (lerp) toward target expressions.
 * - Fire blink timings and look saccades.
 * 
 * TODO:
 * - [ ] Implement procedural integer lerp maths to prevent floating point operations (Uno performance optimization).
 * - [ ] Implement eye saccade timing generators.
 */

#pragma once
#include <stdint.h>
#include "../EmotionEngine/EmotionEngine.h"

struct EyeParameters {
    int8_t leftPupilX;   // Offset X from center
    int8_t leftPupilY;   // Offset Y from center
    int8_t rightPupilX;
    int8_t rightPupilY;
    uint8_t eyeSize;     // Base circle diameter
    uint8_t upperLid;    // Lid closure percentage [0-100]
    uint8_t lowerLid;    // Lid closure percentage [0-100]
    int8_t eyeRotation;  // Rotation in degrees
};

class AnimationEngine {
private:
    EyeParameters currentParameters;
    EyeParameters targetParameters;

public:
    AnimationEngine() {
        // Initialize neutral state parameters
        currentParameters = {0, 0, 0, 0, 16, 0, 0, 0};
        targetParameters = currentParameters;
    }

    /**
     * @brief Update the target expression matching the new emotion.
     */
    void setEmotion(DiscreteEmotion emotion) {
        // TODO: Map discrete emotion categories to specific target parameters
        (void)emotion;
    }

    /**
     * @brief Dynamic frame update tick (runs at 30 FPS / ~33ms).
     * @param[in] dtMs Delta milliseconds since last frame update.
     */
    void tick(uint32_t dtMs) {
        // TODO: Interpolate current parameters to target parameters
        (void)dtMs;
    }

    /**
     * @brief Expose the current eye draw parameters.
     */
    const EyeParameters& getEyeParameters() const { return currentParameters; }
};
