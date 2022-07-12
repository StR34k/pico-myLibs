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
    #ifdef RASPBERRYPI_PICO
        if (pin == 23) { return false; }
        if (pin == 24) { return false; }
        if (pin <= MY_MAX_PIN) { return true; }
        return false;
    #else
        if (pin <= MY_MAX_PIN) { return true; }
        return false;
    #endif
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
     *                      returns -7 (MY_TIMEOUT). If GPIO is set as out put it will return -8
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
     *                      returns -7 (MY_TIMEOUT). If GPIO is set as out put it will return -8
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
     *                      returns -7 (MY_TIMEOUT). If GPIO is set as out put it will return -8
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
     * @brief busy wait until HIGH signal, or timeout. 
     *                      A timeout of 0 (the default)
     *                      means that it will block until HIGH. If the pin is already HIGH, it
     *                      will return immediatly with a value of 0. If timeout is reached it
     *                      returns -7 (MY_TIMEOUT). If GPIO is set as out put it will return -8
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
     *                      returns -7 (MY_TIMEOUT). If GPIO is set as output it will return -8
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
     *                      returns -7 (MY_TIMEOUT). If GPIO is set as output it will return -8
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
/************************** MAP functions: **************************/
    /**
     * @brief Map a value from one range to another. Template.
     * 
     * @param x Value to map
     * @param in_min In range minimum
     * @param in_max In range maximum
     * @param out_min Out range minimum.
     * @param out_max Out range maximum.
     * @return int32_t The mapped value.
     */
    template <class T> T map( T x, T in_min, T in_max, T out_min, T out_max) {
        return (x - in_min) * (out_max-out_min) / (in_max - in_min) + out_min;
    }

/****************************** Reverse functions: ***********************************/
    /**
     * @brief Reverse an 8 bit word.
     * Sets the msb = to lsb. and so on until the word is reversed. used to convert
     * from MSB_FIRST to LSB_FIRST because the hardware SPI doesn't support LSB_FIRST.
     * @param value Word to reverse.
     * @return uint8_t Reversed word.
     */
    uint8_t reverse8(const uint8_t value) {
        uint8_t returnValue = 0x00;
        for (uint8_t i=0; i<8; i++) {
            returnValue <<= 1;
            if ((bool)(value & (1<<i)) == true) {
                returnValue |= 0x01;
            }
        }
        return returnValue;
    }
    /**
     * @brief Reverse a 16 bit word.
     * Sets MSB = to LSB, and so on until the word is reversed. Used to convert
     * from MSB_FIRST to LSB_FIRST because the hardware SPI doesn't support LSB_FIRST.
     * @param value Word to reverse.
     * @return uint16_t Reversed word.
     */
    uint16_t reverse16(const uint16_t value) {
        uint16_t returnValue = 0x0000;
        for (uint8_t i=0; i<16; i++) {
            returnValue <<= 1;
            if ((bool)(value& (1 << i)) == true) {
                returnValue |= 0x01;
            }
        }
        return returnValue;
    }
