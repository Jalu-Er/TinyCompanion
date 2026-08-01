/**
 * @file GazeValidation.h
 * @brief Unit verification suite validating GazeController edge cases.
 */

#pragma once
#include "validation/IValidation.h"

#ifdef RUN_GAZE_TESTS
class GazeValidation : public IValidation {
public:
    GazeValidation();

    void run() override;
    const char* name() const override;
    const char* description() const override;
};
#endif
