/**
 * @file OledValidation.h
 * @brief Dedicated hardware validation test runner for OLED SH1106 display.
 * 
 * Responsibilities:
 * - Define verification loop sequences for displaying text, shapes, and bitmaps.
 * - Verify library and HAL integration correctness outside the main application file.
 * 
 * TODO:
 * - [ ] Connect additional sensors to validation screens if required.
 */

#pragma once
#include "HAL/IOledDisplay.h"

class OledValidation {
private:
    IOledDisplay& display;

public:
    OledValidation(IOledDisplay& oledDisplay);

    /**
     * @brief Executes the full validation sequence (static text, clear screen, geometry shapes, bitmaps).
     */
    void runTest();
};
