/**
 * @file SensorManager.h
 * @brief Periodically polls sensors, applies filter smoothing, and enqueues events.
 * 
 * Responsibilities:
 * - Hold references to input sensor interfaces (Touch, Ultrasonic, Light).
 * - Apply moving average smoothing or debouncers on raw inputs.
 * - Map threshold crossings to event triggers and write them to the EventQueue.
 * 
 * TODO:
 * - [ ] Implement a median filter array to stabilize HC-SR04 readings.
 * - [ ] Implement exponential filters for light level processing.
 */

#pragma once
#include "HAL/IUltrasonic.h"
#include "HAL/ITouchSensor.h"
#include "HAL/ILightSensor.h"
#include "EventSystem/EventQueue.h"

class SensorManager {
private:
    IUltrasonic& ultrasonic;
    ITouchSensor& touch;
    ILightSensor& light;
    EventQueue& eventQueue;

    // Filter states
    bool lastTouchState = false;
    uint32_t lastLdrFiltered = 0;
    
public:
    SensorManager(IUltrasonic& ultra, ITouchSensor& touchSensor, ILightSensor& lightSensor, EventQueue& queue);
    
    /**
     * @brief Polls all physical sensors and schedules event triggers. Called at 20 Hz.
     */
    void poll();
};
