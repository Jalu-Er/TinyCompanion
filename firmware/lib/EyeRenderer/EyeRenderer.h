/**
 * @file EyeRenderer.h
 * @brief OLED Eye Renderer Core. translates abstract expressions to visual eye geometries.
 * 
 * Responsibilities:
 * - Define static layout and coordinate metrics (no hardcoded numbers).
 * - Render shapes (Neutral, Happy, Thinking, Alert, Sleeping, Error) using standard primitives.
 * - Manage eyelid clipping calculations deterministically.
 */

#pragma once
#include "HAL/IOledDisplay.h"
#include "ExpressionEngine/Expression.h"

// Layout configurations centralizing all rendering metrics (no hardcoding elsewhere)
struct EyeConfig {
    static constexpr int16_t CENTER_Y = 32;
    static constexpr int16_t LEFT_CENTER_X = 38;
    static constexpr int16_t RIGHT_CENTER_X = 90;
    
    static constexpr uint8_t DEFAULT_WIDTH = 26;
    static constexpr uint8_t DEFAULT_HEIGHT = 34;
    static constexpr uint8_t CORNER_RADIUS = 6;

    static constexpr int16_t HAPPY_Y_OFFSET = 8;
    static constexpr uint8_t HAPPY_PUPIL_ADD = 3;
    static constexpr int16_t SAD_Y_OFFSET = 4;
    static constexpr int16_t ANGRY_Y_OFFSET = 4;
    static constexpr uint8_t ERROR_CROSS_SIZE = 8;
};

// Geometry profiles computed once per render pass to prevent redundant calculations
struct SingleEyeGeometry {
    int16_t x;             // Left-top bounding box X
    int16_t y;             // Left-top bounding box Y
    int16_t cx;            // Center X
    int16_t cy;            // Center Y
    uint8_t w;             // Target width
    uint8_t h;             // Target height
    uint8_t r;             // Corner radius
    uint8_t pupilR;        // Adjusted pupil radius
    uint8_t closedH;       // Calculated height of closed lid
    int16_t pupilCx;       // Pupil center X
    int16_t pupilCy;       // Pupil center Y
};

struct EyeGeometry {
    SingleEyeGeometry left;
    SingleEyeGeometry right;
};


class EyeRenderer {
private:
    IOledDisplay& display;

    /**
     * @brief Computes all coordinates once based on expression parameters.
     * @param[in] expr Active expression parameter.
     * @return Fully calculated EyeGeometry layout structure.
     */
    EyeGeometry calculateGeometry(const Expression& expr);

    /**
     * @brief Draws a single eye shape on the display buffer using computed geometry.
     * @param[in] eyeGeo Pre-calculated single eye coordinate metrics.
     * @param[in] shape Active shape to render.
     */
    void drawEye(const SingleEyeGeometry& eyeGeo, EyeShape shape);

public:
    EyeRenderer(IOledDisplay& oled);

    /**
     * @brief Clear screen buffer, render left and right eyes, and prepare screen for display.
     * @param[in] expr Target abstract expression profile.
     */
    void render(const Expression& expr);
};
