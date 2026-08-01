/**
 * @file AnimationValidation.cpp
 * @brief Unit verification suite validating AnimationController edge cases.
 */

#include "AnimationValidation.h"
#include "AnimationEngine/AnimationController.h"
#include <Arduino.h>

#ifdef RUN_ANIMATION_TESTS
AnimationValidation::AnimationValidation() {}

void AnimationValidation::run() {
    Serial.println(F("\n--- Running AnimationController Pure Logic Validation ---"));

    AnimationController controller;

    // Test Case 1: Animation Start, Progress & Completion
    {
        Serial.print(F("TC1 (Progress): "));
        Expression target;
        target.eyeShape = EyeShape::ALERT;
        target.pupilRadius = 8;
        target.eyelidOpen = 0;
        target.blinkIntervalS = 4;
        target.aura = AuraState::Alert;
        target.sound = SoundEffect::ALERT_CHIME;

        uint32_t now = 1000;
        controller.setTarget(target, 100, now);

        if (controller.current().eyeShape != EyeShape::ALERT ||
            controller.current().aura != AuraState::Alert) {
            Serial.println(F("FAIL - Discrete property not updated instantly"));
            return;
        }

        controller.tick(now + 50);
        if (controller.current().pupilRadius != 6 || controller.current().eyelidOpen != 50) {
            Serial.print(F("FAIL - Progress calculation wrong. Pupil: "));
            Serial.print(controller.current().pupilRadius);
            Serial.print(F(", Eyelid: "));
            Serial.println(controller.current().eyelidOpen);
            return;
        }

        controller.tick(now + 100);
        if (controller.current().pupilRadius != 8 || controller.current().eyelidOpen != 0 || controller.isAnimating()) {
            Serial.println(F("FAIL - Completion boundary failed"));
            return;
        }
        Serial.println(F("PASS"));
    }

    // Test Case 2: Zero Duration Transition
    {
        Serial.print(F("TC2 (Zero Duration): "));
        Expression target;
        target.eyeShape = EyeShape::HAPPY;
        target.pupilRadius = 2;
        target.eyelidOpen = 80;
        target.blinkIntervalS = 4;
        target.aura = AuraState::Happy;
        target.sound = SoundEffect::NONE;

        controller.setTarget(target, 0, 2000);
        if (controller.current().pupilRadius != 2 || controller.current().eyelidOpen != 80 || controller.isAnimating()) {
            Serial.println(F("FAIL - Zero duration is animating or did not apply instantly"));
            return;
        }
        Serial.println(F("PASS"));
    }

    // Test Case 3: Transition Interruption
    {
        Serial.print(F("TC3 (Interruption): "));
        Expression base;
        base.eyeShape = EyeShape::NORMAL;
        base.pupilRadius = 0;
        base.eyelidOpen = 0;
        base.blinkIntervalS = 4;
        base.aura = AuraState::Idle;
        base.sound = SoundEffect::NONE;
        controller.setTarget(base, 0, 3000);

        Expression targetA = base;
        targetA.pupilRadius = 10;
        controller.setTarget(targetA, 100, 3000);

        controller.tick(3050);

        Expression targetB = base;
        targetB.pupilRadius = 0;
        controller.setTarget(targetB, 100, 3050);

        controller.tick(3100);
        if (controller.current().pupilRadius != 2) {
            Serial.print(F("FAIL - Interruption did not start from current state. Pupil: "));
            Serial.println(controller.current().pupilRadius);
            return;
        }
        Serial.println(F("PASS"));
    }

    // Test Case 4: Repeated Target Set
    {
        Serial.print(F("TC4 (Repeated Target): "));
        Expression target;
        target.eyeShape = EyeShape::NORMAL;
        target.pupilRadius = 10;
        target.eyelidOpen = 100;
        target.blinkIntervalS = 4;
        target.aura = AuraState::Idle;
        target.sound = SoundEffect::NONE;

        controller.setTarget(target, 100, 4000);
        controller.tick(4050);

        controller.setTarget(target, 100, 4050);
        controller.tick(4075);
        
        if (controller.current().pupilRadius != 7 && controller.current().pupilRadius != 8) {
            Serial.print(F("FAIL - Repeated target reset timeline. Pupil: "));
            Serial.println(controller.current().pupilRadius);
            return;
        }
        Serial.println(F("PASS"));
    }

    // Test Case 5: Millis Rollover Safety
    {
        Serial.print(F("TC5 (Rollover Safety): "));
        Expression base;
        base.eyeShape = EyeShape::NORMAL;
        base.pupilRadius = 0;
        base.eyelidOpen = 0;
        base.blinkIntervalS = 4;
        base.aura = AuraState::Idle;
        base.sound = SoundEffect::NONE;
        controller.setTarget(base, 0, 0xFFFFFFF0);

        Expression target = base;
        target.pupilRadius = 10;

        uint32_t startTime = 0xFFFFFFF0;
        controller.setTarget(target, 100, startTime);

        uint32_t tickTime = 40;
        controller.tick(tickTime);

        if (controller.current().pupilRadius != 5 || !controller.isAnimating()) {
            Serial.print(F("FAIL - Modular subtraction failed. Pupil: "));
            Serial.println(controller.current().pupilRadius);
            return;
        }
        Serial.println(F("PASS"));
    }

    Serial.println(F("All AnimationController validation tests completed successfully.\n"));
}

const char* AnimationValidation::name() const {
    return "Animation Controller Logic";
}

const char* AnimationValidation::description() const {
    return "Verifies linear integer transition lerp equations, interruption smoothness, rollover mathematical correctness, and duration boundary clamps.";
}
#endif
