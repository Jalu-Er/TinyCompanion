/**
 * @file AuditDiagnostics.h
 * @brief Dynamic free memory diagnostics utility for AVR microcontrollers.
 */

#pragma once
#include <stdint.h>

#ifdef RUN_DIAGNOSTICS
extern unsigned int __heap_start;
extern void *__brkval;

inline int getFreeMemory() {
    int free_memory;
    if ((int)__brkval == 0) {
        free_memory = ((int)&free_memory) - ((int)&__heap_start);
    } else {
        free_memory = ((int)&free_memory) - ((int)__brkval);
    }
    return free_memory;
}
#else
inline int getFreeMemory() {
    return -1; // Disabled in production
}
#endif
