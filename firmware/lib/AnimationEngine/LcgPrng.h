/**
 * @file LcgPrng.h
 * @brief Lightweight deterministic Linear Congruential Generator.
 */

#pragma once
#include <stdint.h>

class LcgPrng {
private:
    uint32_t state;

public:
    LcgPrng(uint32_t seed = 5381) : state(seed) {}

    void setSeed(uint32_t seed) {
        state = seed;
    }

    uint16_t next() {
        state = (1103515245UL * state + 12345UL) & 0x7FFFFFFFUL;
        return static_cast<uint16_t>(state >> 16);
    }
};
