/**
 * @file BlinkValidation.cpp
 * @brief Unit verification suite validating BlinkController edge cases.
 */

#include "BlinkValidation.h"
#include "AnimationEngine/BlinkController.h"
#include "AnimationEngine/AnimationController.h"
#include <Arduino.h>

#ifdef RUN_BLINK_TESTS
BlinkValidation::BlinkValidation() {}

void BlinkValidation::run() {
    Serial.println(F("\n--- Running BlinkController Pure Logic Validation ---"));
    LcgPrng prng;

    // TC1: Initial scheduling produces a valid next-blink timestamp.
    {
        Serial.print(F("TC1 (Scheduling): "));
        BlinkController bc(prng);
        bc.tick(1000, EyeShape::NORMAL);
        uint32_t nextTime = bc.getNextBlinkTime();
        if (nextTime > 1000 && bc.getState() == BlinkState::IDLE) {
            Serial.println(F("PASS"));
        } else {
            Serial.println(F("FAIL - Invalid next timestamp or initial state"));
            return;
        }
    }

    // TC2 & TC3: Trigger conditions
    {
        Serial.print(F("TC2 & TC3 (Trigger Timing): "));
        BlinkController bc(prng);
        bc.tick(1000, EyeShape::NORMAL);
        uint32_t nextTime = bc.getNextBlinkTime();

        // TC2: Do not trigger early
        bc.tick(nextTime - 1, EyeShape::NORMAL);
        if (bc.getState() != BlinkState::IDLE) {
            Serial.println(F("FAIL - Triggered early"));
            return;
        }

        // TC3: Trigger on scheduled time
        bc.tick(nextTime, EyeShape::NORMAL);
        if (bc.getState() != BlinkState::CLOSING) {
            Serial.println(F("FAIL - Did not trigger on schedule"));
            return;
        }
        Serial.println(F("PASS"));
    }

    // TC4: Blink phase progression
    {
        Serial.print(F("TC4 (Phase Progression): "));
        BlinkController bc(prng);
        bc.forceBlink(1000);
        
        // closing duration is 80ms
        bc.tick(1050, EyeShape::NORMAL);
        if (bc.getState() != BlinkState::CLOSING) {
            Serial.println(F("FAIL - Wrong state in closing"));
            return;
        }

        // closed at elapsed >= 80ms (now = 1080)
        bc.tick(1080, EyeShape::NORMAL);
        if (bc.getState() != BlinkState::CLOSED) {
            Serial.println(F("FAIL - Wrong state in closed"));
            return;
        }

        // opening at elapsed >= 50ms in closed (now = 1080 + 50 = 1130)
        bc.tick(1130, EyeShape::NORMAL);
        if (bc.getState() != BlinkState::OPENING) {
            Serial.println(F("FAIL - Wrong state in opening"));
            return;
        }

        // idle at elapsed >= 90ms in opening (now = 1130 + 90 = 1220)
        bc.tick(1220, EyeShape::NORMAL);
        if (bc.getState() != BlinkState::IDLE || bc.isBlinking()) {
            Serial.println(F("FAIL - Wrong state at completion"));
            return;
        }
        Serial.println(F("PASS"));
    }

    // TC5: Non-blocking execution verified by fast runtime
    {
        Serial.print(F("TC5 (Non-blocking): "));
        BlinkController bc(prng);
        uint32_t tStart = micros();
        bc.tick(1000, EyeShape::NORMAL);
        uint32_t tDiff = micros() - tStart;
        if (tDiff < 500) { // Should take under 500 microseconds (typically under 10us)
            Serial.println(F("PASS"));
        } else {
            Serial.print(F("FAIL - Execution took too long: "));
            Serial.print(tDiff);
            Serial.println(F("us"));
            return;
        }
    }

    // TC6: Millis Rollover safety
    {
        Serial.print(F("TC6 (Rollover Safety): "));
        BlinkController bc(prng);
        
        // Start right before 32-bit rollover
        uint32_t tStart = 0xFFFFFFF0;
        bc.forceBlink(tStart);
        
        // Progress 50ms after rollover (now = 0xFFFFFFF0 + 50 = 40)
        bc.tick(40, EyeShape::NORMAL);
        if (bc.getState() != BlinkState::CLOSING) { // closing is 80ms, so still closing
            Serial.print(F("FAIL - Modular timeline broke. State: "));
            Serial.println(static_cast<uint8_t>(bc.getState()));
            return;
        }
        Serial.println(F("PASS"));
    }

    // TC7 & TC8: Forced Blink & Repeated Trigger Interruption
    {
        Serial.print(F("TC7 & TC8 (Forced & Repeated): "));
        BlinkController bc(prng);
        
        // TC7: Force blink
        bc.forceBlink(1000);
        if (bc.getState() != BlinkState::CLOSING || !bc.isBlinking()) {
            Serial.println(F("FAIL - Force blink failed"));
            return;
        }

        // TC8: Repeated attempts during active blink should be ignored
        bc.tick(1050, EyeShape::NORMAL); // halfway through closing (duration 80)
        bc.forceBlink(1050); // should do nothing
        
        bc.tick(1080, EyeShape::NORMAL); // closing completed, enters closed phase
        if (bc.getState() != BlinkState::CLOSED) {
            Serial.println(F("FAIL - Repeated trigger reset active blink timeline"));
            return;
        }
        Serial.println(F("PASS"));
    }

    // TC9: Sleeping Policy
    {
        Serial.print(F("TC9 (Sleeping Policy): "));
        BlinkController bc(prng);
        bc.forceBlink(1000);
        
        // Sleeping shape suppresses active blink immediately and forces IDLE
        bc.tick(1050, EyeShape::SLEEPY);
        if (bc.isBlinking() || bc.getState() != BlinkState::IDLE) {
            Serial.println(F("FAIL - Active blink not suppressed by SLEEPY"));
            return;
        }

        // Base expression with SLEEPY and eyelidOpen = 0 remains eyelidOpen = 0
        Expression base;
        base.eyeShape = EyeShape::SLEEPY;
        base.eyelidOpen = 0;
        Expression result = bc.applyOverlay(base);
        if (result.eyelidOpen != 0) {
            Serial.println(F("FAIL - Eyelid overlay not 0 for Sleepy"));
            return;
        }
        Serial.println(F("PASS"));
    }

    // TC10: Error/Alert Policy
    {
        Serial.print(F("TC10 (Error & Alert Policy): "));
        BlinkController bc(prng);
        
        // Alert allows blinking
        bc.tick(1000, EyeShape::ALERT);
        uint32_t nextTime = bc.getNextBlinkTime();
        bc.tick(nextTime, EyeShape::ALERT);
        if (bc.getState() != BlinkState::CLOSING) {
            Serial.println(F("FAIL - Alert expression suppressed blink"));
            return;
        }

        // Reset
        BlinkController bc2(prng);
        // Error shape (shape code 99) suppresses blinking
        bc2.tick(1000, static_cast<EyeShape>(99));
        uint32_t errNextTime = bc2.getNextBlinkTime();
        bc2.tick(errNextTime, static_cast<EyeShape>(99));
        if (bc2.getState() != BlinkState::IDLE) {
            Serial.println(F("FAIL - Error expression allowed blinking"));
            return;
        }
        Serial.println(F("PASS"));
    }

    // TC11: Preservation of Expression Semantics
    {
        Serial.print(F("TC11 (Preserve Semantics): "));
        BlinkController bc(prng);
        Expression base;
        base.eyeShape = EyeShape::HAPPY;
        base.eyelidOpen = 75;
        base.pupilRadius = 6;

        bc.forceBlink(1000);
        bc.tick(1080, EyeShape::HAPPY); // enters CLOSED (factor = 0)
        
        Expression activeResult = bc.applyOverlay(base);
        if (activeResult.eyelidOpen != 0 || activeResult.eyeShape != EyeShape::HAPPY || activeResult.pupilRadius != 6) {
            Serial.println(F("FAIL - Eyelid did not close or semantic parameters mutated during blink"));
            return;
        }

        // Complete the blink
        bc.tick(1220, EyeShape::HAPPY); // returns to IDLE
        Expression finalResult = bc.applyOverlay(base);
        if (finalResult.eyelidOpen != 75 || finalResult.eyeShape != EyeShape::HAPPY) {
            Serial.println(F("FAIL - Semantic expression not fully restored after blink"));
            return;
        }
        Serial.println(F("PASS"));
    }

    // TC12: AnimationController + BlinkController interaction
    {
        Serial.print(F("TC12 (Animation + Blink Sync): "));
        AnimationController ac;
        BlinkController bc(prng);
        
        Expression target;
        target.eyeShape = EyeShape::NORMAL;
        target.eyelidOpen = 60;
        target.pupilRadius = 5;

        // Start normal transition (duration 100ms)
        ac.setTarget(target, 100, 1000);
        
        // Tick halfway (eyelidOpen is now 80)
        ac.tick(1050);
        Expression currentBase = ac.current();
        if (currentBase.eyelidOpen != 80) {
            Serial.print(F("FAIL - AnimationController wrong midpoint eyelid: "));
            Serial.println(currentBase.eyelidOpen);
            return;
        }

        // Trigger blink halfway through transition
        bc.forceBlink(1050);
        bc.tick(1090, currentBase.eyeShape); // CLOSED (factor = 0)
        
        Expression overlaid = bc.applyOverlay(currentBase);
        if (overlaid.eyelidOpen != 0) {
            Serial.println(F("FAIL - Eyelid not overridden to 0 by blink during animation"));
            return;
        }

        // Tick normal animation to completion (now = 1100, target is 60)
        ac.tick(1100);
        Expression finalBase = ac.current();
        if (finalBase.eyelidOpen != 60) {
            Serial.println(F("FAIL - Base animation got corrupted by active blink"));
            return;
        }

        // Complete blink (now = 1170)
        bc.tick(1170, finalBase.eyeShape); // IDLE
        Expression finalOverlaid = bc.applyOverlay(finalBase);
        if (finalOverlaid.eyelidOpen != 60) {
            Serial.print(F("FAIL - Final overlaid did not resolve to animation target: "));
            Serial.println(finalOverlaid.eyelidOpen);
            return;
        }
        Serial.println(F("PASS"));
    }

    Serial.println(F("All BlinkController validation tests completed successfully.\n"));
}

const char* BlinkValidation::name() const {
    return "Blink Controller Logic";
}

const char* BlinkValidation::description() const {
    return "Verifies autonomous phase progression, non-blocking LCG interval generators, Sleeping/Error blink suppression policies, and multiplicative overlay safety.";
}
#endif
