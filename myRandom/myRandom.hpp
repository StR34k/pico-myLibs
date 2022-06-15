#ifndef MY_RANDOM_H
#define MY_RANDOM_H
#include "hardware/structs/rosc.h"
#include <pico/time.h>

void seed_random_from_rosc() {
    uint32_t randomSeed;
    bool randomBit;
    for (uint8_t i=0; i<32; i++) {
        randomBit = rosc_hw->randombit;
        sleep_us(randomBit);
        randomBit = rosc_hw->randombit;
        randomSeed |= (randomBit << i);
    }
    srand(randomSeed);
}
#endif