/******************** Date time functions: **********************/

    /**
     * @brief Validate a year value with centuries, IE: 2020.
     * Returns true if year is in the range 0-25599. Since years without a century are assumed to 
     * be year 2000-2099, they are treated as valid years.
     * @param value Year to validate.
     * @return true Year is a valid year.
     * @return false Year is an invalid year.
     */
    bool isValidYear(const int16_t value) {
        if (value < 0 or value > MY_MAX_YEAR) { return false; }
        return true;
    }
    /**
     * @brief Convert a year without centuries to a year with centuries.
     * Years in the range 0-99 are assumed to be years 2000-2099. If a year
     * with centuries is passed, then it's returned untouched. If an invalid
     * year is passed, an error code is returned.
     * @param year Year to convert.
     * @return int16_t Postive converted year, negative for error code.
     */
    int16_t convertYear(int16_t year) {
        if (year < 0 or year > MY_MAX_YEAR) { return MY_HELPERS_INVALID_YEAR; }
        if (year >= MY_MIN_YEAR) { return year; }
        return year + MY_DEFAULT_CENTURY * MY_YEARS_PER_CENTURY;
    }
    /**
     * @brief Check leap year. 
     * Check a year to see if it's a leap year. This works for both years with and without centuries,
     * since years without centuries are assumed to be years 2000-2099. If an invalid year is passed,
     * it will return False.
     * @param year Year to check.
     * @return true Year is a leap year.
     * @return false Year is not a leap year. Or is an invalid year.
     */
    bool inline isLeapYear(const int16_t year) {
        if (isValidYear(year) == false) { return false; }
        int16_t y = convertYear(year);
        if ( (y % 4) == 0) {
            if ((y % 100) == 0) {
                if ((y % 400) == 0) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return true;
            }
        }
        return false;
    }
    /**
     * @brief Get the number of days in a year.
     * Returns the number of days in a given year, if it's a common year it returns 365, if it's
     * a leap year, it returns 366. This works for years with and without centuries as years without
     * centuries are assumed to be 2000-2099. If an invalid year is passed, an error code is returned.
     * @param year Year to get number of days.
     * @return int16_t Postivie is the number of days in the year. Negative for error code.
     */
    int16_t getDaysInYear(int16_t year) {
        if (isValidYear(year) == false) { return MY_HELPERS_INVALID_YEAR; }
        int16_t y = convertYear(year);
        if (isLeapYear(y) == true) { return MY_DAYS_PER_LEAP_YEAR; }
        return MY_DAYS_PER_YEAR;
    }
    /**
     * @brief Validate a month value.
     * Returns true if value is in range (1-12).
     * @param value Month to check.
     * @return true Month is valid.
     * @return false Month is invalid.
     */
    bool isValidMonth(const int8_t value) {
        if (value < MY_MIN_MONTH or value > MY_MAX_MONTH) { return false; }
        return true;
    }
    /**
     * @brief Get the number of days in a month.
     * Returns the number of days in a month given the year and month. This works for years with and
     * without centuries.
     * @param year Year.
     * @param month Month.
     * @return uint8_t Number of days in the month.
     */
    int8_t getDaysInMonth(const int16_t year, const int8_t month) {
        if (isValidYear(year) == false) { return MY_HELPERS_INVALID_YEAR; }
        if (isValidMonth(month) == false) { return MY_HELPERS_INVALID_MONTH; }
        const uint8_t DAYS_PER_MONTH[MY_MONTHS_PER_YEAR] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (isLeapYear(year) == true and month == MY_MONTH_FEB) {
            return (DAYS_PER_MONTH[month-1] + 1);
        } 
        return DAYS_PER_MONTH[month-1];
    }
    /**
     * @brief Validate a day of month.
     * Returns true if a day of the month is in the range (1-[28-31]) depending on year and month provided.
     * If an invalid month or year value is passed, it will return false. This works for both years with
     * and without centuries. as it assumes that values 0-99 are Century 2000.
     * @param year Year day lands in.
     * @param month Month day lands in.
     * @param value Day value to check.
     * @return true Day of month is valid.
     * @return false Day of month is invalid.
     */
    bool isValidDayOfMonth(const int16_t year, const int8_t month, const int8_t value) {
        if (isValidYear(year) == false) { return false; }
        if (isValidMonth(month) == false) { return false; }
        if (value < MY_MIN_DATE or value > getDaysInMonth(year, month)) { return false; }
        return true;
    }
    /**
     * @brief Validate a day of the week value.
     * Returns true if value in range (1-7), where 1=Sunday.
     * @param value Day of the week to validate.
     * @return true Day of the week is valid.
     * @return false Day of the week is invalid.
     */
    bool isValidDayOfWeek(int8_t value) {
        if (value < MY_MIN_DOTW or value > MY_MAX_DOTW) { return false; }
        return true;
    }
    /**
     * @brief Get the day of the week.
     * Returns the day of the week, where 1=Sunday. Years between 00-99 are assumed to be
     * years 2000-2099.
     * @param year Year day lands in.
     * @param month Month day lands in.
     * @param day Day of month.
     * @return int8_t Returns (1-7) where 1=Sunday.
     */
    int8_t getDayOfWeek(const int16_t year, const int8_t month, const int8_t day) {
        static const uint8_t T[12] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
        if (isValidYear(year) == false) { return MY_HELPERS_INVALID_YEAR; }
        if (isValidMonth(month) == false) { return MY_HELPERS_INVALID_MONTH; }
        if (isValidDayOfMonth(year, month, day) == false) { return MY_HELPERS_INVALID_DATE; }
        int16_t y = convertYear(year);
        if (month < 3) { y -= 1; }
        return ((y + y/4 - y/100 + y/400 + T[month-1] + day) % 7)+1;
    }
    /**
     * @brief Validate a date.
     * Returns true if a date given is valid. If a date with no centuries is provided (00-99), then
     * it's assumed to be years 2000-2099.
     * @param year Year of date.
     * @param month Month of date.
     * @param day Day of date.
     * @return true Date is valid.
     * @return false Date is invalid.
     */
    bool isValidDate(const int8_t year, const int8_t month, const int8_t day) {
        if (isValidYear(year) == false) { return false; }
        if (isValidMonth(month) == false) { return false; }
        if (isValidDayOfMonth(year, month, day) == false) { return false; }
        return true;
    }
    /**
     * @brief Validate an hour value.
     * Returns true if value is in range (0-23) in 24h format, or (1-12) in 12h format.
     * @param value Hour to validate.
     * @param is12h False = 24h format, True = 12h format.
     * @return true Hour is valid.
     * @return false Hour is invalid.
     */
    bool isValidHour(const int8_t value, const bool is12h) {
        if (is12h == MY_IS_12H) {
            if (value < MY_MIN_HOUR_12H or value > MY_MAX_HOUR_12H) { return false; }
        } else {
            if (value < MY_MIN_HOUR_24H or value > MY_MAX_HOUR_24H) { return false; }
        }
        return true;
    }
    /**
     * @brief Validate a minute value.
     * Returns true if minute is in range 0-59.
     * @param value Minute to validate.
     * @return true Minute is valid.
     * @return false Minute is invalid.
     */
    bool isValidMinute(const int8_t value) {
        if (value < MY_MIN_MINUTE or value > MY_MAX_MINUTE) { return false; }
        return true;
    }
    /**
     * @brief Validate a second value.
     * Returns true if second is in range 0-59.
     * @param value Second to validate.
     * @return true Second is valid.
     * @return false Second is not valid.
     */
    bool isValidSecond(const int8_t value) {
        if (value < MY_MIN_SECOND or value > MY_MAX_SECOND) { return false; }
        return true;
    }
    /**
     * @brief Validate a time.
     * Returns true if a time is valid.
     * @param hour Hour of time.
     * @param minute Minute of time.
     * @param second Second of time.
     * @param is12h Is 12hr format, False=24h, True=12h.
     * @return true Time is valid.
     * @return false Time is invalid.
     */
    bool isValidTime(const int8_t hour, const int8_t minute, const int8_t second, const bool is12h=false) {
        if (is12h == MY_IS_12H) {
            if (hour < MY_MIN_HOUR_12H or hour > MY_MAX_HOUR_12H) { return false; }
        } else {
            if (hour < MY_MIN_HOUR_24H or hour > MY_MAX_HOUR_24H) { return false; }
        }
        if (minute < MY_MIN_MINUTE or minute > MY_MAX_MINUTE) { return false; }
        if (second < MY_MIN_SECOND or second > MY_MAX_SECOND) { return false; }
        return true;
    }
    /**
     * @brief Convert from 12h format to 24h format.
     * Converts from 12h format to 24h format. Returns 0 for convert okay, otherwise if 
     * an invlid hour is passed, an error code is returned.
     * @param hours12 Hours in 12h format.
     * @param isPM False = AM, True = PM
     * @param hours24 Returned hours in 24h format.
     * @return int16_t Retuns 0 for convert okay, negative for error code.
     */
    int16_t convert12hTo24h(const uint8_t hours12, const bool isPM, uint8_t *hours24) {
        if (isValidHour(hours12, true) == false) { return MY_HELPERS_INVALID_HOUR; }
        if (isPM == true) {
            if (hours12 < 12) {
                *hours24 = hours12 + 12;
            } else {
                *hours24 = hours12;
            }
        } else {
            if (hours12 == 12) {
                *hours24 = 0;
            } else {
                *hours24 = hours12;
            }
        }
        return MY_NO_ERROR;
    }
    /**
     * @brief Convert from 24h format to 12h format.
     * Converts from 24h format to 12h format, returns 0 if converted okay, otherwise if an
     * invalid hour is passed, an error code is returned.
     * @param hours24 Time in 24h format.
     * @param hours12 Returned hours in 12h format.
     * @param isPM False = AM, True = PM.
     * @return int16_t Returns 0 (MY_NO_ERROR) if convert okay, negative for error code.
     */
    int16_t convert24hTo12h(const uint8_t hours24, uint8_t *hours12, bool *isPM) {
        if (isValidHour(hours24, false) == false) { return MY_HELPERS_INVALID_HOUR; }
        if ( hours24 == 0) {
            *hours12 = 12;
            *isPM = false;
        } else {
            if (hours24 > 12) {
                *hours12 = hours24 - 12;
                *isPM = true;
            } else if (hours24 == 12) {
                *hours12 = hours24;
                *isPM = true;
            } else {
                *hours12 = hours24;
                *isPM = false;
            }
        }
        return MY_NO_ERROR;
    }
