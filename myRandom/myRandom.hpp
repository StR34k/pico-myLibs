/* myRandom.hpp
 *  Provides: seed_from_rosc() and randomBit().
 *  Uses the unconditioned ring oscilator to set the
 *  random seed. We read once a microsecond beacuse
 *  the source is periodic if read too quickly.
 *  Random code provided by: https://github.com/raspberrypi/pico-sdk/issues/569
 */
#ifndef MY_RANDOM_H
#define MY_RANDOM_H

#include "hardware/structs/rosc.h"
#include <pico/time.h>
namespace myRandom {
    bool randomBit() {
        sleep_us(1);
        return rosc_hw->randombit;
    }

    void seed_from_rosc() {
        uint32_t randomSeed;
        for (uint8_t i=0; i<32; i++) {
            randomSeed |= (randomBit() << i);
        }
        srand(randomSeed);
    }
};
#endif