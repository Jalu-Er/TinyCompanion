/**
 * @file IValidation.h
 * @brief Reusable software interface contract for hardware verification validators.
 * 
 * Responsibilities:
 * - Define a generic execution flow for hardware test benches.
 * - Supply metadata getters (name, description) to orchestrate test runs cleanly.
 * 
 * TODO:
 * - [ ] Implement a test manager to run multiple validations sequentially.
 */

#pragma once

class IValidation {
public:
    virtual ~IValidation() {}

    /**
     * @brief Executes the validation test routine.
     */
    virtual void run() = 0;

    /**
     * @brief Returns the identifier name of the validator.
     */
    virtual const char* name() const = 0;

    /**
     * @brief Returns a brief summary of the validation targets.
     */
    virtual const char* description() const = 0;
};