/*************************** Shift in / out functions ***************************/
    int8_t shiftOut(const uint8_t clockPin, const uint8_t dataPin, const uint8_t *buffer, const size_t len, 
                            const bool bitOrder=MSB_FIRST) {
        if (isPin(clockPin) == false) { return MY_INVALID_PIN; }
        if (isPin(dataPin) == false) { return MY_INVALID_PIN; }
        if (gpio_get_function(clockPin) != GPIO_FUNC_SIO) { return MY_INVALID_FUNC; }
        if (gpio_get_function(dataPin) != GPIO_FUNC_SIO) { return MY_INVALID_FUNC; }
        if (gpio_get_dir(clockPin) != GPIO_OUT) { return MY_INVALID_DIR; }
        if (gpio_get_dir(dataPin) != GPIO_OUT) { return MY_INVALID_DIR; }
        for (size_t i=0; i<len; i++) {
            for (uint8_t j=0; j<8; j++) {
                uint8_t val = buffer[i];
                if (bitOrder == MSB_FIRST) {
                    gpio_put(dataPin, (bool)(val & 0x80));
                    val <<= 1;
                } else {
                    gpio_put(dataPin, (bool)(val & 0x01));
                    val >>= 1;
                }
                gpio_put(clockPin, true);
                gpio_put(clockPin, false);
            }
        }
        return MY_NO_ERROR;
    }

    int8_t shiftIn(const uint8_t clockPin, const uint8_t dataPin, uint8_t *buffer, const size_t len,
                        const bool bitOrder=MSB_FIRST) {
        if (isPin(clockPin) == false) { return MY_INVALID_PIN; }
        if (isPin(dataPin) == false) { return MY_INVALID_PIN; }
        if (gpio_get_function(clockPin) != GPIO_FUNC_SIO) { return MY_INVALID_FUNC; }
        if (gpio_get_function(dataPin) != GPIO_FUNC_SIO) { return MY_INVALID_FUNC; }
        if (gpio_get_dir(clockPin) != GPIO_OUT) { return MY_INVALID_DIR; }
        if (gpio_get_dir(dataPin) != GPIO_IN) { return MY_INVALID_DIR; }
        for (size_t i=0; i< len; i++) {
            uint8_t value;
            for (uint8_t j=0; i<8; j++) {
                gpio_put(clockPin, true);
                if (bitOrder == MSB_FIRST) {
                    value <<=1;
                    value |= (gpio_get(dataPin) ? 0x01 : 0x00);
                } else {
                    value >>=1;
                    value |= (gpio_get(dataPin) ? 0x80 : 0x00);
                }
                gpio_put(clockPin, false);
            }
            buffer[i] = value;
        }
        return MY_NO_ERROR;
    }

/****************** print Binary format: ******************************/
    template <class T> void printBin(const T &value,const bool printSpaces=true,
                                            const bool printPrefix=true) {
        uint8_t size = 8 * sizeof(value);
        if (printPrefix == true) {
            printf("0b");
            if (printSpaces == true) { printf(" "); }
        }
        for (uint8_t i=0; i<size; i++) {
            if (i%4 == 0 and i != 0 and printSpaces == true) {printf(" "); }
            bool bitValue = bitRead( value, (size-i-1) );
            printf("%i",bitValue);
        }
    }



};

#endif