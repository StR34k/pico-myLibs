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
 * Class to use a DS1307 Real time clock. Leap year and day of week algorithms taken from:
 * https://artofmemory.com/blog/how-to-calculate-the-day-of-the-week/ . Datasheet:
 * https://datasheets.maximintegrated.com/en/ds/DS1307.pdf
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
        /**
         * @brief Invalid seconds. Value -550.
         * Invalid seconds value was passed. Valid values (0-59).
         */
        static const int16_t ERROR_INVALID_SECONDS = MY_ERROR_MYDS1307_INVALID_SECONDS;
        /**
         * @brief Invalid minutes. Value -551.
         * Invalid minutes value was passed. Valid values (0-59).
         */
        static const int16_t ERROR_INVALID_MINUTES = MY_ERROR_MYDS1307_INVALID_MINUTES;
        /**
         * @brief Invalid hours. Value -552.
         * Invalid hours value was passed. Valid values (0-23 in 24h mode, 1-12 in 12h mode).
         */
        static const int16_t ERROR_INVALID_HOURS = MY_ERROR_MYDS1307_INVALID_HOURS;
        /**
         * @brief Invalid day of the week. Value -553.
         * Invalid day value was passed. Valid values (1-7).
         */
        static const int16_t ERROR_INVALID_DAY = MY_ERROR_MYDS1307_INVALID_DAY;
        /**
         * @brief Invalid date. Value -554.
         * Invalid date value was passed. Valid values (1-31).
         */
        static const int16_t ERROR_INVALID_DATE = MY_ERROR_MYDS1307_INVALID_DATE;
        /**
         * @brief Invalid month. Value -555.
         * Invalid month value was passed. Valid values (1-12).
         */
        static const int16_t ERROR_INVALID_MONTH = MY_ERROR_MYDS1307_INVALID_MONTH;
        /**
         * @brief Invalid year. Value -556.
         * Invalid year value was passed. Valid values (2000-2099).
         */
        static const int16_t ERROR_INVALID_YEAR = MY_ERROR_MYDS1307_INVALID_YEAR;
        /**
         * @brief Invalid rate. Value -557.
         * Invalid rate was passed. Valid values (RATE_*) (0-3).
         */
        static const int16_t ERROR_INVALID_RATE = MY_ERROR_MYDS1307_INVALID_RATE;
        /**
         * @brief Comms check failed. Value -558.
         * The comms check during initialization failed.
         */
        static const int16_t ERROR_COMMS_FAILED = MY_ERROR_MYDS1307_COMMS_CHECK_FAILED;
        /**
         * @brief Boundary crossed. Value -559.
         * The read / write operation would cross a boundary.
         */
        static const int16_t ERROR_BOUNDARY_CROSSED = MY_ERROR_MYDS1307_BOUNDARY_CROSSED;
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
    // Sram constants:
        /**
         * @brief Length of sram.
         * Length of sram in bytes.
         */
        static const uint8_t SRAM_LENGTH = 56;
/* ############### Constructor ############### */
    myDS1307(i2c_inst_t *i2cPort, const uint8_t sdaPin, const uint8_t sclPin): _sdaPin (sdaPin), 
                    _sclPin (sclPin) {
        _i2cPort = i2cPort;
    }

