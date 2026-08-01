/**
 * @file ArduinoBuzzer.cpp
 * @brief Concrete implementation for Passive Buzzer audio rendering.
 * 
 * Responsibilities:
 * - Generate signal frequencies and silence pins.
 * 
 * TODO:
 * - [ ] Implement non-blocking tones or standard tone() wrappers.
 */

#include "ArduinoBuzzer.h"
#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

static volatile uint8_t gBuzzerPin = 8;
static volatile bool gBuzzerActive = false;

ArduinoBuzzer::ArduinoBuzzer(uint8_t pin) : buzzerPin(pin) {
    gBuzzerPin = pin;
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, LOW);
}

void ArduinoBuzzer::playTone(uint16_t frequencyHz) {
    if (frequencyHz == 0) {
        stopTone();
        return;
    }

    cli();

    // Reset Timer 1 Control Registers
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    // Calculate OCR1A: Timer 1 CTC mode with prescaler 8 (16 MHz base clock)
    // OCR1A = (16,000,000 / (2 * 8 * F)) - 1 = (1,000,000 / F) - 1
    uint32_t ocrValue = (1000000UL / frequencyHz) - 1;
    if (ocrValue > 65535) ocrValue = 65535;
    OCR1A = ocrValue;

    // Configure CTC mode (WGM12 = 1) and set prescaler to 8 (CS11 = 1)
    TCCR1B |= (1 << WGM12) | (1 << CS11);

    // Enable Timer 1 Compare Match A Interrupt
    TIMSK1 |= (1 << OCIE1A);

    gBuzzerActive = true;

    sei();
}

void ArduinoBuzzer::stopTone() {
    cli();
    
    // Disable Timer 1 Compare Match A Interrupt
    TIMSK1 &= ~(1 << OCIE1A);
    
    // Disable Timer 1 Clock Source to stop counter
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
    
    gBuzzerActive = false;
    digitalWrite(buzzerPin, LOW);
    
    sei();
}

ISR(TIMER1_COMPA_vect) {
    if (gBuzzerActive) {
        uint8_t bit = digitalPinToBitMask(gBuzzerPin);
        uint8_t port = digitalPinToPort(gBuzzerPin);
        volatile uint8_t *out = portOutputRegister(port);
        *out ^= bit;
    }
}
