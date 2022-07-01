/**
 * @file myDS1307.hpp
 * @author Peter Nearing (pnearing@protonmail.com)
 * @brief Class to use a DS1307 Real time clock.
 * @version 0.1
 * @date 2022-06-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MY_DS1307_H
#define MY_DS1307_H

#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <pico/types.h>

#include "../myStandardDefines.hpp"
#include "../myErrorCodes.hpp"
#include "../myHelpers.hpp"
#include "../myI2C/myI2C.hpp"

/**
 * @brief Class to use a DS1307 real time clock.
 * 
 */
class myDS1307 {

    public:
/* ################ Public Constants: ################ */
    // Error constants:
        /**
         * @brief No error. Value 0.
         * No error occured.
         */
        const int16_t NO_ERROR = MY_NO_ERROR;
        /**
         * @brief Timeout. Value -7.
         * The device timedout during read / write operation.
         */
        const int16_t ERROR_TIMEOUT = MY_TIMEOUT;
        /**
         * @brief Invalid SDA pin. Value -450.
         * Invalid SDA pin provided.
         */
        const int16_t ERROR_INVALID_SDA_PIN = MY_ERROR_MYI2C_INVALID_SDA_PIN;
        /**
         * @brief Invalid SCL pin. Value -451.
         * Invalid SCL pin provided.
         */
        const int16_t ERROR_INVALID_SCL_PIN = MY_ERROR_MYI2C_INVALID_SCL_PIN;
        /**
         * @brief I2C already initialized. Value -452.
         * The I2C port was already initialized when initialize was called.
         */
        static const int16_t ERROR_INITIALIZED = MY_ERROR_MYI2C_INITIALIZED;
        /**
         * @brief I2C address not responding. Value -453.
         * The chip is not responding.
         */
        static const int16_t ERROR_NOT_RESPONDING = MY_ERROR_MYI2C_ADDRESS_NOT_RESPONDING;

    // Rate Select values:
        /**
         * @brief Rate 1 Hz.
         * Set square wave output to 1 Hz.
         */
        static const uint8_t RATE_1HZ = 0x00;
        /**
         * @brief Rate 4.096 KHz.
         * Set square wave output to 4.096 KHz.
         */
        static const uint8_t RATE_4KHZ = 0x01;
        /**
         * @brief Rate 8.192 KHz.
         * Set square wave output to 8.192 KHz.
         */
        static const uint8_t RATE_8KHZ = 0x02;
        /**
         * @brief Rate 32.768 KHz.
         * Set square wave output to 32.768 KHz.
         */
        static const uint8_t RATE_32KHZ = 0x03;
/* ############### Constructor ############### */
    myDS1307(i2c_inst_t *i2cPort, const uint8_t sdaPin, const uint8_t sclPin): _sdaPin (sdaPin), 
                    _sclPin (sclPin) {
        _i2cPort = i2cPort;
    }

/* ################ Public Functions: ################ */
    /**
     * @brief Get the hour mode.
     * Get the 12H / 24H mode and return it. Returns 1 for 12H mode, 0 for 24H mode, or if
     * a comms error occured, it returns an error code.
     * @return int16_t Returns 1 for 12H mode, 0 for 24H mode, or negative for error code.
     */
    int16_t getMode();
    /**
     * @brief Get the Seconds.
     * Read and return the seconds. If a comms error occurs then a error code is returned.
     * @return int16_t Positive (00-59) is the seconds, negative is an error code.
     */
    int16_t getSeconds();
    /**
     * @brief Get the Minutes.
     * Read and return the minutes. If a comms error occurs, then an error code is returned.
     * @return int16_t Positive (00-59) is the minutes, negative is an error code.
     */
    int16_t getMinutes();
    /**
     * @brief Get the hours in 12 hour format.
     * Return the hours in 12 hour format. If a communications error occurs, then an error
     * code is returned.
     * @return int16_t Positive (1-12) is the hour. Negative is error code.
     */
    int16_t getHour12H();
    /**
     * @brief Get the hours in 24 hour format.
     * Returns the hours in 24 hour format. If a comm error occurs, then an error code is returned.
     * @return int16_t Positive (0-23) is the hour. Negative is error code.
     */
    int16_t getHour24H();
    /**
     * @brief Get if it's AM/PM.
     * Gets am/pm and returns it, 0 for AM, 1 for PM, if a comms error occurs, then an error code is
     * returned.
     * @return int16_t 0=AM, 1=PM, negative for error code.
     */
    int16_t getAmPm();
    /**
     * @brief Get the day of week.
     * Read and return the day of week. This is user programable. So when we set it we set Sunday 
     * equal to 1. Until it's set, we can't confirm what it is, since the datasheet doesn't list
     * a default value. If a comms error occurs during read, an error code is returned.
     * @return int16_t Positive(1-7) is the day, negative is error code.
     */
    int16_t getDay();
    /**
     * @brief Get the date (day of month)
     * Read and return the day of month. Positve values are the date, while if an error
     * occurs during comms, an error code is returned.
     * @return int16_t Positive (1-31) day of month, negative is error code.
     */
    int16_t getDate();
    /**
     * @brief Get the month.
     * Read and return the month. Positive values are the month number, with 01 being January.
     * if an error occurs during comms, an error code is returned.
     * @return int16_t Positive (1-12) month, negative for error code.
     */
    int16_t getMonth();
    /**
     * @brief Get the year.
     * Read and return the year. Positive values are the year, if an error occurs during
     * comms, an error code is returned.
     * @note Centuries are not tracked, so I'm assuimg 00=2000. Sorry for introducing a year 2099 bug.
     * @return int16_t Positve is the year, negative is an error code.
     */
    int16_t getYear();
    /**
     * @brief Get the current date / time.
     * Reads and returns the current date / time as a pico datetime_t struct. If a comms error occurs
     * then all the values will be set to -1, except for the year, which will be set to the error code.
     * @note Centuries are not tracked, so I'm assuimg 00=2000. Sorry for introducing a year 2099 bug.
     * @return datetime_t 
     */
    datetime_t getDateTime();
    /**
     * @brief Initialize the chip.
     * Initialize communications with the chip. Returns 0(NO_ERROR) for okay, otherwise if
     * an invalid pin is passed during construction, or a comms error occurs, an error code
     * is returned.
     * @return int16_t Returns 0 (NO_ERROR) if initialized okay, negative for error code.
     */
    int16_t initialize();

