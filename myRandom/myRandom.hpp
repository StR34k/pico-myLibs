/* myRandom.hpp
 *  provides my_seed_random_from_rosc.
 *  use the unconditioned ring oscilator to set the
 *  random seed. We read once a microsecond beacuse
 *  the source is periodic if read quickly.
 *  random code provided by: https://github.com/raspberrypi/pico-sdk/issues/569
 */
#ifndef MY_RANDOM_H
#define MY_RANDOM_H

#include "hardware/structs/rosc.h"
#include <pico/time.h>

void my_seed_random_from_rosc() {
    uint32_t randomSeed;
    bool randomBit;
    for (uint8_t i=0; i<32; i++) {
        randomBit = rosc_hw->randombit;
        sleep_us(1);
        randomSeed |= (randomBit << i);
    }
    srand(randomSeed);
}
#endif