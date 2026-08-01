/**
 * @file Config.h
 * @brief Global configuration parameters, pin mappings, and scheduler constraints.
 * 
 * Responsibilities:
 * - Define pin mapping assignments based on the hardware specification.
 * - Establish thresholds (LDR ambient light limits, proximity triggers).
 * - Centralize calibration constants.
 * 
 * TODO:
 * - [ ] Verify pin compatibility with potential Version 2 servo timer demands.
 * - [ ] Calibrate LDR dark thresholds under practical desktop environments.
 */

#pragma once
#include <stdint.h>

// --- PIN SPECIFICATION ---
#define TM1637_CLK   2
#define TM1637_DIO   3

#define PIN_TRIG     6
#define PIN_ECHO     7

#define PIN_BUZZER   8
#define PIN_TOUCH    9
#define PIN_TIMING_MARKER 10
#define PIN_JITTER_MARKER 4

#define PIN_LDR      A0

#define LED_RED      11
#define LED_BLUE     12
#define LED_GREEN    13

// --- CONFIGURATION PARAMETERS ---
#define LDR_DARK_THRESHOLD   750  // Higher values mean darker room

// Proximity boundaries (cm)
#define DISTANCE_NEAR_LIMIT  15
#define DISTANCE_FAR_LIMIT   80

// Time limits
#define BEDTIME_HOUR         22
#define BEDTIME_MINUTE       0
#define WAKETIME_HOUR        7
#define WAKETIME_MINUTE      0

// Scheduler Ticks (ms)
#define TICK_PERIOD_SENSORS  50   // 20 Hz
#define TICK_PERIOD_LOGIC    100  // 10 Hz
#define TICK_PERIOD_AUDIO    20   // 50 Hz
#define TICK_PERIOD_RENDER   33   // 30 Hz (Smooth animations)
