/**
 * @file ArduinoUltrasonic.cpp
 * @brief Concrete implementation for HC-SR04 ultrasonic proximity measurement.
 * 
 * Responsibilities:
 * - Configure trigger pin as OUTPUT and echo pin as INPUT.
 * - Poll hardware range measurements using pulse trigger and duration tracking.
 * 
 * NOTE: Uses blocking pulseIn() with a short timeout. This is noted as technical debt.
 */

#include "ArduinoUltrasonic.h"
#include <Arduino.h>

ArduinoUltrasonic::ArduinoUltrasonic(uint8_t trigger, uint8_t echo) 
    : trigPin(trigger), echoPin(echo) {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    digitalWrite(trigPin, LOW);
}

uint16_t ArduinoUltrasonic::getDistanceCm() {
    // 1. Send trigger pulse
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // 2. Measure pulse duration on echo pin (timeout of 20ms = ~3.4 meters max)
    // Avoids infinite blocking if the sensor is disconnected
    uint32_t duration = pulseIn(echoPin, HIGH, 20000);
    
    if (duration == 0) {
        return 0; // Echo timeout or no object detected
    }
    
    // 3. Convert microseconds duration to centimeters (sound speed = 340m/s -> 1cm per 58.8us roundtrip)
    return (uint16_t)(duration / 58);
}
