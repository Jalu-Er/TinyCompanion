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
#include "validation/AnimationValidation.h"
#include "validation/BlinkValidation.h"
#include "validation/GazeValidation.h"
#include "HAL/AuditDiagnostics.h"

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
      personalityEngine(),
      emotionEngine(personalityEngine),
      expressionEngine(),
      prng(),
      animationController(),
      blinkController(prng),
      gazeController(prng),
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

    #ifdef RUN_RUNTIME_DIAGNOSTICS
    pinMode(PIN_TIMING_MARKER, OUTPUT);
    digitalWrite(PIN_TIMING_MARKER, LOW);
    pinMode(PIN_JITTER_MARKER, OUTPUT);
    digitalWrite(PIN_JITTER_MARKER, LOW);
    #endif

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
    scheduler.addTask("Emotion", TICK_PERIOD_LOGIC, updateEmotionCallback);
    scheduler.addTask("LED", TICK_PERIOD_LOGIC, updateLedCallback);
    scheduler.addTask("OLED", 67, updateOledCallback); // Strictly limited to 15 Hz refresh
    scheduler.addTask("TM1637", 200, updateTm1637Callback);
    scheduler.addTask("Audio", TICK_PERIOD_AUDIO, updateAudioCallback);
    scheduler.addTask("Validation", 1000, validationRunnerCallback);

    // 5. Enqueue final boot event to trigger transition to Idle State
    Event ev;
    ev.type = EventType::INITIALIZATION_FINISHED;
    eventQueue.enqueue(ev);

#ifdef RUN_ANIMATION_TESTS
    // 6. Execute pure logic animation tests on boot
    AnimationValidation animVal;
    animVal.run();
#endif

#ifdef RUN_BLINK_TESTS
    // 7. Execute pure logic blink overlay tests on boot
    BlinkValidation blinkVal;
    blinkVal.run();
#endif

#ifdef RUN_GAZE_TESTS
    // 8. Execute pure logic gaze movement tests on boot
    GazeValidation gazeVal;
    gazeVal.run();
#endif

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
            // 5. Feed event and FSM state into the Emotion Engine
            appInstance->emotionEngine.processStateAndEvent(
                appInstance->stateMachine.getCurrentState(), ev
            );
            // 6. Dispatch event to consumers
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

void TinyCompanionApp::updateEmotionCallback() {
    if (appInstance) {
        CompanionState state = appInstance->stateMachine.getCurrentState();
        // 1. Tick emotional values decay over time
        appInstance->emotionEngine.tick(TICK_PERIOD_LOGIC, state);
        
        // 2. Compute abstract expression based on updated emotions
        Expression expr = appInstance->expressionEngine.calculateExpression(
            appInstance->emotionEngine.getEmotionState(), state
        );
        
        // 3. Set target expression into the AnimationController (500 ms transition)
        uint32_t now = millis();
        appInstance->animationController.setTarget(expr, 500, now);
        
        // 4. Directly feed presentation expressions into Led & Audio managers
        appInstance->ledManager.updateExpression(expr);
        appInstance->audioManager.updateExpression(expr);
    }
}

void TinyCompanionApp::updateLedCallback() {
    #ifdef RUN_RUNTIME_DIAGNOSTICS
    digitalWrite(PIN_JITTER_MARKER, HIGH);
    #endif

    if (appInstance) {
        // Handle blink toggles periodically
        appInstance->ledAura.update();
    }

    #ifdef RUN_RUNTIME_DIAGNOSTICS
    digitalWrite(PIN_JITTER_MARKER, LOW);
    #endif
}

void TinyCompanionApp::updateOledCallback() {
    if (appInstance) {
        uint32_t now = millis();
        // 1. Step the animation controller
        appInstance->animationController.tick(now);
        
        // 2. Extract current interpolated base expression
        Expression baseExpr = appInstance->animationController.current();
        
        // 3. Step the blink controller and apply overlay on top of base expression
        appInstance->blinkController.tick(now, baseExpr.eyeShape);
        Expression overlaidExpr = appInstance->blinkController.applyOverlay(baseExpr);
        
        // 4. Step the gaze controller and apply overlay on top of blink-overlaid expression
        appInstance->gazeController.tick(now, overlaidExpr.eyeShape);
        Expression finalExpr = appInstance->gazeController.applyOverlay(overlaidExpr);
        
        // 5. Sync the final overlaid frame with displayManager
        appInstance->displayManager.updateExpression(finalExpr);
        
        // 6. Flush visual buffer to screen at 15 Hz
        appInstance->displayManager.renderDisplay();
    }
}