/* ################ Public Functions: ################ */
    /**
     * @brief Get the Day Of Week.
     * Returns the day of the week given a date. Returns 1=Sunday..., otherwise if an invalid date
     * is given, an error code is returned.
     * @param year Year of the date. Valid values (1700-2399).
     * @param month Month of the date. Valid values (1-12).
     * @param day Day of month. Valid values (1-[28-31]) based on month and year.
     * @return int16_t Returns positive for day of week, negative for error code.
     */
    int16_t getDayOfWeek(const uint16_t year, const uint8_t month, const uint8_t day);
    /**
     * @brief Check if leap year.
     * Checks a given year if it's a leap year. Returns true if it's a leapyear.
     * @param year Year to check. Expects full year format. EX: 2021, instead of 21.
     * @return true Year is a leap year.
     * @return false Year is not a leap year.
     */
    bool inline isLeapYear(const uint16_t year);
    /**
     * @brief Check if a date is valid.
     * Checks a given date to see if it's valid, will return an error code with the offending
     * value if it's invalid. 
     * @param year Year. Year is expected to be in full format IE: 2021 instead of 21. Valid values (2000-2099).
     * @param month Month. Valid values (1-12).
     * @param day Day of month. Valid values (1-[28-31]), based on month.
     * @return int16_t Returns 0 (NO_ERROR) if valid, negative for error code.
     */
    int16_t isValidDate(const uint16_t year, const uint8_t month, const uint8_t day);
    /**
     * @brief Validate a datetime_t
     * Validate a datetime_t value.  Returns 0 (NO_ERROR) for okay, otherwise an error code is
     * returned containing the error code for the value that failed.
     * @param value The datetime_t value to check.
     * @return int16_t Returns 0 (NO_ERROR) if okay, negative for error code.
     */
    int16_t isValidDateTime(const datetime_t value);
    /**
     * @brief Get clock enabled.
     * Read and return the clock inhibit bit. Returns 0 for clock enabled, 1 for clock inhibited, 
     * otherwise if a comms error occurs, an error code is returned.
     * @return int16_t Returns 0 for clock enabled, 1 for clock disabled, negative for error code.
     */
    int16_t getEnabled();
    /**
     * @brief Set clock enabled.
     * Set the clock inhibit bit. Returns 0 (NO_ERROR) if set okay, otherwise if a comms error 
     * occurs an error code is returned.
     * @param value True = clock enabled, False = clock disabled.
     * @return int16_t Returns 0 (NO_ERROR) for set okay, negative for error code.
     */
    int16_t setEnabled(const bool value);
    /**
     * @brief Get the hour mode.
     * Get the 12H / 24H mode and return it. Returns 1 for 12H mode, 0 for 24H mode, or if
     * a comms error occured, it returns an error code.
     * @return int16_t Returns 1 for 12H mode, 0 for 24H mode, or negative for error code.
     */
    int16_t getMode();
    /**
     * @brief Set the hour mode.
     * Set the 12H / 24H operating mode. Returns 0 (NO_ERROR) for set okay, otherwise if
     * a comms error occurs, an error code is returned. This will recalculate the hours
     * to retain the proper time.
     * @param is12h bool True = 12h mode, False = 24h mode.
     * @return int16_t Returns 0 for set okay, negative for error code.
     */
    int16_t setMode(const bool is12h);
    /**
     * @brief Get the Seconds.
     * Read and return the seconds. If a comms error occurs then a error code is returned.
     * @return int16_t Positive (00-59) is the seconds, negative is an error code.
     */
    int16_t getSeconds();
    /**
     * @brief Set the Seconds.
     * Set the seconds value, returns 0 (NO_ERROR) if set okay, otherwise if an invalid value
     * is passed, or a comms error occurs, an error code is returned.
     * @param value Value to set seconds to. Valid values: 0-59.
     * @return int16_t Returns 0 (NO_ERROR) if set okay, negative for error code.
     */
    int16_t setSeconds(const uint8_t value);
    /**
     * @brief Get the Minutes.
     * Read and return the minutes. If a comms error occurs, then an error code is returned.
     * @return int16_t Positive (00-59) is the minutes, negative is an error code.
     */
    int16_t getMinutes();
    /**
     * @brief Set the Minutes.
     * Set the minutes. Returns 0 (NO_ERROR) if set okay, otherwise if a comms error occurs, or
     * an invalid value is passed an error code is returned.
     * @param value Value to set minutes to. Valid values: 0-59.
     * @return int16_t Returns 0 (NO_ERROR) is set okay, negative for error code.
     */
    int16_t setMinutes(const uint8_t value);
    /**
     * @brief Get the hours in 12 hour format.
     * Return the hours in 12 hour format. If a communications error occurs, then an error
     * code is returned.
     * @return int16_t Positive (1-12) is the hour. Negative is error code.
     */
    int16_t getHour12H();
    /**
     * @brief Set the Hour, 12h format.
     * Set the hour in 12 hour format. returns 0 if set okay, otherwise if an invalid hour is passed,
     * or a comms error occurs, an error code is returned.
     * @param hour Hour to set. Valid values: 1-12.
     * @param isPM False = AM, True = PM.
     * @return int16_t Returns 0 (NO_ERROR) if set okay, negative is an error code.
     */
    int16_t setHour12H(const uint8_t hour, const bool isPM);
    /**
     * @brief Get the hours in 24 hour format.
     * Returns the hours in 24 hour format. If a comm error occurs, then an error code is returned.
     * @return int16_t Positive (0-23) is the hour. Negative is error code.
     */
    int16_t getHour24H();
    /**
     * @brief Set the hours in 24 hour format.
     * Set the hour in 24 hour format. Returns 0 (NO_ERROR) if set okay, other wise if an invalid
     * parameter is passed, or a comms error occurs, an error code is returned.
     * @param hour Value to set hour to.
     * @return int16_t Returns 0 (NO_ERROR) if set okay, or negative for error code.
     */
    int16_t setHour24H(const uint8_t hour);
    /**
     * @brief Set the full time. 12h format.
     * Sets the full time in 12h format. Returns 0 (NO_ERROR) if set okay, otherwise if
     * an invalid value is passed, or a comms error occurs, an error code is returned.
     * @param hour Hour to set. Valid values (1-12).
     * @param isPM False = AM, True = PM.
     * @param minute Minute to set. Valid values (0-59).
     * @param second Seconds to set. Valid values (0-59).
     * @return int16_t Returns 0 (NO_ERROR) if set okay, negative for error code.
     */
    int16_t setFullTime12h(const uint8_t hour, const bool isPM, const uint8_t minute, const uint8_t second);
    /**
     * @brief Set the time, 24 hour format.
     * Sets the full time. Returns 0 (NO_ERROR) if set okay, otherwise if an invalid value,
     * or a comms error occurs, it will return an error code.
     * @param hour Hour to set. Valid values (0-23).
     * @param minute Minute to set. Valid values (0-59).
     * @param second Second to set. Valid values (0-59).
     * @return int16_t Returns 0 (NO_ERROR) if set okay, negative for error code.
     */
    int16_t setFullTime24h(const uint8_t hour, const uint8_t minute, const uint8_t second);
    /**
     * @brief Get if it's AM/PM.
     * Gets am/pm and returns it, 0 for AM, 1 for PM, if a comms error occurs, then an error code is
     * returned.
     * @return int16_t 0=AM, 1=PM, negative for error code.
     */
    int16_t getAmPm();
    /**
     * @brief Set the AM/PM bit.
     * Sets am / pm bit if in 12h mode, only sets it if not set. If in 24h mode, will increase or
     * decrease the hour depending on the current time, and requested am/pm mode. Returns 0 if set
     * okay, otherwise if a comms error occurs, it will return an error code.
     * @param isPM False = AM, True = PM
     * @return int16_t Returns 0 (NO_ERROR) if set okay, negative for error code.
     */
    int16_t setAmPm(const bool isPM);
    /**
     * @brief Get the day of week.
     * Read and return the day of week. This is user programable. So when we set it we set Sunday 
     * equal to 1. Until it's set, we can't confirm what it is, since the datasheet doesn't list
     * a default value. If a comms error occurs during read, an error code is returned.
     * @return int16_t Positive(1-7) is the day, negative is error code.
     */
    int16_t getDay();
    /**
     * @brief Set the day of week.
     * Sets the day of week, since this is user programmable, we set 1 to equal sunday for the
     * purposes of this library. It isn't recommended to use this, as it's included for completeness.
     * Returns 0 (NO_ERROR) if set okay, otherwise if an invalid value is passed, or a comms error
     * occurs, an error code is returned.
     * @note It isn't recommended to use this method, use setFullDate() instead as it calculates the
     * appropriate day of the week and sets it.  An invalid value here will cause invalid operation.
     * @param value Value to set day of the week to. Valid values (1-7).
     * @return int16_t Returns 0 (NO_ERROR) if set okay, negative for error code.
     */
    int16_t setDay(const uint8_t value);
    /**
     * @brief Get the date (day of month)
     * Read and return the day of month. Positve values are the date, while if an error
     * occurs during comms, an error code is returned.
     * @return int16_t Positive (1-31) day of month, negative is error code.
     */
    int16_t getDate();
    /**
     * @brief Set the day of month (date).
     * Sets the day of month. Invalid values here will cause invalid operation. Returns 0 (NO_ERROR) for
     * set okay, or if an invalid value is passed, or a comms error occurs, an error code is returned.
     * @note It isn't recommended to use this, it's included for completeness, use setFullDate() instead,
     * as it calculates valid day of month values and checks for them, while this doesn't check the month.
     * @param value Value to set day of month to. Valid values (1-31).
     * @return int16_t Returns 0 (NO_ERROR) if set okay, negative for error code.
     */
    int16_t setDate(const uint8_t value);
    /**
     * @brief Get the month.
     * Read and return the month. Positive values are the month number, with 01 being January.
     * if an error occurs during comms, an error code is returned.
     * @return int16_t Positive (1-12) month, negative for error code.
     */
    int16_t getMonth();
    /**
     * @brief Set the Month.
     * Sets the month value. It's not recommended to use this, as it's included for completeness. An
     * invalid value here in respect to day of month, will cause invalid operation. Returns
     * 0 (NO_ERROR) if set okay, otherwise if an invalid value or a comms error occurs, an error code
     * is returned.
     * @note It isn't recommended to use this. Use setFullDate() instead, as this doesn't check the day
     * of month to verify valid values.
     * @param value Value to set month to. Valid values (1-12).
     * @return int16_t Returns 0 if set okay, negative for error code.
     */
    int16_t setMonth(const uint8_t value);
    /**
     * @brief Get the year.
     * Read and return the year. Positive values are the year, if an error occurs during
     * comms, an error code is returned.
     * @note Centuries are not tracked, so I'm assuimg 00=2000. Sorry for introducing a year 2099 bug.
     * @return int16_t Positve is the year, negative is an error code.
     */
    int16_t getYear();
    /**
     * @brief Set the Year.
     * Set the year value. It's not recommended to use this, as it's included for completeness.
     * An invalid value here in respect to month and date, will cause invalid operation. Returns 0
     * (NO_ERROR) if set okay, otherwise if an invalid value is passed or a comms error occurs,
     * an error code is returned.
     * @note It's not recommended to use this, use setFullDate instead as it calculates valid values,
     * and checks for them while this does not.
     * @param value Value to set year to. Valid values (2000-2099).
     * @return int16_t Returns 0 (NO_ERROR) if set okay, negative for error code.
     */
    int16_t setYear(const uint16_t value);
    /**
     * @brief Set the date.
     * Sets the full date, with error checking for invalid dates. This will calculate and
     * set the day of week as well. Returns 0 (NO_ERROR) if set okay, otherwise if an invalid
     * date is passed, or a comms error occurs, it will return an error code.
     * @param year Year to set. Valid values (2000-2099).
     * @param month Month to set. Valid values (1-12).
     * @param day Day of month to set. Valid values (1-[28-31]) depending on year and month.
     * @return int16_t Returns 0(NO_ERROR) if set okay, negative for error code.
     */
    int16_t setFullDate(const uint16_t year, const uint8_t month, const uint8_t day);
    /**
     * @brief Get the current date / time.
     * Reads and returns the current date / time as a pico datetime_t struct. If a comms error occurs
     * then all the values will be set to -1, except for the year, which will be set to the error code.
     * @note Centuries are not tracked, so I'm assuimg 00=2000. Sorry for introducing a year 2099 bug.
     * @return datetime_t 
     */
    datetime_t getDateTime();
    /**
     * @brief Set the date and time.
     * Sets the date and time from a datetime_t value. Returns 0 if set okay, otherwise if an invalid
     * value is passed, or a comms error occurs, an error code is returned.
     * @note This ignores the day of week portion of the datetime_t value, and instead calculates the
     * valid value to set.
     * @param value Value to set date/time to.
     * @return int16_t Retuns 0 (NO_ERROR) for set okay, negative for error code.
     */
    int16_t setDateTime(const datetime_t value);
    /**
     * @brief Get if the square wave enabled.
     * Gets if the square wave out put is enabled. Returns 0 if disabled, returns 1 if enabled,
     * otherwise if a comms error occurs, an error code will be returned.
     * @return int16_t Return 0 = disabled, 1 = enabled, negative is error code.
     */
    int16_t getSquareWaveEnabled();
    /**
     * @brief Set square wave enabled.
     * Enables / disables the square wave pin. Returns 0 (NO_ERROR) if set okay, otherwise if 
     * a comms error occurs, an error code will be returned.
     * @param value True = enabled, False = disabled.
     * @return Returns 0 (NO_ERROR) for set okay, negative for error code.
     */
    int16_t setSquareWaveEnabled(const bool value);
    /**
     * @brief Get the Square Wave idle state.
     * Returns the square wave pin state when square wave is disabled. Return value of 0 is idle low,
     * a value of 1 is idle high. If a comms error occurs an error code will be returned,
     * @return int16_t Return 0 = Idle low, 1 = Idle high, negative for error code.
     */
    int16_t getSquareWaveIdle();
    /**
     * @brief Set the Square Wave Idle state.
     * Sets the square wave pin state when square wave is disabled. A value of false means that the
     * pin will idle low, a value of true means the pin will idle high. Returns 0 (NO_ERROR) if set
     * okay, otherwise  if a comms error occurs, an error code will be returned.
     * @param value False = Idle LOW, True = Idle HIGH.
     * @return int16_t Returns 0 (NO_ERROR) if set okay, negative for error code.
     */
    int16_t setSquareWaveIdle(const bool value);
    /**
     * @brief Get the Square Wave frequency.
     * Returns the square wave frequency. Positive values, including zero is the rate (one of the
     * RATE_* values), otherwise if a comms error occurs an error code is returned.
     * @return int16_t Positive (including 0) is the rate, negative for an error code.
     */
    int16_t getSquareWaveRate();
    /**
     * @brief Set the Square Wave frequency.
     * Set the square wave frequency. Valid values are 0-3, rates are defined as RATE_*. Returns
     * 0 (NO_ERROR) if set okay, otherwise if an invalid value is passed, or a comms error occurs
     * an error code is returned.
     * @param value Frequency to set. Valid values (0-3)(IE: RATE_* values)
     * @return int16_t Returns 0 (NO_ERROR) if set okay, negative for error code.
     */
    int16_t setSquareWaveRate(const uint8_t value);
    /**
     * @brief Read sram.
     * Reads the sram, returns 0 (NO_ERROR) if read okay, otherwise if the read would
     * cross a boundary or if a comms error occurs, an error code is returned. If the
     * read would cross a boundary, no data is read.
     * @note Index's are zero based, so valid values are 0-55.
     * @param index Index of sram. Valid values (0-55).
     * @param buffer Buffer to read into.
     * @param length Number of bytes to read.
     * @return int16_t Returns 0 (NO_ERROR) if read okay, negative for error code.
     */
    int16_t readSram(const uint8_t index, uint8_t *buffer, const uint8_t length);
    /**
     * @brief Write to the sram.
     * Write to the sram. Returns 0 (NO_ERROR) if written successfully. If a boundary
     * would be crossed, an error code is returned, and no data is written, if a comms
     * error occurs, an error code will also be returned.
     * @note Index's are zero based, so valid values are 0-55.
     * @param index Index of sram. Valid values (0-55);
     * @param buffer Data to write to sram.
     * @param length Number of bytes to write.
     * @return int16_t Retuns 0 (NO_ERROR) for okay, negative for error code.
     */
    int16_t writeSram(const uint8_t index, uint8_t *buffer, const uint8_t length);
    /**
     * @brief Initialize the chip.
     * Initialize communications with the chip. Returns 0(NO_ERROR) for okay, otherwise if
     * an invalid pin is passed during construction, or a comms error occurs, an error code
     * is returned.
     * @param initI2c True = init the i2c bus, False = Don't init i2c bus, assumes already init.
     * @return int16_t Returns 0 (NO_ERROR) if initialized okay, negative for error code.
     */
    int16_t initialize(const bool initI2C=true);

    private:
