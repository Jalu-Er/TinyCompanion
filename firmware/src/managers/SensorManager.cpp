/**
 * @file SensorManager.cpp
 * @brief Periodically polls hardware sensors, processes inputs using filters/hysteresis, and publishes semantic events.
 * 
 * Responsibilities:
 * - Direct sensor polling.
 * - Translate raw inputs (ADC/cm/Time) into discrete semantic events.
 */

#include "SensorManager.h"
#include "Config.h"
#include <Arduino.h>

SensorManager::SensorManager(IUltrasonic& ultra, ITouchSensor& touchSensor, ILightSensor& lightSensor, IRtcClock& rtcClock, EventQueue& queue)
    : ultrasonic(ultra), touch(touchSensor), light(lightSensor), rtc(rtcClock), eventQueue(queue) {}

TimePeriod SensorManager::getPeriodForTime(uint8_t hour, uint8_t minute) {
    (void)minute; // Minute unused in current coarse boundary mapping
    if (hour >= 7 && hour < 12) {
        return TimePeriod::MORNING;
    } else if (hour >= 12 && hour < 17) {
        return TimePeriod::AFTERNOON;
    } else if (hour >= 17 && hour < 22) {
        return TimePeriod::EVENING;
    } else {
        return TimePeriod::NIGHT;
    }
}

void SensorManager::poll() {
    // 1. Process touch sensor reads (Edge detection)
    bool touched = touch.isTouched();
    if (touched != lastTouchState) {
        Event ev;
        ev.type = touched ? EventType::TOUCH_PRESSED : EventType::TOUCH_RELEASED;
        ev.data.metadata = 0;
        eventQueue.enqueue(ev);
        lastTouchState = touched;
    }

    // 2. Poll light sensor changes with hysteresis
    uint16_t lightLevel = light.getLightLevel();
    if (isAmbientDark) {
        // Transition to Bright if ADC drops below threshold - hysteresis band
        if (lightLevel < (LDR_DARK_THRESHOLD - LDR_HYSTERESIS)) {
            isAmbientDark = false;
            Event ev;
            ev.type = EventType::AMBIENT_BRIGHT;
            ev.data.metadata = 0;
            eventQueue.enqueue(ev);
        }
    } else {
        // Transition to Dark if ADC rises above threshold + hysteresis band
        if (lightLevel > (LDR_DARK_THRESHOLD + LDR_HYSTERESIS)) {
            isAmbientDark = true;
            Event ev;
            ev.type = EventType::AMBIENT_DARK;
            ev.data.metadata = 0;
            eventQueue.enqueue(ev);
        }
    }

    // 3. Poll proximity values with hysteresis
    uint16_t distance = ultrasonic.getDistanceCm();
    if (distance > 0) { // 0 indicates hardware read timeout/lost echo
        if (isUserNear) {
            // Transition to Far if distance exceeds limit + hysteresis band
            if (distance > (DISTANCE_NEAR_LIMIT + DISTANCE_HYSTERESIS)) {
                isUserNear = false;
                Event ev;
                ev.type = EventType::USER_LEAVING;
                ev.data.metadata = 0;
                eventQueue.enqueue(ev);
            }
        } else {
            // Transition to Near if distance falls within limit
            if (distance <= DISTANCE_NEAR_LIMIT) {
                isUserNear = true;
                Event ev;
                ev.type = EventType::USER_APPROACHING;
                ev.data.metadata = 0;
                eventQueue.enqueue(ev);
            }
        }
    }

    // 4. Poll RTC Time Period transitions
    TimeStruct now;
    if (rtc.getTime(now)) {
        TimePeriod period = getPeriodForTime(now.hour, now.minute);
        if (period != currentPeriod) {
            currentPeriod = period;
            Event ev;
            ev.data.metadata = 0;
            switch (period) {
                case TimePeriod::MORNING:   ev.type = EventType::TIME_PERIOD_MORNING;   break;
                case TimePeriod::AFTERNOON: ev.type = EventType::TIME_PERIOD_AFTERNOON; break;
                case TimePeriod::EVENING:   ev.type = EventType::TIME_PERIOD_EVENING;   break;
                case TimePeriod::NIGHT:     ev.type = EventType::TIME_PERIOD_NIGHT;     break;
                default:                    ev.type = EventType::NONE;                  break;
            }
            if (ev.type != EventType::NONE) {
                eventQueue.enqueue(ev);
            }
        }
    }
}
