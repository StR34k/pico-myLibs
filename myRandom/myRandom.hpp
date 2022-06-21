/* myRandom.hpp
 *  Provides: seed_from_rosc() and randomBit().
 *  Uses the unconditioned ring oscilator to set the
 *  random seed. We read once a microsecond beacuse
 *  the source is periodic if read too quickly.
 *  Random code provided by: https://github.com/raspberrypi/pico-sdk/issues/569
 * 
 *    Functions:
 *      roscRandomBit:      returns a single random bit from the random oscilator.
 *      roscRandomByte:     returns 8 random bits from the random oscilator.
 *      roscRandomWord:     return n bits, where n is sizeof(uint) * 8. as defined by the compliler.
 *      seedFromROSC:       seed srand using random bits from random oscilator.
 *      seedFromADCChannel: seed srand given an unconnected ADC channel.
 *      seedFromADCPin:     same as channel, just takes a pin as an argument.
 */
#ifndef MY_RANDOM_H
#define MY_RANDOM_H

#include "hardware/structs/rosc.h"
#include <pico/time.h>
#include "../myStandardDefines.hpp"
#include "../myADC/myADC.hpp"

namespace myRandom {
/* Constants: */
    int16_t NO_ERROR = MY_NO_ERROR;
/* Funtions: */
    // Return a single random bit from the random oscilator.
    bool roscRandomBit() {
        sleep_us(1);    // Sleep so we don't read too often.
        return rosc_hw->randombit; // Return a random bit.
    }
    // Return  a random byte:
    uint8_t roscRandomByte() {
        uint8_t value=0;
        for (uint8_t i=0; i<8; i++) {
            value |= roscRandomBit();
            value <<= 1;
        }
        return value;
    }
    // Return a random word:
    uint roscRandomWord() {
        uint value;
        uint8_t numBits = sizeof(value) * 8;
        for (uint8_t i=0; i<numBits; i++) {
            value |= roscRandomBit();
            value <<= 1;
        }
        return value;
    }
    // Seed srand with a randomSeed generated from the random oscilator.
    void seedFromROSC() {
        uint32_t randomSeed;
        for (uint8_t i=0; i<32; i++) {
            randomSeed |= roscRandomBit();
            randomSeed <<= 1;
        }
        srand(randomSeed);
    }
    // Generate a random seed by reading an unconnected adc pin, in arduino style.
    int16_t seedFromADCChannel(uint8_t adcChannel) {
    // Init variables:
        uint32_t randomSeed = 0;
        int16_t returnValue;
    // Init ADC, and channel:
        myADC::initialize();
        returnValue = myADC::initChannel(adcChannel);
        if (returnValue < 0) { return returnValue; } // If there is an error return the error.
    // Take 4 readings, only looking at the last 8 bits of each reading, for the total 32 bits:
        for (uint8_t i=0; i<4; i++) {
            returnValue = myADC::readChannelRaw(adcChannel); // Read the channel
            if (returnValue < 0) { return returnValue; }      // if error return false.
            randomSeed <<= 8;                       // Shift the value.
            randomSeed |= (uint8_t)returnValue;         // Store the reading.
            sleep_us(1);                            // Delay to give adc time to 'randomize'
        }
        srand(randomSeed);  // Set the seed.
        return NO_ERROR;
    }
    
    int16_t seedFromADCPin(uint8_t adcPin) {
    // Init variables:
        uint32_t randomSeed = 0;
        int16_t returnValue;
    // Init ADC, and channel:
        myADC::initialize();
        returnValue = myADC::initPin(adcPin);
        if (returnValue < 0) { return returnValue; } // if error return error code.
    // Take 4 readings, only looking at the last 8 bits of each reading, for the total 32 bits:
        for (uint8_t i=0; i<4; i++) {
            returnValue = myADC::readPinRaw(adcPin); // Read the pin
            if (returnValue < 0) { return returnValue; }      // if error return error code.
            randomSeed <<= 8;                       // Shift the value.
            randomSeed |= (uint8_t)returnValue;         // Store the reading.
            sleep_us(1);                            // Delay to give adc time to 'randomize'
        }
        srand(randomSeed);  // Set the seed.
        return NO_ERROR;
    }
};
#endif