/**
 * @file AnimationValidation.h
 * @brief Unit verification suite validating AnimationController edge cases.
 */

#pragma once
#include "validation/IValidation.h"

#ifdef RUN_ANIMATION_TESTS
class AnimationValidation : public IValidation {
public:
    AnimationValidation();

    void run() override;
    const char* name() const override;
    const char* description() const override;
};
#endif
