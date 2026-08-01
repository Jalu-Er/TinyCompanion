/**
 * @file AuditDiagnostics.h
 * @brief Dynamic free memory diagnostics utility for AVR microcontrollers.
 */

#pragma once
#include <stdint.h>

#ifdef RUN_RUNTIME_DIAGNOSTICS
extern unsigned int __heap_start;

inline void initStackWatermark() {
    extern unsigned int __heap_start;
    uint8_t *p = (uint8_t *)&__heap_start;
    volatile uint8_t current_stack;
    // Set limit 50 bytes below current stack frame pointer to prevent self-corruption
    uint8_t *stack_limit = (uint8_t *)&current_stack - 50;
    
    while (p < stack_limit) {
        *p = 0x55;
        p++;
    }
}

inline uint16_t getUnusedStackSram() {
    extern unsigned int __heap_start;
    uint8_t *p = (uint8_t *)&__heap_start;
    uint16_t count = 0;
    while (*p == 0x55) {
        count++;
        p++;
    }
    return count;
}
#else
inline void initStackWatermark() {}
inline uint16_t getUnusedStackSram() {
    return 0;
}
#endif