    private:
/* ############# Private Constants: ############# */
    // I2C Constants:
        const uint8_t I2C_ADDRESS       = 0X68;
        const uint32_t I2C_TIMEOUT_US   = 20000; // 20 MILLISECONDS.
    // Register Addresses:
        const uint8_t REG_SECONDS       = 0x00;
        const uint8_t REG_MINUTES       = 0x01;
        const uint8_t REG_HOURS         = 0x02;
        const uint8_t REG_DAY           = 0X03;
        const uint8_t REG_DATE          = 0X04;
        const uint8_t REG_MONTH         = 0x05;
        const uint8_t REG_YEAR          = 0x06;
        const uint8_t REG_CONTROL       = 0x07;
        const uint8_t REG_SRAM_START    = 0x08;
        const uint8_t REG_SRAM_STOP     = 0x3F;
    // Register Masks:
        const uint8_t MASK_CLOCK_HALT       = 0x80;
        const uint8_t MASK_TENS_SECONDS     = 0x70;
        const uint8_t MASK_SECONDS          = 0x0F;
        const uint8_t MASK_TENS_MINUTES     = 0x70;
        const uint8_t MASK_MINUTES          = 0x0F;
        const uint8_t MASK_12H_24H          = 0x40;
        const uint8_t MASK_12H_AM_PM        = 0x20;
        const uint8_t MASK_12H_TENS_HOURS   = 0x10;
        const uint8_t MASK_24H_TENS_HOURS   = 0x30;
        const uint8_t MASK_HOURS            = 0x0F;
        const uint8_t MASK_TENS_DATE        = 0x10;
        const uint8_t MASK_DATE             = 0x0F;
        const uint8_t MASK_TENS_MONTHS      = 0X10;
        const uint8_t MASK_MONTHS           = 0x0F;
        const uint8_t MASK_TENS_YEARS       = 0xF0;
        const uint8_t MASK_YEARS            = 0x0F;
        const uint8_t MASK_CONTROL_OUT      = 0x80;
        const uint8_t MASK_CONTROL_SQWE     = 0x10;
        const uint8_t MASK_CONTROL_RS       = 0X03;
        
    // Pins:
        const uint8_t _sdaPin;
        const uint8_t _sclPin;
/* ############# Private Variables: ############## */
    // I2C variables:
        i2c_inst_t *_i2cPort;
    // State variables:
        bool        _is12h = false; // False 24h True 12h
        bool        _isPM = false;
/* ################ Private Functions: ################ */
    int16_t __readRegisters__(const uint8_t reg, uint8_t *buffer, size_t length);
};

/* ############### Public Functions: ################# */

int16_t myDS1307::getMode() {
    int16_t returnValue;
    uint8_t rawHours;
    uint8_t reg = REG_HOURS;
    returnValue = __readRegisters__(reg, &rawHours, 1);
    if (returnValue < 0) { return returnValue; }
    _is12h = (bool)(rawHours & MASK_12H_24H);
    return (int16_t)_is12h;
}

