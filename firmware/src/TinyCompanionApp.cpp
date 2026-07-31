/**
 * @file TinyCompanionApp.cpp
 * @brief Application composition root orchestrating adapters, managers, and the cooperative scheduler.
 * 
 * Responsibilities:
 * - Handle dependency injection and wire hardware drivers to software orchestrators.
 * - Initialize scheduler with periodic tasks matching platform timing frequencies.
 * - Serve as the application static delegation target.
 */

#include "TinyCompanionApp.h"
#include "Config.h"
#include <Arduino.h>

// Static reference pointer to current application instance
static TinyCompanionApp* appInstance = nullptr;

TinyCompanionApp::TinyCompanionApp()
    : ultrasonic(PIN_TRIG, PIN_ECHO),
      touch(PIN_TOUCH),
      light(PIN_LDR),
      rtc(),
      ledAura(LED_RED, LED_GREEN, LED_BLUE),
      buzzer(PIN_BUZZER),
      oledDisplay(),
      tm1637(TM1637_CLK, TM1637_DIO),
      eventQueue(),
      eventDispatcher(),
      eventHistory(),
      eventStatistics(),
      stateMachine(eventQueue),
      sensorManager(ultrasonic, touch, light, rtc, eventQueue),
      ledManager(ledAura),
      displayManager(oledDisplay, tm1637, rtc),
      audioManager(buzzer),
      scheduler() {
    appInstance = this;
}

void TinyCompanionApp::begin() {
    Serial.begin(115200);
    while(!Serial); // Wait for Serial console on USB
    Serial.println(F("\n============================================="));
    Serial.println(F("TinyCompanion Cooperative Scheduler Booting..."));
    Serial.println(F("============================================="));

    // 1. Initialize hardware display registers
    oledDisplay.begin();

    // 2. Initialize FSM Core to Boot State
    stateMachine.begin();

    // 3. Register event consumers to the dispatcher
    eventDispatcher.registerConsumer(&ledManager);
    eventDispatcher.registerConsumer(&displayManager);
    eventDispatcher.registerConsumer(&audioManager);

    // 4. Register periodic tasks to scheduler
    scheduler.addTask("Sensors", TICK_PERIOD_SENSORS, pollSensorsCallback);
    scheduler.addTask("FSM", TICK_PERIOD_LOGIC, updateFsmCallback);
    scheduler.addTask("LED", TICK_PERIOD_LOGIC, updateLedCallback);
    scheduler.addTask("OLED", TICK_PERIOD_RENDER, updateOledCallback);
    scheduler.addTask("TM1637", 200, updateTm1637Callback);
    scheduler.addTask("Audio", TICK_PERIOD_AUDIO, updateAudioCallback);
    scheduler.addTask("Validation", 1000, validationRunnerCallback);

    // 5. Enqueue final boot event to trigger transition to Idle State
    Event ev;
    ev.type = EventType::INITIALIZATION_FINISHED;
    eventQueue.enqueue(ev);

    Serial.println(F("Scheduler initialization complete. Running loop..."));
}

void TinyCompanionApp::loop() {
    // Non-blocking tick check
    scheduler.tick();
}

// --- Scheduler Task Callbacks Implementation ---

#include "EventSystem/EventLogger.h"

void TinyCompanionApp::pollSensorsCallback() {
    if (appInstance) {
        appInstance->sensorManager.poll();
        
        Event ev;
        while (appInstance->eventQueue.dequeue(ev)) {
            uint32_t now = millis();
            // 1. Record in the history trace
            appInstance->eventHistory.record(ev.type, now);
            // 2. Record statistics counter
            appInstance->eventStatistics.recordEvent(ev.type);
            // 3. Print semantic event log to serial
            EventLogger::log(ev.type, now);
            // 4. Feed event into behavioral State Machine
            appInstance->stateMachine.processEvent(ev);
            // 5. Dispatch event to consumers
            appInstance->eventDispatcher.dispatch(ev);
        }
    }
}

void TinyCompanionApp::updateFsmCallback() {
    if (appInstance) {
        // Tick active state internal countdown timers
        appInstance->stateMachine.tick(TICK_PERIOD_LOGIC);
    }
}

void TinyCompanionApp::updateLedCallback() {
    if (appInstance) {
        // Handle blink toggles periodically
        appInstance->ledAura.update();
    }
}

void TinyCompanionApp::updateOledCallback() {
    // Future placeholder: render current eye state to oled buffer
}

void TinyCompanionApp::updateTm1637Callback() {
    // Future placeholder: display current rtc time on clock display
}

void TinyCompanionApp::updateAudioCallback() {
    // Future placeholder: play active buzzer note
}

void TinyCompanionApp::validationRunnerCallback() {
    if (appInstance) {
        Serial.print(F("[HEARTBEAT] Uptime: "));
        Serial.print(millis() / 1000);
        Serial.print(F("s | FSM State: "));
        Serial.print(static_cast<uint8_t>(appInstance->stateMachine.getCurrentState()));
        Serial.print(F(" | TouchPress Count: "));
        Serial.print(appInstance->eventStatistics.getCounter(EventType::TOUCH_PRESSED));
        
        // Print the latest event type from the history trace for debug inspection
        uint8_t count = appInstance->eventHistory.getCount();
        if (count > 0) {
            HistoryEntry entry;
            if (appInstance->eventHistory.getEntry(count - 1, entry)) {
                Serial.print(F(" | Latest History Code: "));
                Serial.print(static_cast<uint8_t>(entry.type));
                Serial.print(F(" at "));
                Serial.print(entry.timestamp);
                Serial.print(F(" ms"));
            }
        }
        Serial.println();
    }
}
