/**
 * @file SensorManager.h
 * @brief Periodically polls hardware sensors, processes inputs using filters/hysteresis, and publishes semantic events.
 * 
 * Responsibilities:
 * - Collect raw data from Touch, Ultrasonic, LDR, and RTC abstractions.
 * - Detect state transitions using hysteresis bands and edge triggering.
 * - Enqueue clean semantic events into the EventQueue.
 */

#pragma once
#include "HAL/IUltrasonic.h"
#include "HAL/ITouchSensor.h"
#include "HAL/ILightSensor.h"
#include "HAL/IRtcClock.h"
#include "EventSystem/EventQueue.h"

enum class TimePeriod : uint8_t {
    UNKNOWN = 0,
    MORNING,
    AFTERNOON,
    EVENING,
    NIGHT
};

class SensorManager {
private:
    IUltrasonic& ultrasonic;
    ITouchSensor& touch;
    ILightSensor& light;
    IRtcClock& rtc;
    EventQueue& eventQueue;

    // Internal filter and state tracking parameters
    bool lastTouchState = false;
    bool isAmbientDark = false;
    bool isUserNear = false;
    TimePeriod currentPeriod = TimePeriod::UNKNOWN;

    // Constant parameters for hysteresis
    static constexpr uint8_t LDR_HYSTERESIS = 20;
    static constexpr uint8_t DISTANCE_HYSTERESIS = 3;

    // Helper to evaluate time period boundaries based on hour and minute
    TimePeriod getPeriodForTime(uint8_t hour, uint8_t minute);

public:
    SensorManager(IUltrasonic& ultra, ITouchSensor& touchSensor, ILightSensor& lightSensor, IRtcClock& rtcClock, EventQueue& queue);
    
    /**
     * @brief Polls all sensors. Should be called periodically (e.g. at 20 Hz / 50ms interval).
     */
    void poll();
};