/* ############# Private Constants: ############# */
    // Date Constants:
        const uint8_t DAYS_PER_MONTH[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        const uint8_t MONTH_CODES[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
        const uint8_t CENTURY_CODES[7] = {4, 2, 0, 6, 4, 2, 0};
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
        const uint8_t MASK_SECONDS_BCD      = 0x7F;
        const uint8_t MASK_MINUTES_BCD      = 0x7F;
        const uint8_t MASK_IS_12H           = 0x40;
        const uint8_t MASK_IS_PM            = 0x20;
        const uint8_t MASK_12H_HOURS_BCD    = 0x1F;
        const uint8_t MASK_24H_HOURS_BCD    = 0x3F;
        const uint8_t MASK_DAY_BCD          = 0x03;
        const uint8_t MASK_DATE_BCD         = 0x3F;
        const uint8_t MASK_MONTHS_BCD       = 0x1F;
        const uint8_t MASK_YEARS_BCD        = 0xFF;
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
    int16_t     __readRegisters__(const uint8_t reg, uint8_t *buffer, size_t length);
    int16_t     __writeRegisters__(const uint8_t reg, uint8_t *buffer, size_t length);
    uint8_t     __bcd_to_dec__(const uint8_t value);
    uint8_t     __dec_to_bcd__(const uint8_t value);
    bool inline __is12h__(const uint8_t rawHours);
    bool inline __isPM__(const uint8_t rawHour);
    uint8_t     __hours12h__(const uint8_t rawHours);
    uint8_t     __hours24h__(const uint8_t rawHours);
    void        __12h_to_24h__(const uint8_t hours12, const bool isPM, uint8_t *hours24);
    void        __24h_to_12h__(const uint8_t hours24, uint8_t *hours12, bool *isPM);
    uint8_t     __updateSeconds__(const uint8_t origSeconds, const uint8_t newSeconds);
    uint8_t     __updateHours__(const uint8_t origHours, const uint8_t newHours);
};

/* ############### Public Functions: ################# */
int16_t myDS1307::getDayOfWeek(const uint16_t year, const uint8_t month, const uint8_t day) {
    int16_t returnValue;
// Validate Date for the range this works for.
    if (year < 1700 or year > 2399) { return ERROR_INVALID_YEAR; }
    if (month < 1 or month > 12) { return ERROR_INVALID_MONTH; }
    uint8_t maxDate = DAYS_PER_MONTH[month-1];
    if (isLeapYear(year) == true) { maxDate += 1; }
    if (day < 1 or day > maxDate) { return ERROR_INVALID_DATE; }
// Calculate century code:
    uint8_t centuryCode = CENTURY_CODES[((year / 100) - 17)];
// Calculate year code:
    uint8_t yearCode = year - ((year / 100) * 100);
    yearCode = (yearCode + (yearCode/4)) % 7;
// Get month code:
    uint8_t monthCode = MONTH_CODES[month-1];
// Leap year Code:
    uint8_t leapYearCode = isLeapYear(year);
// Calculate day of week.
    return (int16_t)((yearCode + monthCode + centuryCode + day - leapYearCode) % 7) + 1;
}

bool inline myDS1307::isLeapYear(const uint16_t year) {
    if ( (year % 4) == 0) {
        if ((year % 100) == 0) {
            if ((year % 400) == 0) {
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

int16_t myDS1307::isValidDate(const uint16_t year, const uint8_t month, const uint8_t day) {
    if (year < 2000 or year > 2099) { return ERROR_INVALID_YEAR; }
    if (month < 1 or month > 12) { return ERROR_INVALID_MONTH; }
    uint8_t maxDate = DAYS_PER_MONTH[month-1];
    if (isLeapYear(year) == true) { maxDate += 1; }
    if (day < 1 or day > maxDate) { return ERROR_INVALID_DAY; }
    return NO_ERROR;
}

int16_t myDS1307::isValidDateTime(const datetime_t value) {
    int16_t returnValue;
    if (value.year < 2000 or value.year > 2099) { return ERROR_INVALID_YEAR; }
    if (value.month < 1 or value.month > 12) { return ERROR_INVALID_MONTH; }
    uint8_t maxDate = DAYS_PER_MONTH[value.month-1];
    if (isLeapYear(value.year) == true and value.month == 2) { maxDate += 1; }
    if (value.day < 1 or value.day > maxDate) { return ERROR_INVALID_DATE; }
    if (value.hour < 0 or value.hour > 23) { return ERROR_INVALID_HOURS; }
    if (value.min < 0 or value.min > 59) { return ERROR_INVALID_MINUTES; }
    if (value.sec < 0 or value.sec > 59) { return ERROR_INVALID_SECONDS; }
    return NO_ERROR;
}

int16_t myDS1307::getEnabled() {
    int16_t returnValue;
    uint8_t rawSeconds;
    bool enabled;
    returnValue = __readRegisters__(REG_SECONDS, &rawSeconds, 1);
    if (returnValue < 0) { return returnValue; }
    enabled = !((bool)(rawSeconds & MASK_CLOCK_HALT));
    return (int16_t)enabled;
}

int16_t myDS1307::setEnabled(const bool value) {
    int16_t returnValue;
    uint8_t rawSeconds;
    returnValue = __readRegisters__(REG_SECONDS, &rawSeconds, 1);
    if (returnValue < 0) { return returnValue; }
    rawSeconds &= ~MASK_CLOCK_HALT;
    if (value == false) { rawSeconds |= MASK_CLOCK_HALT; }
    returnValue = __writeRegisters__(REG_SECONDS, &rawSeconds, 1);
    return returnValue;
}

int16_t myDS1307::getMode() {
    int16_t returnValue;
    uint8_t rawHours;
    uint8_t reg = REG_HOURS;
    returnValue = __readRegisters__(reg, &rawHours, 1);
    if (returnValue < 0) { return returnValue; }
    _is12h = (bool)(rawHours & MASK_IS_12H);
    return (int16_t)_is12h;
}

int16_t myDS1307::setMode(const bool is12h) {
    int16_t returnValue;
    uint8_t rawHours;
    uint8_t newHours;
    returnValue = __readRegisters__(REG_HOURS, &rawHours, 1);
    if (returnValue < 0) { return returnValue; }
    if (__is12h__(rawHours) == is12h) { return NO_ERROR; } // NO change return okay.
// Switch from 12h mode to 24h mode.
    if (__is12h__(rawHours) == true and is12h == false) {
    // Convert and set the hours:
        uint8_t hours12 = __bcd_to_dec__(rawHours & MASK_12H_HOURS_BCD);
        bool isPM = __isPM__(rawHours);
        uint8_t hours24;
        __12h_to_24h__(hours12, isPM, &hours24);
        newHours = __dec_to_bcd__(hours24);
        rawHours = __updateHours__(rawHours, newHours);
    // Clear the is12h flag.
        rawHours &= ~MASK_IS_12H;
// Switch from 24h mode to 12h mode.
    } else if (__is12h__(rawHours) == false and is12h == true) {
    // Convert and set the hours:    
        uint8_t hours24 = __bcd_to_dec__(rawHours & MASK_24H_HOURS_BCD);
        uint8_t hours12;
        bool isPM;
        __24h_to_12h__(hours24, &hours12, &isPM);
        newHours = __dec_to_bcd__(hours12);
        if (isPM == true) { newHours |= MASK_IS_PM; }
        rawHours = __updateHours__(rawHours, newHours);
    // Set the is12h bit.
        rawHours |= MASK_IS_12H;
    }
    returnValue == __writeRegisters__(REG_HOURS, &rawHours, 1);
    return returnValue;
}

int16_t myDS1307::getSeconds() {
    int16_t returnValue;
    uint8_t rawSeconds;
    returnValue = __readRegisters__(REG_SECONDS, &rawSeconds, 1);
    if (returnValue < 0) { return returnValue; }
    return (int16_t) __bcd_to_dec__(rawSeconds & MASK_SECONDS_BCD);
}

int16_t myDS1307::setSeconds(const uint8_t value) {
    int16_t returnValue;
    uint8_t rawSeconds;
    if (value > 59) { return ERROR_INVALID_SECONDS; }
    returnValue = __readRegisters__(REG_SECONDS, &rawSeconds, 1);
    if (returnValue < 0) { return returnValue; }
    rawSeconds = __updateSeconds__(rawSeconds, __dec_to_bcd__(value));
    // rawSeconds &= ~MASK_SECONDS_BCD;
    // rawSeconds |= (__dec_to_bcd__(value) & MASK_SECONDS_BCD);
    returnValue = __writeRegisters__(REG_SECONDS, &rawSeconds, 1);
    return returnValue;
}

int16_t myDS1307::getMinutes() {
    int16_t returnValue;
    uint8_t rawMinutes;
    returnValue = __readRegisters__(REG_MINUTES, &rawMinutes, 1);
    if (returnValue < 0) { return returnValue; }
    return (int16_t) __bcd_to_dec__(rawMinutes & MASK_MINUTES_BCD);
}

int16_t myDS1307::setMinutes(const uint8_t value) {
    int16_t returnValue;
    uint8_t rawMinutes;
    if (value > 59) { return ERROR_INVALID_MINUTES; }
    rawMinutes = __dec_to_bcd__(value);
    returnValue = __writeRegisters__(REG_MINUTES, &rawMinutes, 1);
    return returnValue;
}

int16_t myDS1307::getHour12H() {
    int16_t returnValue;
    uint8_t rawHours;
    returnValue = __readRegisters__(REG_HOURS, &rawHours, 1);
    if (returnValue < 0) { return returnValue; }
    return (int16_t) __hours12h__(rawHours);
}

int16_t myDS1307::setHour12H(const uint8_t hour, const bool isPM) {
    uint16_t returnValue;
    uint8_t rawHours;
    uint8_t newHours;
    if (hour < 1 or hour > 12) { return ERROR_INVALID_HOURS; }
    returnValue = __readRegisters__(REG_HOURS, &rawHours, 1);
    if (returnValue < 0) { return returnValue; }
    if (__is12h__(rawHours) == true) {
        if (isPM == true) { newHours |= MASK_IS_PM; }
        newHours |= __dec_to_bcd__(hour);
        rawHours = __updateHours__(rawHours, newHours);
    } else {
        uint8_t hours24;
        __12h_to_24h__(hour, isPM, &hours24);
        newHours |= __dec_to_bcd__(hours24);
        rawHours = __updateHours__(rawHours, newHours);
    }
    returnValue = __writeRegisters__(REG_HOURS, &rawHours, 1);
    return returnValue;
}

int16_t myDS1307::getHour24H() {
    int16_t returnValue;
    uint8_t rawHours;
    returnValue = __readRegisters__(REG_HOURS, &rawHours, 1);
    if (returnValue < 0) { return returnValue; }
    return (int16_t) __hours24h__(rawHours);
}

int16_t myDS1307::setHour24H(const uint8_t hour) {
    int16_t returnValue;
    uint8_t rawHours;
    uint8_t newHours;
    returnValue = __readRegisters__(REG_HOURS, &rawHours, 1);
    if (returnValue < 0) { return returnValue; }
    if (__is12h__(rawHours) == true) {
        uint8_t hours12;
        bool isPM;
        __24h_to_12h__(hour, &hours12, &isPM);
        if (isPM == true) { newHours |= MASK_IS_PM;}
        newHours |= __dec_to_bcd__(hours12);
        rawHours = __updateHours__(rawHours, newHours);
    } else {
        rawHours = __updateHours__(rawHours, __dec_to_bcd__(hour));
    }
    returnValue = __writeRegisters__(REG_HOURS, &rawHours, 1);
    return returnValue;
}

int16_t myDS1307::setFullTime12h(const uint8_t hour, const bool isPM, const uint8_t minute,
                                        const uint8_t second) {
    int16_t returnValue;
    uint8_t dataBuffer[3];
    uint8_t newHours;
    if (hour < 1 or hour > 12) { return ERROR_INVALID_HOURS; }
    if (minute > 59) { return ERROR_INVALID_MINUTES; }
    if (second > 59) { return ERROR_INVALID_SECONDS; }
    returnValue = __readRegisters__(REG_SECONDS, dataBuffer, 3);
    dataBuffer[0] = __updateSeconds__(dataBuffer[0], __dec_to_bcd__(second));
    dataBuffer[1] = __dec_to_bcd__(minute);
    if (__is12h__(dataBuffer[2]) == true) {
        if (isPM == true) { newHours |= MASK_IS_PM; }
        newHours |= __dec_to_bcd__(hour);
    } else {
        uint8_t hours24;
        __12h_to_24h__(hour, isPM, &hours24);
        newHours = __dec_to_bcd__(hours24);
    }
    dataBuffer[2] = __updateHours__(dataBuffer[2], newHours);
    returnValue = __writeRegisters__(REG_SECONDS, dataBuffer, 3);
    return returnValue;
}

int16_t myDS1307::setFullTime24h(const uint8_t hour, const uint8_t minute, const uint8_t second) {
    int16_t returnValue;
    uint8_t dataBuffer[3];
    uint8_t newHours;
    if (hour > 23) { return ERROR_INVALID_HOURS; }
    if (minute > 59) { return ERROR_INVALID_MINUTES; }
    if (second > 59) { return ERROR_INVALID_SECONDS; }
    returnValue = __readRegisters__(REG_SECONDS, dataBuffer, 3);
    dataBuffer[0] = __updateSeconds__(dataBuffer[0], __dec_to_bcd__(second));
    dataBuffer[1] = __dec_to_bcd__(minute);
    if (__is12h__(dataBuffer[2]) == true) {
        uint8_t hours12;
        bool isPM;
        __24h_to_12h__(hour, &hours12, &isPM);
        if (isPM == true) { newHours |= MASK_IS_PM; }
        newHours |= __dec_to_bcd__(hours12);
    } else {
        newHours = __dec_to_bcd__(hour);
    }
    dataBuffer[2] = __updateHours__(dataBuffer[2], newHours);
    returnValue = __writeRegisters__(REG_SECONDS, dataBuffer, 3);
    return returnValue;
}

int16_t myDS1307::getAmPm() {
    int16_t returnValue;
    uint8_t rawHours;
    uint8_t hours;
    returnValue = __readRegisters__(REG_HOURS, &rawHours, 1);
    if (returnValue < 0) { return returnValue; }
    return (int16_t)__isPM__(rawHours);
}

int16_t myDS1307::setAmPm(const bool isPM) {
    int16_t returnValue;
    uint8_t rawHours;
    returnValue = __readRegisters__(REG_HOURS, &rawHours, 1);
    if (__is12h__(rawHours) == true) {
        rawHours &= ~MASK_IS_PM;
        if (isPM) { rawHours |= MASK_IS_PM; }
    } else {
        uint8_t hours = __bcd_to_dec__(rawHours & MASK_24H_HOURS_BCD);
        if (isPM == true and hours < 13) {
            hours += 12;
        } else if (isPM == false and hours > 12) {
            hours -= 12;
        }
        rawHours = __updateHours__(rawHours, __dec_to_bcd__(hours));
    }
    returnValue = __writeRegisters__(REG_HOURS, &rawHours, 1);
    return returnValue;
}

int16_t myDS1307::getDay() {
    int16_t returnValue;
    uint8_t day;
    returnValue = __readRegisters__(REG_DAY, &day, 1);
    if (returnValue < 0) { return returnValue; }
    return (int16_t)day;
}

int16_t myDS1307::setDay(const uint8_t value){
    int16_t returnValue;
    uint8_t val = value;
    if (value < 1 or value > 7) { return ERROR_INVALID_DAY; }
    returnValue = __writeRegisters__(REG_DAY, &val, 1);
    return returnValue;
}

int16_t myDS1307::getDate() {
    int16_t returnValue;
    uint8_t rawDate;
    returnValue = __readRegisters__(REG_DATE, &rawDate, 1);
    if (returnValue < 0) { return returnValue; }
    return (int16_t)__bcd_to_dec__(rawDate & MASK_DATE_BCD);
}

int16_t myDS1307::setDate(const uint8_t value) {
    int16_t returnValue;
    uint8_t newDate;
    if (value < 1 or value > 31) { return ERROR_INVALID_DATE; }
    newDate = __dec_to_bcd__(value);
    returnValue = __writeRegisters__(REG_DATE, &newDate, 1);
    return returnValue;
}

int16_t myDS1307::getMonth() {
    int16_t returnValue;
    uint8_t rawMonth;
    returnValue = __readRegisters__(REG_MONTH, &rawMonth, 1);
    if (returnValue < 0) { return returnValue; }
    return (int16_t)__bcd_to_dec__(rawMonth & MASK_MONTHS_BCD);
}

int16_t myDS1307::setMonth(const uint8_t value){
    int16_t returnValue;
    uint8_t newMonth;
    if (value < 1 or value > 12) { return ERROR_INVALID_MONTH; }
    newMonth = __dec_to_bcd__(value);
    returnValue = __writeRegisters__(REG_MONTH, &newMonth, 1);
    return returnValue;
}

int16_t myDS1307::getYear() {
    int16_t returnValue;
    uint8_t rawYear;
    int16_t year;
    returnValue = __readRegisters__(REG_YEAR, &rawYear, 1);
    if (returnValue < 0) { return returnValue; }
    year = __bcd_to_dec__(year & MASK_YEARS_BCD);
    year += 2000; // Here is the bug.
    return year;
}

int16_t myDS1307::setYear(const uint16_t value) {
    int16_t returnValue;
    uint8_t newYear;
    if (value < 2000 or value > 2099) { return ERROR_INVALID_YEAR; }
    newYear = __dec_to_bcd__((value-2000));
    returnValue = __writeRegisters__(REG_YEAR, &newYear, 1);
    return returnValue;
}

int16_t myDS1307::setFullDate(const uint16_t year, const uint8_t month, const uint8_t day) {
    int16_t returnValue;
    uint8_t dataBuffer[4];
    uint8_t dow;
    returnValue = isValidDate(year, month, day);
    if (returnValue < 0) { return returnValue; }
    dow = getDayOfWeek(year, month, day);
    dataBuffer[0] = dow;
    dataBuffer[1] = __dec_to_bcd__(day);
    dataBuffer[2] = __dec_to_bcd__(month);
    dataBuffer[3] = __dec_to_bcd__(year-2000);
    returnValue = __writeRegisters__(REG_DAY, dataBuffer, 4);
    return returnValue;
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
    dateTime.sec = __bcd_to_dec__(dataBuffer[0] & MASK_SECONDS_BCD);
    dateTime.min = __bcd_to_dec__(dataBuffer[1] & MASK_MINUTES_BCD);
    dateTime.hour = __hours24h__(dataBuffer[2]);

    dateTime.dotw = (dataBuffer[3] - 1); // 0 is sunday in datetime_t.
    dateTime.day = __bcd_to_dec__(dataBuffer[4] & MASK_DATE_BCD);
    dateTime.month = __bcd_to_dec__(dataBuffer[5] & MASK_MONTHS_BCD);
    dateTime.year = __bcd_to_dec__(dataBuffer[6] & MASK_YEARS_BCD);
    dateTime.year += 2000; // Here's the bug again.
    return dateTime;
}

int16_t myDS1307::setDateTime(const datetime_t value) {
    int16_t returnValue;
    uint8_t dataBuffer[7];
    returnValue = isValidDateTime(value);
    if (returnValue < 0 ) { return returnValue; }
    returnValue = __readRegisters__(REG_SECONDS, dataBuffer, 7);
    if (returnValue < 0) { return returnValue; }
    dataBuffer[0] = __updateSeconds__(dataBuffer[0], __dec_to_bcd__(value.sec));
    dataBuffer[1] = __dec_to_bcd__(value.min);
    if (__is12h__(dataBuffer[2]) == true) {
        uint8_t newHours;
        uint8_t hours12;
        bool isPM;
        __24h_to_12h__(value.hour, &hours12, &isPM);
        if (isPM == true) { newHours |= MASK_IS_PM; }
        newHours |= __dec_to_bcd__(hours12);
        dataBuffer[2] = __updateHours__(dataBuffer[2], newHours);
    } else {
        dataBuffer[2] = __updateHours__(dataBuffer[2], __dec_to_bcd__(value.hour));
    }
    dataBuffer[3] = getDayOfWeek(value.year, value.month, value.day);
    dataBuffer[4] = __dec_to_bcd__(value.day);
    dataBuffer[5] = __dec_to_bcd__(value.month);
    dataBuffer[6] = __dec_to_bcd__((value.year-2000));
    returnValue = __writeRegisters__(REG_SECONDS, dataBuffer, 7);
    return returnValue;
}

int16_t myDS1307::getSquareWaveEnabled() {
    int16_t returnValue;
    uint8_t controlData;
    returnValue = __readRegisters__(REG_CONTROL, &controlData, 1);
    if (returnValue < 0) { return returnValue; }
    return (int16_t)((bool)(controlData & MASK_CONTROL_SQWE));
}

int16_t myDS1307::setSquareWaveEnabled(const bool value) {
    int16_t returnValue;
    uint8_t controlData;
    returnValue = __readRegisters__(REG_CONTROL, &controlData, 1);
    if (returnValue < 0) { return returnValue; }
    controlData &= ~MASK_CONTROL_SQWE;
    if (value) { controlData |= MASK_CONTROL_SQWE; }
    returnValue = __writeRegisters__(REG_CONTROL, &controlData, 1);
    return returnValue;
}

int16_t myDS1307::getSquareWaveIdle() {
    int16_t returnValue;
    uint8_t controlData;
    returnValue = __readRegisters__(REG_CONTROL, &controlData, 1);
    if (returnValue < 0) { return returnValue; }
    return (int16_t) ((bool)(controlData & MASK_CONTROL_OUT));
}

int16_t myDS1307::setSquareWaveIdle(const bool value) {
    int16_t returnValue;
    uint8_t controlData;
    returnValue = __readRegisters__(REG_CONTROL, &controlData, 1);
    if (returnValue < 0) { return returnValue; }
    controlData &= ~ MASK_CONTROL_OUT;
    if (value == true) { controlData |= MASK_CONTROL_OUT; }
    returnValue = __writeRegisters__(REG_CONTROL, &controlData, 1);
    return returnValue;
}

int16_t myDS1307::getSquareWaveRate() {
    int16_t returnValue;
    uint8_t controlData;
    returnValue = __readRegisters__(REG_CONTROL, &controlData, 1);
    if (returnValue < 0) { return returnValue; }
    return (int16_t)(controlData & MASK_CONTROL_RS);
}

int16_t myDS1307::setSquareWaveRate(const uint8_t value) {
    int16_t returnValue;
    uint8_t controlData;
    if (value > RATE_32KHZ) { return ERROR_INVALID_RATE; }
    returnValue = __readRegisters__(REG_CONTROL, &controlData, 1);
    if (returnValue < 0) { return returnValue; }
    controlData &= ~MASK_CONTROL_RS;
    controlData |= value;
    returnValue = __writeRegisters__(REG_CONTROL, &controlData, 1);
}

int16_t myDS1307::readSram(const uint8_t index, uint8_t *buffer, const uint8_t length) {
    int16_t returnValue;
    uint8_t reg;
    if ((index + (length - 1)) > 55) { return ERROR_BOUNDARY_CROSSED; }
    reg = REG_SRAM_START + index;
    returnValue = __readRegisters__(reg, buffer, length);
    return returnValue;
}

int16_t myDS1307::writeSram(const uint8_t index, uint8_t *buffer, const uint8_t length) {
    int16_t returnValue;
    uint8_t reg;
    if ((index + (length - 1)) > 55) { return ERROR_BOUNDARY_CROSSED; }
    reg = REG_SRAM_START + index;
    returnValue = __writeRegisters__(reg, buffer, length);
    return returnValue;
}

int16_t myDS1307::initialize(const bool initI2C) {
    int32_t returnValue;
    // Initialize i2c at 50 KHz, since we're running at 3.3v which is underspec, we give it more
    // time to respond. Otherwise occasionally the chip will not respond, and we throw an error.
    if (initI2C == true) {
        returnValue = myI2C::initializeMaster(_i2cPort, _sdaPin, _sclPin, 50*1000);
        if (returnValue < 0) { return (int16_t)returnValue; }
    }
    // Verify comms by getting the day byte and checking it's value.
    // since this value should never be zero.
    uint8_t day;
    returnValue = __readRegisters__(REG_DAY, &day, 1);
    if (returnValue < 0) { return (int16_t)returnValue; }
    if (day < 1 or day > 7) { return ERROR_COMMS_FAILED; }
    return NO_ERROR;
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

int16_t myDS1307::__writeRegisters__(const uint8_t reg, uint8_t *buffer, size_t length) {
    int16_t returnValue;
// Create buffer to send:
    uint8_t dataBuffer[length+1];
    dataBuffer[0] = reg;
    for (uint8_t i=0; i<length; i++) {
        dataBuffer[i+1] = buffer[i];
    }
// Send buffer:
    returnValue = i2c_write_timeout_us(_i2cPort, I2C_ADDRESS, dataBuffer, length+1, false, I2C_TIMEOUT_US);
    if (returnValue < 0) {
        if (returnValue == PICO_ERROR_GENERIC) { return ERROR_NOT_RESPONDING; }
        if (returnValue == PICO_ERROR_TIMEOUT) { return ERROR_TIMEOUT; }
    }
    return NO_ERROR;
}

uint8_t myDS1307::__bcd_to_dec__(const uint8_t value) {
    return ((((value & 0xF0) >> 4) * 10) + (value & 0x0F));
}

uint8_t myDS1307::__dec_to_bcd__(const uint8_t value) {
    return (((value / 10) << 4) | (value - ((value / 10) * 10)));
}

bool inline myDS1307::__is12h__(const uint8_t rawHours) {
    return bool(rawHours & MASK_IS_12H);
}

bool inline myDS1307::__isPM__(const uint8_t rawHours) {
    if (__is12h__(rawHours) == true) {
        return bool(rawHours & MASK_IS_PM);
    }
    if (__bcd_to_dec__(rawHours & MASK_24H_HOURS_BCD) > 11) {
        return true;
    } else {
        return false;
    }
}

uint8_t myDS1307::__hours12h__(const uint8_t rawHours) {
    // uint8_t hours;
    if (__is12h__(rawHours) == true) {
        return __bcd_to_dec__(rawHours & MASK_12H_HOURS_BCD);
    } else {
        uint8_t hours24 = __bcd_to_dec__(rawHours & MASK_24H_HOURS_BCD);
        uint8_t hours12;
        bool isPM;
        __24h_to_12h__(hours24, &hours12, &isPM);
        return hours12;
    }
}

uint8_t myDS1307::__hours24h__(const uint8_t rawHours) {
    // uint8_t hours;
    if (__is12h__(rawHours) == true) {
        uint8_t hours12 = __bcd_to_dec__(rawHours & MASK_12H_HOURS_BCD);
        bool isPM =__isPM__(rawHours);
        uint8_t hours24;
        __12h_to_24h__(hours12, isPM, &hours24);
        return hours24;
    } else {
        return __bcd_to_dec__(rawHours & MASK_24H_HOURS_BCD);
    }
}

void myDS1307::__12h_to_24h__(const uint8_t hours12, const bool isPM, uint8_t *hours24) {
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
}

void myDS1307::__24h_to_12h__(const uint8_t hours24, uint8_t *hours12, bool *isPM) {
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
}

uint8_t myDS1307::__updateSeconds__(const uint8_t origSeconds, const uint8_t newSeconds) {
    uint8_t returnValue = origSeconds & ~MASK_SECONDS_BCD;
    returnValue |= newSeconds & MASK_SECONDS_BCD;
    return returnValue;
}

uint8_t myDS1307::__updateHours__(const uint8_t origHours, const uint8_t newHours){
    uint8_t returnValue = origHours & ~MASK_24H_HOURS_BCD;
    returnValue |= newHours & MASK_24H_HOURS_BCD;
    return returnValue;
}
#endif