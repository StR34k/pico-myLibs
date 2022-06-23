/**
 * @file myHelpers.hpp
 * @author Peter Nearing (pnearing@protonmail.com)
 * @brief Helper functions.
 * @version 0.1
 * @date 2022-06-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MY_HELPERS_H
#define MY_HELPERS_H

#include <hardware/gpio.h>
#include <hardware/timer.h>
#include <pico/time.h>

#include "myStandardDefines.hpp"
#include "myErrorCodes.hpp"
/**
 * @brief Helper functions.
 * Define some helper functions for use within pico-myLibs.
 */
namespace myHelpers {
    /**
     * @brief Validate if the pin is a valid pin number for the rp2040.
     * 
     * @param pin Pin to validate.
     * @return true is a valid pin
     * @return false is not a valid pin
     */
    bool inline isPin(const uint8_t pin) {
        if (pin < MY_MAX_PIN) { return true; }
        return false;
    }
    /**
     * @brief Return the bit mask for a single pin.
     * 
     * @param pin Pin to generate the bit mask for
     * @return Postive = bitmask, Negaitve = error code.
     */
    int32_t inline pinToBitMask(const uint8_t pin) {
        if (isPin(pin) == false ) { return MY_INVALID_PIN; }
        return (1 << pin);
    }
    /**
     * @brief Return a pin number given a valid bit mask.
     * 
     * @param bitMask The bit mask for a single pin.
     * @return int8_t The pin associated with the bitmask.
     */
    int8_t inline bitMaskToPin(const uint32_t bitMask) {
        switch (bitMask) {
        case 0x00000001: return 0;
        case 0x00000002: return 1;
        case 0x00000004: return 2;
        case 0x00000008: return 3;
        case 0x00000010: return 4;
        case 0x00000020: return 5;
        case 0x00000040: return 6;
        case 0x00000080: return 7;
        case 0x00000100: return 8;
        case 0x00000200: return 9;
        case 0x00000400: return 10;
        case 0x00000800: return 11;
        case 0x00001000: return 12;
        case 0x00002000: return 13;
        case 0x00004000: return 14;
        case 0x00008000: return 15;
        case 0x00010000: return 16;
        case 0x00020000: return 17;
        case 0x00040000: return 18;
        case 0x00080000: return 19;
        case 0x00100000: return 20;
        case 0x00200000: return 21;
        case 0x00400000: return 22;
        case 0x00800000: return 23;
        case 0x01000000: return 24;
        case 0x02000000: return 25;
        case 0x04000000: return 26;
        case 0x08000000: return 27;
        case 0x10000000: return 28;
        case 0x20000000: return 29;
        default: return MY_INVALID_BIT_MASK;
        }
    }
    /**
     * @brief busy wait until LOW signal, or timeout. a timeout of 0 (the default)
     *                      means that it will block until LOW. If the pin is already LOW, it
     *                      will return immediatly with a value of 0. If timeout is reached it
     *                      returns -4 (MY_TIMEOUT). If GPIO is set as out put it will return -5
     *                      (MY_INVALID_DIR). Assumes gpio_init has already been called.
     * 
     * @param pin Pin to read
     * @param timeout Timeout time in microseconds
     * @return Postive = time elapsed in microseconds, negative error code.
     */
    int32_t waitForLow32(const uint8_t pin, const uint32_t timeout=0) {
        uint32_t startTime;     // Time started.
        uint32_t timeoutTime;   // absolute timeout time.
        uint32_t stopTime;      // time stopped.
    // Check pin status:
        if (isPin(pin) == false) { return MY_INVALID_PIN; }
        if (gpio_get_dir(pin) == GPIO_OUT) { return MY_INVALID_DIR; }
    // Check if pin is already low:
        bool state = gpio_get(pin);
        if (state == false) { return 0; }
    // If a timeout is requested wait until timeout.
        if (timeout > 0) {
            startTime = time_us_32();
            timeoutTime = startTime + timeout;
            while (state == true) {
                if (time_us_32() >= timeout) { return MY_TIMEOUT; }
                state = gpio_get(pin);
            }
            stopTime = time_us_32();
    // Otherwise block until low.
        } else {
            startTime = time_us_32();
            while (state == true) {
                state = gpio_get(pin);
            }
            stopTime = time_us_32();
        }
    // Return the time delta:
        return (stopTime-startTime);
    }
    /**
     * @brief busy wait until HIGH signal, or timeout. a timeout of 0 (the default)
     *                      means that it will block until HIGH. If the pin is already HIGH, it
     *                      will return immediatly with a value of 0. If timeout is reached it
     *                      returns -4 (MY_TIMEOUT). If GPIO is set as out put it will return -5
     *                      (MY_INVALID_DIR). Assumes gpio_init has already been called.
     * 
     * @param pin Pin to read.
     * @param timeout Timeout time in microseconds.
     * @return Positive = elapsed time in microseconds, negaitve  = error code.
     */
    int32_t waitForHigh32(const uint8_t pin, const uint32_t timeout=0) {
        uint32_t startTime;     // Time started.
        uint32_t timeoutTime;   // absolute timeout time.
        uint32_t stopTime;      // time stopped.
    // Check pin status:
        if (isPin(pin) == false) { return MY_INVALID_PIN; }
        if (gpio_get_dir(pin) == GPIO_OUT) { return MY_INVALID_DIR; }
    // Check if pin is already high:
        bool state = gpio_get(pin);
        if (state == true) { return 0; }
    // If a timeout is requested wait until timeout.
        if (timeout > 0) {
            startTime = time_us_32();
            timeoutTime = startTime + timeout;
            while (state == false) {
                if (time_us_32() >= timeout) { return MY_TIMEOUT; }
                state = gpio_get(pin);
            }
            stopTime = time_us_32();
    // Otherwise block until high.
        } else {
            startTime = time_us_32();
            while (state == false) {
                state = gpio_get(pin);
            }
            stopTime = time_us_32();
        }
    // Return the time delta:
        return (stopTime-startTime);
    }
    /**
     * @brief busy wait until LOW signal, or timeout.
     *                      Busy wait until LOW signal or timeout. A timeout of 0 (the default)
     *                      means that it will block until LOW. If the pin is already LOW, it
     *                      will return immediatly with a value of 0. If timeout is reached it
     *                      returns -4 (MY_TIMEOUT). If GPIO is set as out put it will return -5
     *                      (MY_INVALID_DIR). Assumes gpio_init has already been called.
     * 
     * @param pin pin to read
     * @param timeout timeout time in microseconds
     * @return positive = elapsed time in microseconds, negative = error code.
     */
    int64_t waitForLow64(const uint8_t pin, const uint64_t timeout=0) {
        uint64_t startTime;     // Time started.
        uint64_t timeoutTime;   // absolute timeout time.
        uint64_t stopTime;      // time stopped.
    // Check pin status:
        if (isPin(pin) == false) { return MY_INVALID_PIN; }
        if (gpio_get_dir(pin) == GPIO_OUT) { return MY_INVALID_DIR; }
    // Check if pin is already low:
        bool state = gpio_get(pin);
        if (state == false) { return 0; }
    // If a timeout is requested wait until timeout.
        if (timeout > 0) {
            startTime = time_us_64();
            timeoutTime = startTime + timeout;
            while (state == true) {
                if (time_us_64() >= timeout) { return MY_TIMEOUT; }
                state = gpio_get(pin);
            }
            stopTime = time_us_64();
    // Otherwise block until low.
        } else {
            startTime = time_us_64();
            while (state == true) {
                state = gpio_get(pin);
            }
            stopTime = time_us_64();
        }
    // Return the time delta:
        return (stopTime-startTime);
    }
    /**
     * @brief busy wait until HIGH signal, or timeout. a timeout of 0 (the default)
     *                      means that it will block until HIGH. If the pin is already HIGH, it
     *                      will return immediatly with a value of 0. If timeout is reached it
     *                      returns -4 (MY_TIMEOUT). If GPIO is set as out put it will return -5
     *                      (MY_INVALID_DIR). Assumes gpio_init has already been called.
     * 
     * @param pin Pin to readk.
     * @param timeout Timeout time in microseconds
     * @return int64_t Positive time elapsed, negaitve error code.
     */
    int64_t waitForHigh64(const uint8_t pin, const uint64_t timeout=0) {
        uint64_t startTime;     // Time started.
        uint64_t timeoutTime;   // absolute timeout time.
        uint64_t stopTime;      // time stopped.
    // Check pin status:
        if (isPin(pin) == false) { return MY_INVALID_PIN; }
        if (gpio_get_dir(pin) == GPIO_OUT) { return MY_INVALID_DIR; }
    // Check if pin is already High:
        bool state = gpio_get(pin);
        if (state == true) { return 0; }
    // If a timeout is requested wait until timeout.
        if (timeout > 0) {
            startTime = time_us_64();
            timeoutTime = startTime + timeout;
            while (state == false) {
                if (time_us_64() >= timeout) { return MY_TIMEOUT; }
                state = gpio_get(pin);
            }
            stopTime = time_us_64();
    // Otherwise block until low.
        } else {
            startTime = time_us_64();
            while (state == false) {
                state = gpio_get(pin);
            }
            stopTime = time_us_64();
        }
    // Return the time delta:
        return (stopTime-startTime);
    }
    /**
     * @brief busy wait until pin change, or timeout. a timeout of 0 (the default)
     *                      means that it will block until change. If timeout is reached it
     *                      returns -4 (MY_TIMEOUT). If GPIO is set as output it will return -5
     *                      (MY_INVALID_DIR). Assumes gpio_init has already been called.
     * 
     * @param pin Pin to read.
     * @param timeout Timeout time in microseconds.
     * @return int32_t Positive = time elapsed in microseconds, negative = error code.
     */
    int32_t waitForChange32(const uint8_t pin, const uint32_t timeout) {
        uint32_t startTime;     // Time started.
        uint32_t timeoutTime;   // absolute timeout time.
        uint32_t stopTime;      // time stopped.
        bool     state;         // the current state of the pin.
        bool     waitState;     // the state to wait for.
    // Check pin status:
        if (isPin(pin) == false) { return MY_INVALID_PIN; }
        if (gpio_get_dir(pin) == GPIO_OUT) { return MY_INVALID_DIR; }
    // Set the state we're waiting for.
        waitState = not(gpio_get(pin));
    // If a timeout is requested wait until timeout.
        if (timeout > 0) {
            startTime = time_us_32();
            timeoutTime = startTime + timeout;
            while (state != waitState) {
                if (time_us_32() >= timeout) { return MY_TIMEOUT; }
                state = gpio_get(pin);
            }
            stopTime = time_us_32();
    // Otherwise block until change.
        } else {
            startTime = time_us_32();
            while (state != waitState) {
                state = gpio_get(pin);
            }
            stopTime = time_us_32();
        }
    // Return the time delta:
        return (stopTime-startTime);
    }
    /**
     * @brief busy wait until pin change, or timeout. a timeout of 0 (the default)
     *                      means that it will block until change. If timeout is reached it
     *                      returns -4 (MY_TIMEOUT). If GPIO is set as output it will return -5
     *                      (MY_INVALID_DIR). Assumes gpio_init has already been called.
     * 
     * @param pin Pin to read.
     * @param timeout Timeout time in microseconds.
     * @return int64_t Positive = time elapsed in microseconds, negaive = error code.
     */
    int64_t waitForChange32(const uint8_t pin, const uint64_t timeout) {
        uint32_t startTime;     // Time started.
        uint32_t timeoutTime;   // absolute timeout time.
        uint32_t stopTime;      // time stopped.
        bool     state;         // the current state of the pin.
        bool     waitState;     // the state to wait for.
    // Check pin status:
        if (isPin(pin) == false) { return MY_INVALID_PIN; }
        if (gpio_get_dir(pin) == GPIO_OUT) { return MY_INVALID_DIR; }
    // Set the state we're waiting for.
        waitState = not(gpio_get(pin));
    // If a timeout is requested wait until timeout.
        if (timeout > 0) {
            startTime = time_us_64();
            timeoutTime = startTime + timeout;
            while (state != waitState) {
                if (time_us_64() >= timeout) { return MY_TIMEOUT; }
                state = gpio_get(pin);
            }
            stopTime = time_us_64();
    // Otherwise block until change.
        } else {
            startTime = time_us_64();
            while (state != waitState) {
                state = gpio_get(pin);
            }
            stopTime = time_us_64();
        }
    // Return the time delta:
        return (stopTime-startTime);
    }
    /**
     * @brief Read a pulse on a pin 32 bit. 
     * 
     * @param pin Pin to read pulse on.
     * @param state True waits for a high pulse, False waits for a low pulse.
     * @param timeout Timeout time in microseconds. A timeout value of 0 (the default) blocks indefinatly.
     * @return int32_t Positive = time elapsed in microseconds, negative = error code.
     */
    int32_t pulseIn32(const uint8_t pin, const bool state, uint32_t timeout=0) {
        uint32_t startTime;
        uint32_t stopTime;
        uint32_t timeoutTime;
    // Check Pin:
        if (myHelpers::isPin(pin) == false) { return MY_INVALID_PIN; }
        if (gpio_get_dir(pin) == GPIO_OUT) { return MY_INVALID_DIR; }
    // If a timeout was requested:
        if (timeout > 0){
            startTime = time_us_32();
            timeoutTime = startTime + timeout;
        // Wait for previous pulse to end:
            while (gpio_get(pin) == state) {
                if (time_us_32() >= timeoutTime) { return MY_TIMEOUT; }
            }
        // Wait for pulse to start
            while (gpio_get(pin) != state) {
                if (time_us_32() >= timeoutTime) { return MY_TIMEOUT; }
            }
        // Wait for pulse to stop.
            while (gpio_get(pin) == state) {
                if (time_us_32() >= timeoutTime) { return MY_TIMEOUT; }
            }
            stopTime = time_us_32();

        } else {
        // Wait for previous pulse to end:
            while (gpio_get(pin) == state) {
                tight_loop_contents();
            }
        // Wait for pulse to start
            while (gpio_get(pin) != state) {
                tight_loop_contents();
            }
        // Wait for pulse to stop.
            while (gpio_get(pin) == state) {
                tight_loop_contents();
            }
            stopTime = time_us_32();
        }
    // Return the time delta:
        return (stopTime - startTime);
    }
    /**
     * @brief Read a pulse on a pin 64 bit. 
     * 
     * @param pin Pin to read pulse on.
     * @param state True waits for a high pulse, False waits for a low pulse.
     * @param timeout Timeout time in microseconds. A timeout value of 0 (the default) blocks indefinatly.
     * @return int64_t Positive = time elapsed in microseconds, negative = error code.
     */
    int64_t pulseIn64(const uint8_t pin, const bool state, uint64_t timeout=0) {
        uint64_t startTime;
        uint64_t stopTime;
        uint64_t timeoutTime;
    // Check Pin:
        if (myHelpers::isPin(pin) == false) { return MY_INVALID_PIN; }
        if (gpio_get_dir(pin) == GPIO_OUT) { return MY_INVALID_DIR; }
    // If a timeout was requested:
        if (timeout > 0){
            startTime = time_us_64();
            timeoutTime = startTime + timeout;
        // Wait for previous pulse to end:
            while (gpio_get(pin) == state) {
                if (time_us_64() >= timeoutTime) { return MY_TIMEOUT; }
            }
        // Wait for pulse to start
            while (gpio_get(pin) != state) {
                if (time_us_64() >= timeoutTime) { return MY_TIMEOUT; }
            }
        // Wait for pulse to stop.
            while (gpio_get(pin) == state) {
                if (time_us_64() >= timeoutTime) { return MY_TIMEOUT; }
            }
            stopTime = time_us_64();

        } else {
        // Wait for previous pulse to end:
            while (gpio_get(pin) == state) {
                tight_loop_contents();
            }
        // Wait for pulse to start
            while (gpio_get(pin) != state) {
                tight_loop_contents();
            }
        // Wait for pulse to stop.
            while (gpio_get(pin) == state) {
                tight_loop_contents();
            }
            stopTime = time_us_64();
        }
    // Return the time delta:
        return (stopTime - startTime);
    }
    /**
     * @brief Map a value from one range to another.
     * 
     * @param x Value to map
     * @param in_min In range minimum
     * @param in_max In range maximum
     * @param out_min Out range minimum.
     * @param out_max Out range maximum.
     * @return int32_t The mapped value.
     */
    int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max) {
        return (x - in_min) * (out_max-out_min) / (in_max - in_min) + out_min;
    }
};

#endif