/**
 * @file SensorManager.cpp
 * @brief Periodically polls sensors, applies filter smoothing, and enqueues events.
 * 
 * Responsibilities:
 * - Collect range, light, and touch values.
 * - Detect transition thresholds and enqueue Event definitions.
 * 
 * TODO:
 * - [ ] Implement ultrasonic debouncing timers.
 */

#include "SensorManager.h"
#include "Config.h"

SensorManager::SensorManager(IUltrasonic& ultra, ITouchSensor& touchSensor, ILightSensor& lightSensor, EventQueue& queue)
    : ultrasonic(ultra), touch(touchSensor), light(lightSensor), eventQueue(queue) {}

void SensorManager::poll() {
    // 1. Process touch sensor reads
    bool touched = touch.isTouched();
    if (touched != lastTouchState) {
        Event touchEvent;
        touchEvent.type = touched ? EventType::TOUCH_TRIGGERED : EventType::TOUCH_RELEASED;
        eventQueue.enqueue(touchEvent);
        lastTouchState = touched;
    }

    // 2. Poll light sensor changes (1 second LDR ticks)
    uint16_t lightLevel = light.getLightLevel();
    // TODO: Apply exponential smoothing filter
    (void)lightLevel;

    // 3. Poll proximity values
    uint16_t distance = ultrasonic.getDistanceCm();
    // TODO: Apply running median filters and trigger OBJECT_DETECTED events
    (void)distance;
}
