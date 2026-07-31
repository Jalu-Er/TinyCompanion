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

    // 2. Register periodic tasks to scheduler
    scheduler.addTask("Sensors", TICK_PERIOD_SENSORS, pollSensorsCallback);
    scheduler.addTask("LED", TICK_PERIOD_LOGIC, updateLedCallback);
    scheduler.addTask("OLED", TICK_PERIOD_RENDER, updateOledCallback);
    scheduler.addTask("TM1637", 200, updateTm1637Callback);
    scheduler.addTask("Audio", TICK_PERIOD_AUDIO, updateAudioCallback);
    scheduler.addTask("Validation", 1000, validationRunnerCallback);

    Serial.println(F("Scheduler initialization complete. Running loop..."));
}

void TinyCompanionApp::loop() {
    // Non-blocking tick check
    scheduler.tick();
}

// --- Scheduler Task Callbacks Implementation ---

void TinyCompanionApp::pollSensorsCallback() {
    if (appInstance) {
        appInstance->sensorManager.poll();
        
        // Dequeue generated semantic events inside sensors task to demonstrate operations
        Event ev;
        while (appInstance->eventQueue.dequeue(ev)) {
            Serial.print(F("[EVENT QUEUED] Type Code: "));
            Serial.println(static_cast<uint8_t>(ev.type));
        }
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
    // Periodic status heartbeat printed to confirm timing fairness
    Serial.print(F("[HEARTBEAT] System Uptime: "));
    Serial.print(millis() / 1000);
    Serial.println(F(" seconds."));
}
