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
#include "validation/IValidation.h"

class OledValidation : public IValidation {
private:
    IOledDisplay& display;

public:
    OledValidation(IOledDisplay& oledDisplay);

    // IValidation interface implementation
    void run() override;
    const char* name() const override;
    const char* description() const override;
};
