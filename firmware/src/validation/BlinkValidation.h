/**
 * @file BlinkValidation.h
 * @brief Unit verification suite validating BlinkController edge cases.
 */

#pragma once
#include "validation/IValidation.h"

#ifdef RUN_BLINK_TESTS
class BlinkValidation : public IValidation {
public:
    BlinkValidation();

    void run() override;
    const char* name() const override;
    const char* description() const override;
};
#endif
