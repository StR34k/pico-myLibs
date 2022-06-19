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
#include <hardware/gpio.h>
#include "hardware/adc.h"
#include <pico/time.h>
#include "../myADC/myADC.hpp"

namespace myRandom {
    // Return a single random bit from the random oscilator.
    bool randomBit() {
        sleep_us(1);    // Sleep so we don't read too often.
        return rosc_hw->randombit; // Return a random bit.
    }
    // Return  a random byte:
    uint8_t randomByte() {
        uint8_t value=0;
        for (uint8_t i=0; i<8; i++) {
            value |= randomBit();
            value <<= 1;
        }
        return value;
    }
    // Return a random word:
    uint randomWord() {
        uint value;
        uint8_t numBits = sizeof(value) * 8;
        for (uint8_t i=0; i<numBits; i++) {
            value |= randomBit();
            value <<= 1;
        }
        return value;
    }
    // Seed srand with a randomSeed generated from the random oscilator.
    void seed_from_rosc() {
        uint32_t randomSeed;
        for (uint8_t i=0; i<32; i++) {
            randomSeed |= randomBit();
            randomSeed <<= 1;
        }
        srand(randomSeed);
    }
    // Generate a random seed by reading an unconnected adc pin, in arduino style.
    bool seed_from_adc(uint8_t adcChannel) {
    // Init variables:
        uint32_t randomSeed = 0;
    // Check valid channel:
        if (myADC::validChannel(adcChannel) == false) { return false; }
    // Init ADC, and channel:
        myADC::initialize();
        myADC::initChannel(adcChannel);
    // Take 4 readings, only looking at the last 8 bits of each reading, for the total 32 bits:
        for (uint8_t i=0; i<4; i++) {
            int16_t reading = myADC::readChannelRaw(adcChannel); // Read the channel
            if (reading < 0) { return false; }      // if error return false.
            randomSeed <<= 8;                       // Shift the value.
            randomSeed |= (uint8_t)reading;         // Store the reading.
            sleep_us(1);                            // Delay to give adc time to 'randomize'
        }
        srand(randomSeed);  // Set the seed.
        return true;
    }

};
#endif