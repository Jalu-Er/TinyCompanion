/**
 * @file main.cpp
 * @brief Entry point, POST, and cooperative task scheduler.
 * 
 * Responsibilities:
 * - Instantiate concrete adapters and inject dependencies into managers.
 * - Manage cooperative task rates without blocking loops.
 * - Execute Power-On Self-Test (POST).
 * 
 * TODO:
 * - [ ] Implement a system watchdog timer to recover from locks.
 * - [ ] Implement precise battery diagnostics check during startup.
 */

#include <Arduino.h>
#include "Config.h"
#include "EventSystem/EventQueue.h"
#include "PersonalityEngine/PersonalityEngine.h"
#include "EmotionEngine/EmotionEngine.h"
#include "StateMachine/StateMachine.h"
#include "AnimationEngine/AnimationEngine.h"
#include "AnimationEngine/EyeRenderer.h"

// Concrete adapter headers (to be generated in src/adapters/)
#include "adapters/ArduinoUltrasonic.h"
#include "adapters/ArduinoTouch.h"
#include "adapters/ArduinoLightSensor.h"
#include "adapters/ArduinoRtc.h"
#include "adapters/ArduinoBuzzer.h"
#include "adapters/ArduinoLed.h"
#include "adapters/ArduinoOledSH1106.h"
#include "adapters/ArduinoTm1637.h"

// System manager headers (to be generated in src/managers/)
#include "managers/SensorManager.h"
#include "managers/DisplayManager.h"
#include "managers/AudioManager.h"
#include "managers/LedManager.h"

// Global instances
static EventQueue globalEventQueue;
static PersonalityEngine globalPersonality;
static EmotionEngine globalEmotion(globalPersonality);
static StateMachine globalStateMachine(globalEmotion);
static AnimationEngine globalAnimation;

// Hardware drivers
static ArduinoUltrasonic adapterUltrasonic(PIN_TRIG, PIN_ECHO);
static ArduinoTouch adapterTouch(PIN_TOUCH);
static ArduinoLightSensor adapterLight(PIN_LDR);
static ArduinoRtc adapterRtc;
static ArduinoBuzzer adapterBuzzer(PIN_BUZZER);
static ArduinoLed adapterLed(LED_RED, LED_GREEN, LED_BLUE);
static ArduinoOledSH1106 adapterOled;
static ArduinoTm1637 adapterTm1637(TM1637_CLK, TM1637_DIO);

// Managers
static SensorManager sensorManager(adapterUltrasonic, adapterTouch, adapterLight, globalEventQueue);
static DisplayManager displayManager(adapterOled, adapterTm1637, adapterRtc);
static AudioManager audioManager(adapterBuzzer);
static LedManager ledManager(adapterLed);

// Eye rendering pipeline
static EyeRenderer eyeRenderer(adapterOled);

// Cooperative task runner layouts
struct Task {
    void (*taskFunc)();
    uint32_t periodMs;
    uint32_t lastRunMs;
};

// Scheduler callback forwards
static void taskPollSensors();
static void taskUpdateLogic();
static void taskUpdateAudio();
static void taskRenderDisplay();

static Task systemScheduler[] = {
    { taskPollSensors,   TICK_PERIOD_SENSORS, 0 },
    { taskUpdateLogic,   TICK_PERIOD_LOGIC,   0 },
    { taskUpdateAudio,   TICK_PERIOD_AUDIO,   0 },
    { taskRenderDisplay, TICK_PERIOD_RENDER,  0 }
};

void setup() {
    Serial.begin(115200);

    // 1. Initialize Display Adapters
    adapterOled.begin();
    adapterTm1637.clear();

    // 2. Run self-checks & POST indications (RGB flashing, quick chime)
    adapterLed.setColor(255, 0, 0); // Red
    adapterBuzzer.playTone(440);
    delay(100);
    adapterLed.setColor(0, 255, 0); // Green
    adapterBuzzer.playTone(880);
    delay(100);
    adapterLed.setColor(0, 0, 255); // Blue
    adapterBuzzer.stopTone();
    delay(100);
    adapterLed.setColor(0, 0, 0);   // Off

    // 3. Set default state checking clock readings
    TimeStruct initialTime;
    if (adapterRtc.getTime(initialTime)) {
        // Enqueue simulated sunrise/sunset to boot FSM correctly
        Event startupTimeEvent;
        startupTimeEvent.type = (initialTime.hour >= BEDTIME_HOUR || initialTime.hour < WAKETIME_HOUR) 
            ? EventType::LIGHT_LEVEL_DARK : EventType::LIGHT_LEVEL_BRIGHT;
        globalEventQueue.enqueue(startupTimeEvent);
    }
}

void loop() {
    uint32_t currentMillis = millis();

    // Loop cooperative scheduler tasks
    for (auto& task : systemScheduler) {
        if (currentMillis - task.lastRunMs >= task.periodMs) {
            task.taskFunc();
            task.lastRunMs = currentMillis;
        }
    }
}

static void taskPollSensors() {
    sensorManager.poll();
}

static void taskUpdateLogic() {
    Event currentEvent;

    // Process queued events
    while (globalEventQueue.dequeue(currentEvent)) {
        globalStateMachine.processEvent(currentEvent);
        globalEmotion.processEvent(currentEvent);
    }

    // Engine updates (100ms ticks)
    globalStateMachine.tick(TICK_PERIOD_LOGIC);
    globalEmotion.tick(TICK_PERIOD_LOGIC);

    // Coordinate inputs to updates
    globalAnimation.setEmotion(globalEmotion.getCurrentEmotion());
    audioManager.setState(globalStateMachine.getCurrentState());
    ledManager.setEmotion(globalEmotion.getValence(), globalEmotion.getArousal());
}

static void taskUpdateAudio() {
    audioManager.tick(TICK_PERIOD_AUDIO);
}

static void taskRenderDisplay() {
    // Tick frame interpolation
    globalAnimation.tick(TICK_PERIOD_RENDER);
    
    // Clear screen
    adapterOled.clear();
    
    // Procedural eye render
    eyeRenderer.render(globalAnimation.getEyeParameters());
    
    // Push buffer to display hardware
    adapterOled.display();
    
    // Tick segment outputs
    displayManager.tick(TICK_PERIOD_RENDER);
}
