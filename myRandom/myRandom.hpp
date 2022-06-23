/**
 * @file myRandom.hpp
 * @author Peter Nearing (pnearing@protonmail.com)
 * @brief Random helpers:
 * @version 0.1
 * @date 2022-06-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#ifndef MY_RANDOM_H
#define MY_RANDOM_H

#include "hardware/structs/rosc.h"
#include <pico/time.h>
#include "../myStandardDefines.hpp"
#include "../myADC/myADC.hpp"
/**
 * @brief Random helper functions:
 *  Uses the unconditioned ring oscilator to set the
 *  random seed. We read once a microsecond beacuse
 *  the source is periodic if read too quickly.
 *  Random code provided by: https://github.com/raspberrypi/pico-sdk/issues/569
 * 
 */
namespace myRandom {
/* Constants: */
    /**
     * @brief no error. Value 0 (NO_ERROR)
     * 
     */
    int16_t NO_ERROR = MY_NO_ERROR;
/* Funtions: */
    // Return a single random bit from the random oscilator.
    /**
     * @brief Return a single random bit from the random oscilator
     * 
     * @return Random bit. 
     */
    bool roscRandomBit() {
        sleep_us(1);    // Sleep so we don't read too often.
        return rosc_hw->randombit; // Return a random bit.
    }
    // Return  a random byte:
    /**
     * @brief Return a random byte from the random oscilator
     * 
     * @return uint8_t random byte
     */
    uint8_t roscRandomByte() {
        uint8_t value=0;
        for (uint8_t i=0; i<8; i++) {
            value |= roscRandomBit();
            value <<= 1;
        }
        return value;
    }
    // Return a random word:
    /**
     * @brief return a random word from the random oscilator.
     * 
     * @return uint random word
     */
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
    /**
     * @brief Seed srand with a seed generated from the random oscilator.
     * 
     */
    void seedFromROSC() {
        uint32_t randomSeed;
        for (uint8_t i=0; i<32; i++) {
            randomSeed |= roscRandomBit();
            randomSeed <<= 1;
        }
        srand(randomSeed);
    }
    // Generate a random seed by reading an unconnected adc channel, in arduino style.
    /**
     * @brief Generate a random seed by readin a floatind ADC pin, in arduino style.
     * 
     * @param adcChannel Channel to read
     * @return int16_t returns 0 (NO_ERROR) for okay, negative for error code. Error codes are from myADC
     */
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
    /**
     * @brief Seed srand from a floating analog pin. in arduino style
     * 
     * @param adcPin Pin to read.
     * @return int16_t returns 0 (NO_ERROR) for okay, negative for error code. Error codes are from myADC.
     */
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