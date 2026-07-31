/**
 * @file TinyCompanionApp.h
 * @brief Application composition root orchestrating adapters, managers, and the cooperative scheduler.
 * 
 * Responsibilities:
 * - Act as the Composition Root (instantiate and wire all components).
 * - Expose basic begin() and loop() lifecycles.
 * - Manage periodic tasks execution via a static non-blocking cooperative Scheduler.
 */

#pragma once
#include "adapters/ArduinoUltrasonic.h"
#include "adapters/ArduinoTouch.h"
#include "adapters/ArduinoLightSensor.h"
#include "adapters/ArduinoRtc.h"
#include "adapters/ArduinoLed.h"
#include "adapters/ArduinoBuzzer.h"
#include "adapters/ArduinoOledSH1106.h"
#include "adapters/ArduinoTm1637.h"
#include "EventSystem/EventQueue.h"
#include "EventSystem/EventDispatcher.h"
#include "EventSystem/EventHistory.h"
#include "EventSystem/EventStatistics.h"
#include "StateMachine/StateMachine.h"
#include "PersonalityEngine/PersonalityEngine.h"
#include "EmotionEngine/EmotionEngine.h"
#include "ExpressionEngine/ExpressionEngine.h"
#include "managers/SensorManager.h"
#include "managers/LedManager.h"
#include "managers/DisplayManager.h"
#include "managers/AudioManager.h"
#include "Scheduler/Scheduler.h"

class TinyCompanionApp {
private:
    // 1. Concrete Hardware Driver Adapters
    ArduinoUltrasonic ultrasonic;
    ArduinoTouch touch;
    ArduinoLightSensor light;
    ArduinoRtc rtc;
    ArduinoLed ledAura;
    ArduinoBuzzer buzzer;
    ArduinoOledSH1106 oledDisplay;
    ArduinoTm1637 tm1637;

    // 2. Shared Infrastructure
    EventQueue eventQueue;
    EventDispatcher eventDispatcher;
    EventHistory eventHistory;
    EventStatistics eventStatistics;
    StateMachine stateMachine;
    PersonalityEngine personalityEngine;
    EmotionEngine emotionEngine;
    ExpressionEngine expressionEngine;

    // 3. Application Managers
    SensorManager sensorManager;
    LedManager ledManager;
    DisplayManager displayManager;
    AudioManager audioManager;

    // 4. Cooperative Scheduler
    Scheduler scheduler;

    // Static callback wrappers matching Scheduler.h TaskCallback type
    static void pollSensorsCallback();
    static void updateFsmCallback();
    static void updateEmotionCallback();
    static void updateLedCallback();
    static void updateOledCallback();
    static void updateTm1637Callback();
    static void updateAudioCallback();
    static void validationRunnerCallback();

public:
    TinyCompanionApp();

    /**
     * @brief Performs setup and initialization of all hardware and software components.
     */
    void begin();

    /**
     * @brief Ticks the cooperative scheduler. Delegated directly from main loop().
     */
    void loop();
};