void TinyCompanionApp::updateTm1637Callback() {
    // Future placeholder: display current rtc time on clock display
}

void TinyCompanionApp::updateAudioCallback() {
    // Future placeholder: play active buzzer note
}

void TinyCompanionApp::validationRunnerCallback() {
    if (appInstance) {
        uint32_t uptimeS = millis() / 1000;
        
        // Rotate target visual expression validator every 3 seconds
        uint8_t stage = (uptimeS / 3) % 6;
        
        Expression testExpr;
        testExpr.pupilRadius = 5;
        testExpr.eyelidOpen = 100;
        testExpr.blinkIntervalS = 4;
        testExpr.aura = AuraState::Idle;
        testExpr.sound = SoundEffect::NONE;
        
        const char* shapeName = "Unknown";
        
        switch (stage) {
            case 0:
                testExpr.eyeShape = EyeShape::NORMAL;
                shapeName = "Neutral";
                break;
            case 1:
                testExpr.eyeShape = EyeShape::HAPPY;
                testExpr.aura = AuraState::Happy;
                shapeName = "Happy";
                break;
            case 2:
                testExpr.eyeShape = EyeShape::SQUINT;
                testExpr.eyelidOpen = 50;
                testExpr.pupilRadius = 3;
                testExpr.aura = AuraState::Thinking;
                shapeName = "Thinking (Squint)";
                break;
            case 3:
                testExpr.eyeShape = EyeShape::ALERT;
                testExpr.pupilRadius = 8;
                testExpr.aura = AuraState::Alert;
                shapeName = "Alert";
                break;
            case 4:
                testExpr.eyeShape = EyeShape::SLEEPY;
                testExpr.eyelidOpen = 0;
                testExpr.aura = AuraState::Sleeping;
                shapeName = "Sleeping";
                break;
            case 5:
                testExpr.eyeShape = EyeShape::NORMAL; // default error cross shape
                testExpr.eyeShape = static_cast<EyeShape>(99); // Force default/unknown shape trigger
                testExpr.eyelidOpen = 40;
                testExpr.aura = AuraState::Error;
                shapeName = "Error (Cross)";
                break;
        }
        
        // Push current test expression sequence to AnimationController (500 ms transition)
        uint32_t now = millis();
        appInstance->animationController.setTarget(testExpr, 500, now);
        
        const EmotionState& emo = appInstance->emotionEngine.getEmotionState();
        Serial.print(F("[VALIDATOR] Shape: "));
        Serial.print(shapeName);
        Serial.print(F(" | Uptime: "));
        Serial.print(uptimeS);
        Serial.print(F("s | FSM State: "));
        Serial.print(static_cast<uint8_t>(appInstance->stateMachine.getCurrentState()));
        Serial.print(F(" | Valence: "));
        Serial.print(emo.valence);
        Serial.print(F(" | Arousal: "));
        Serial.print(emo.arousal);
        Serial.print(F(" | TouchPress: "));
        Serial.print(appInstance->eventStatistics.getCounter(EventType::TOUCH_PRESSED));
        
        // Print the latest event type from the history trace for debug inspection
        uint8_t count = appInstance->eventHistory.getCount();
        if (count > 0) {
            HistoryEntry entry;
            if (appInstance->eventHistory.getEntry(count - 1, entry)) {
                Serial.print(F(" | Latest History Code: "));
                Serial.print(static_cast<uint8_t>(entry.type));
            }
        }
        
        #ifdef RUN_RUNTIME_DIAGNOSTICS
        uint16_t unusedStack = getUnusedStackSram();
        uint16_t peakStack = 304 - unusedStack; // 304 is (2048 - 1744) available for stack
        uint8_t pctUtil = (static_cast<uint32_t>(peakStack) * 100) / 304;
        Serial.print(F(" | [DIAGNOSTIC] Peak Stack: "));
        Serial.print(peakStack);
        Serial.print(F(" bytes | Unused SRAM: "));
        Serial.print(unusedStack);
        Serial.print(F(" bytes | Util: "));
        Serial.print(pctUtil);
        Serial.print(F("%"));
        #endif

        Serial.println();
    }
}
