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

DisplayManager::DisplayManager(IOledDisplay& screen, ITm1637& segments, IRtcClock& clock)
    : oled(screen), tm1637(segments), rtc(clock) {}

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
