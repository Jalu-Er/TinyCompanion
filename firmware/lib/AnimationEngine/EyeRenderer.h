/**
 * @file EyeRenderer.h
 * @brief Procedural translation of EyeParameters into OLED drawing calls.
 * 
 * Responsibilities:
 * - Read EyeParameters and execute primitive geometric screen outputs.
 * - Manage blinking using clipping masks to save memory (Uno optimization).
 * 
 * TODO:
 * - [ ] Verify layout symmetry bounds on 128x64 pixels display space.
 * - [ ] Implement clipping rectangle overlays to override eye shapes.
 */

#pragma once
#include "../../include/HAL/IOledDisplay.h"
#include "AnimationEngine.h"

class EyeRenderer {
private:
    IOledDisplay& display;

public:
    EyeRenderer(IOledDisplay& oledDisplay) : display(oledDisplay) {}

    /**
     * @brief Translates active parameters to graphics commands in the display driver.
     * @param[in] params Set of computed coordinates to draw.
     */
    void render(const EyeParameters& params) {
        // TODO: Perform math centering and paint left/right eyes procedurally
        (void)params;
    }
};