int16_t myDS1307::getSeconds() {
    int16_t returnValue;
    uint8_t rawSeconds;
    uint8_t seconds;
    returnValue = __readRegisters__(REG_SECONDS, &rawSeconds, 1);
    if (returnValue < 0) { return returnValue; }
    seconds = ((rawSeconds & MASK_TENS_SECONDS) >> 4) * 10;
    seconds += (rawSeconds & MASK_SECONDS);
    return (int16_t)seconds;
}

int16_t myDS1307::getMinutes() {
    int16_t returnValue;
    uint8_t rawMinutes;
    uint8_t minutes;
    returnValue = __readRegisters__(REG_MINUTES, &rawMinutes, 1);
    if (returnValue < 0) { return returnValue; }
    minutes = ((rawMinutes & MASK_TENS_MINUTES) >> 4) * 10;
    minutes += (rawMinutes & MASK_MINUTES);
    return (int16_t)minutes;
}

int16_t myDS1307::getHour12H() {
    int16_t returnValue;
    uint8_t rawHours;
    uint8_t hours;
    returnValue = __readRegisters__(REG_HOURS, &rawHours, 1);
    if (returnValue < 0) { return returnValue; }
    _is12h = (bool)(rawHours & MASK_12H_24H);
    if (_is12h == true) { 
        _isPM = (bool)(rawHours & MASK_12H_AM_PM);
        hours = ((rawHours & MASK_12H_TENS_HOURS) >> 4) * 10;
        hours += (rawHours & MASK_HOURS);
    } else {
        hours = ((rawHours & MASK_24H_TENS_HOURS) >> 4) * 10;
        hours += (rawHours & MASK_HOURS);
        _isPM = (bool)(hours > 11);
    // Convert from 24h to 12h
        if (hours > 12) { hours -= 12; }
        if (hours == 0) { hours = 12; }
    }
    return hours;
}

int16_t myDS1307::getHour24H() {
    int16_t returnValue;
    uint8_t rawHours;
    uint8_t hours;
    returnValue = __readRegisters__(REG_HOURS, &rawHours, 1);
    if (returnValue < 0) { return returnValue; }
    _is12h = (bool)(rawHours & MASK_12H_24H);
    if (_is12h == true) {
        hours = ((rawHours & MASK_12H_TENS_HOURS) >> 4) * 10;
        hours += (rawHours & MASK_HOURS);
        _isPM = (bool)(rawHours & MASK_12H_AM_PM);
    // Convert from 12h to 24h
        if (_isPM == true) {
            hours = hours + 12;
        } else if ( hours == 0 ) {
            hours = 12;
        }
    } else {
        hours = ((rawHours & MASK_24H_TENS_HOURS) >> 4) * 10;
        hours += (rawHours & MASK_HOURS);
        _isPM = (bool)(hours > 11);
    }
    return (int16_t)hours;
}

int16_t myDS1307::getAmPm() {
    int16_t returnValue;
    uint8_t rawHours;
    uint8_t hours;
    returnValue = __readRegisters__(REG_HOURS, &rawHours, 1);
    if (returnValue < 0) { return returnValue; }
    _is12h = (bool)(rawHours & MASK_12H_24H);
    if (_is12h == true) {
        _isPM = (bool)(rawHours & MASK_12H_AM_PM);
        return (int16_t)_isPM;
    } else {
        hours = ((rawHours & MASK_24H_TENS_HOURS) >> 4) * 10;
        hours += (rawHours & MASK_HOURS);
        _isPM = (bool)(hours > 11);
        return _isPM;
    }
}

int16_t myDS1307::getDay() {
    int16_t returnValue;
    uint8_t day;
    returnValue = __readRegisters__(REG_DAY, &day, 1);
    if (returnValue < 0) { return returnValue; }
    return (int16_t)day;
}

int16_t myDS1307::getDate() {
    int16_t returnValue;
    uint8_t rawDate;
    uint8_t date;
    returnValue = __readRegisters__(REG_DATE, &rawDate, 1);
    if (returnValue < 0) { return returnValue; }
    date = ((rawDate & MASK_TENS_DATE)>>4) * 10;
    date += (rawDate & MASK_DATE);
    return (int16_t)date;
}

int16_t myDS1307::getMonth() {
    int16_t returnValue;
    uint8_t rawMonth;
    uint8_t month;
    returnValue = __readRegisters__(REG_MONTH, &rawMonth, 1);
    if (returnValue < 0) { return returnValue; }
    month = ((rawMonth & MASK_TENS_MONTHS) >> 4) * 10;
    month += (rawMonth & MASK_MONTHS);
    return (int16_t)month;
}

