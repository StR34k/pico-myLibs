#ifndef MY_HELPERS_H
#define MY_HELPERS_H

#include <hardware/gpio.h>
#include <hardware/timer.h>
#include <pico/time.h>

#include "myStandardDefines.hpp"
#include "myErrorCodes.hpp"

namespace myHelpers {
    /**
     *  isPin:
     *      Description: Validate if the pin is a valid pin number for the rp2040.
     *      Params: uint8_t pin number.
     *      Returns: true if valid, false if not.
     */
    bool isPin(const uint8_t pin) {
        if (pin < MY_MAX_PIN) { return true; }
        return false;
    }
    /**
     * pinToBitMask:
     *      Description: Return the bit mask for a single pin.
     *      Params: uint8_t pin number.
     *      Returns: Positive is a bitmask, negative is an error code.
     */
    int32_t pinToBitMask(const uint8_t pin) {
        if (isPin(pin) == false ) { return MY_INVALID_PIN; }
        return (1 << pin);
    }
    /**
     * bitMaskToPin:
     *      Description: Return a pin number given a valid bit mask.
     *      Params: uint32_t a bitmask for a single pin.
     *      Return: Positive is pin number, negative is an error code.
     */
    int8_t bitMaskToPin(const uint32_t bitMask) {
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
     * waitForLow32:
     *      Description: busy wait until LOW signal, or timeout. a timeout of 0 (the default)
     *                      means that it will block until LOW. If the pin is already LOW, it
     *                      will return immediatly with a value of 0. If timeout is reached it
     *                      returns -4 (MY_TIMEOUT). If GPIO is set as out put it will return -5
     *                      (MY_INVALID_DIR). Assumes gpio_init has already been called.
     *      Params: uint8_t pin to read.
     *              uint32_t timeout in microseconds.
     *      Returns: int32_t Postive is amount of time elapsed in microseconds. Negative is error code.
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
                if (time_us_32() == timeout) { return MY_TIMEOUT; }
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
     * waitForHigh32:
     *      Description: busy wait until HIGH signal, or timeout. a timeout of 0 (the default)
     *                      means that it will block until HIGH. If the pin is already HIGH, it
     *                      will return immediatly with a value of 0. If timeout is reached it
     *                      returns -4 (MY_TIMEOUT). If GPIO is set as out put it will return -5
     *                      (MY_INVALID_DIR). Assumes gpio_init has already been called.
     *      Params: uint8_t pin to read.
     *              uint32_t timeout in microseconds.
     *      Returns: int32_t Postive is amount of time elapsed in microseconds. Negative is error code.
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
                if (time_us_32() == timeout) { return MY_TIMEOUT; }
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
     * waitForLow64:
     *      Description: busy wait until LOW signal, or timeout. a timeout of 0 (the default)
     *                      means that it will block until LOW. If the pin is already LOW, it
     *                      will return immediatly with a value of 0. If timeout is reached it
     *                      returns -4 (MY_TIMEOUT). If GPIO is set as out put it will return -5
     *                      (MY_INVALID_DIR). Assumes gpio_init has already been called.
     *      Params: uint8_t pin to read.
     *              uint64_t timeout in microseconds.
     *      Returns: int64_t Positive: amount of time elapsed in microseconds, Negaitive Error code.
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
                if (time_us_64() == timeout) { return MY_TIMEOUT; }
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
     * waitForHigh64:
     *      Description: busy wait until HIGH signal, or timeout. a timeout of 0 (the default)
     *                      means that it will block until HIGH. If the pin is already HIGH, it
     *                      will return immediatly with a value of 0. If timeout is reached it
     *                      returns -4 (MY_TIMEOUT). If GPIO is set as out put it will return -5
     *                      (MY_INVALID_DIR). Assumes gpio_init has already been called.
     *      Params: uint8_t pin to read.
     *              uint64_t timeout in microseconds.
     *      Returns: int64_t Positive: amount of time elapsed in microseconds, Negaitive Error code.
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
                if (time_us_64() == timeout) { return MY_TIMEOUT; }
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
     * waitForChange32:
     *      Description: busy wait until pin change, or timeout. a timeout of 0 (the default)
     *                      means that it will block until change. If timeout is reached it
     *                      returns -4 (MY_TIMEOUT). If GPIO is set as output it will return -5
     *                      (MY_INVALID_DIR). Assumes gpio_init has already been called.
     *      Params: uint8_t pin to read.
     *              uint32_t timeout in microseconds.
     *      Returns: int32_t Positive: amount of time elapsed in microseconds, Negaitive Error code.
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
                if (time_us_32() == timeout) { return MY_TIMEOUT; }
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
     * waitForChange64:
     *      Description: busy wait until pin change, or timeout. a timeout of 0 (the default)
     *                      means that it will block until change. If timeout is reached it
     *                      returns -4 (MY_TIMEOUT). If GPIO is set as output it will return -5
     *                      (MY_INVALID_DIR). Assumes gpio_init has already been called.
     *      Params: uint8_t pin to read.
     *              uint64_t timeout in microseconds.
     *      Returns: int64_t Positive: amount of time elapsed in microseconds, Negaitive Error code.
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
                if (time_us_64() == timeout) { return MY_TIMEOUT; }
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
};

#endif