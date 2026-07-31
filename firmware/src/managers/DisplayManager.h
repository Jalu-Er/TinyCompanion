/**
 * @file DisplayManager.h
 * @brief Coordinates visual renders on OLED and status/time segmented output.
 * 
 * Responsibilities:
 * - Update time clocks on TM1637 display.
 * - Coordinate debug messages or battery logs.
 * - Limit slow clock query rate reads on shared I2C lanes.
 * 
 * TODO:
 * - [ ] Implement segment flicker effects for low battery states.
 */

#pragma once
#include "HAL/IOledDisplay.h"
#include "HAL/ITm1637.h"
#include "HAL/IRtcClock.h"
#include "EventSystem/IEventConsumer.h"
#include "ExpressionEngine/Expression.h"

class DisplayManager : public IEventConsumer {
private:
    IOledDisplay& oled;
    ITm1637& tm1637;
    IRtcClock& rtc;

    uint32_t rtcPollTimerMs = 0;
    TimeStruct cachedTime = {0, 0, 0, 0};

public:
    DisplayManager(IOledDisplay& screen, ITm1637& segments, IRtcClock& clock);
    
    /**
     * @brief Polls RTC and updates the segmented clock display interface.
     * @param[in] dtMs Time delta in milliseconds.
     */
    void tick(uint32_t dtMs);

    // IEventConsumer implementation
    void onEvent(const Event& event) override;

    /**
     * @brief Updates eye geometries on OLED display based on abstract expression profiles.
     */
    void updateExpression(const Expression& expr);
};