int16_t myDS1307::getYear() {
    int16_t returnValue;
    uint8_t rawYear;
    int16_t year;
    returnValue = __readRegisters__(REG_YEAR, &rawYear, 1);
    if (returnValue < 0) { return returnValue; }
    year = ((rawYear & MASK_TENS_YEARS)>>4) * 10;
    year += (rawYear & MASK_YEARS);
    year += 2000; // Here is the bug.
    return year;
}

datetime_t myDS1307::getDateTime() {
    int16_t returnValue;
    uint8_t dataBuffer[7];
    datetime_t dateTime;
    returnValue = __readRegisters__(REG_SECONDS, dataBuffer, 7);
    if (returnValue < 0) {
        dateTime.year = returnValue;
        dateTime.month = -1;
        dateTime.day = -1;
        dateTime.dotw = -1;
        dateTime.hour = -1;
        dateTime.min = -1;
        dateTime.sec = -1;
        return dateTime;
    }
    dateTime.sec = (((dataBuffer[0] & MASK_TENS_SECONDS) >> 4) * 10) + (dataBuffer[0] & MASK_SECONDS);
    dateTime.min = (((dataBuffer[1] & MASK_TENS_MINUTES) >> 4) * 10) + (dataBuffer[1] & MASK_MINUTES);
    _is12h = bool(dataBuffer[2] & MASK_12H_24H);
    if (_is12h == true) {
        _isPM = (bool)(dataBuffer[2] & MASK_12H_AM_PM);
        dateTime.hour = (((dataBuffer[2] & MASK_12H_TENS_HOURS) >> 4) * 10) + (dataBuffer[2] & MASK_HOURS);
    // Convert to 24h
        if (_isPM == true) {
            dateTime.hour += 12;
        } else if (dateTime.hour == 0) {
            dateTime.hour = 12;
        }
    } else {
        dateTime.hour = (((dataBuffer[2] & MASK_24H_TENS_HOURS) >> 4) * 10) + (dataBuffer[2] & MASK_HOURS);
    }
    dateTime.dotw = (dataBuffer[3] -1);
    dateTime.day = (((dataBuffer[4] & MASK_TENS_DATE) >> 4) * 10) + (dataBuffer[4] & MASK_DATE);
    dateTime.month = (((dataBuffer[5] & MASK_TENS_MONTHS) >> 4) * 10) + (dataBuffer[5] & MASK_MONTHS);
    dateTime.year = (((dataBuffer[6] & MASK_TENS_YEARS) >> 4) * 10) + (dataBuffer[6] & MASK_YEARS);
    dateTime.year += 2000; // Here's the bug again.
}

int16_t myDS1307::initialize() {
    int32_t returnValue;
    // Initialize i2c at 50 KHz, since we're running at 3.3v which is underspec, we give it more
    // time to respond. Otherwise occasionally the chip will not respond.
    returnValue = myI2C::initializeMaster(_i2cPort, _sdaPin, _sclPin, 50*1000);
    if (returnValue < 0) { return (int16_t)returnValue; }
    // Verify comms by getting the hour byte and determining 
    // We're in 12H or 24H mode:
    returnValue = getMode();
    if (returnValue < 0) { return (int16_t)returnValue; }
    _is12h = (bool)returnValue;
    return NO_ERROR;
}

datetime_t myDS1307::getDateTime() {
    int16_t returnValue;
    uint8_t rawData[7];
    datetime_t dateTime;
    
    return dateTime;
}
/* ############### Private Functions: ################# */

int16_t myDS1307::__readRegisters__(const uint8_t reg, uint8_t *buffer, size_t length) {
    int16_t returnValue;
    returnValue = i2c_write_timeout_us(_i2cPort, I2C_ADDRESS, &reg, 1, true, I2C_TIMEOUT_US);
    if (returnValue < 0) {
        if (returnValue == PICO_ERROR_GENERIC) { return ERROR_NOT_RESPONDING; }
        if (returnValue == PICO_ERROR_TIMEOUT) { return ERROR_TIMEOUT; }
    }
    returnValue = i2c_read_timeout_us(_i2cPort, I2C_ADDRESS, buffer, length, false, I2C_TIMEOUT_US);
    if (returnValue < 0) {
        if (returnValue == PICO_ERROR_GENERIC) { return ERROR_NOT_RESPONDING; }
        if (returnValue == PICO_ERROR_TIMEOUT) { return ERROR_TIMEOUT; }
    }
    return NO_ERROR;
}
#endif