/**
 * @file main.cpp
 * @brief Standard entry point delegating control to the application Composition Root.
 * 
 * Responsibilities:
 * - Instantiate the reusable TinyCompanionApp container object.
 * - Delegate hardware setup and non-blocking loop ticks.
 */

#include "TinyCompanionApp.h"

#include "HAL/AuditDiagnostics.h"

// Unified application container instanced statically in global space
static TinyCompanionApp app;

void setup() {
    initStackWatermark();
    // Initialize Composition Root structures and configure periodic scheduler
    app.begin();
}

void loop() {
    // Keep coop-tasks ticking indefinitely
    app.loop();
}
