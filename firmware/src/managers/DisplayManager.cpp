/**
 * @file DisplayManager.cpp
 * @brief Coordinates visual renders on OLED and status/time segmented output.
 * 
 * Responsibilities:
 * - Update digits on TM1637 clock display.
 * 
 * TODO:
 * - [ ] Implement time-colon blinking sweeps.
 */

#include "DisplayManager.h"
#include "Config.h"
#include <Arduino.h>

DisplayManager::DisplayManager(IOledDisplay& screen, ITm1637& segments, IRtcClock& clock)
    : oled(screen), tm1637(segments), rtc(clock), eyeRenderer(screen) {
    currentExpression.eyeShape = EyeShape::NORMAL;
    currentExpression.pupilRadius = 5;
    currentExpression.eyelidOpen = 100;
    currentExpression.blinkIntervalS = 4;
    currentExpression.aura = AuraState::Idle;
    currentExpression.sound = SoundEffect::NONE;
}

void DisplayManager::tick(uint32_t dtMs) {
    rtcPollTimerMs += dtMs;

    // Read RTC every 5 seconds to reduce I2C bus traffic
    if (rtcPollTimerMs >= 5000) {
        rtc.getTime(cachedTime);
        rtcPollTimerMs = 0;

        // Display HH:MM on TM1637 segments
        uint16_t timeVal = (cachedTime.hour * 100) + cachedTime.minute;
        tm1637.displayInteger(timeVal, true);
    }
}

void DisplayManager::onEvent(const Event& event) {
    // Placeholder to respond to semantic events and update visual state on OLED/TM1637
    (void)event;
}

void DisplayManager::updateExpression(const Expression& expr) {
    currentExpression = expr;
}

void DisplayManager::renderDisplay() {
    #ifdef RUN_RUNTIME_DIAGNOSTICS
    digitalWrite(PIN_TIMING_MARKER, HIGH);
    #endif

    eyeRenderer.render(currentExpression);

    #ifdef RUN_RUNTIME_DIAGNOSTICS
    digitalWrite(PIN_TIMING_MARKER, LOW);
    #endif
}
