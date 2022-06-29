/**
 * @file myPWM.hpp
 * @author Peter Nearing (pnearing@protonmail.com)
 * @brief Namespace for PWM helper functions.
 * @version 0.1
 * @date 2022-06-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MY_PWM_H
#define MY_PWM_H

#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include "../myStandardDefines.hpp"
#include "../myErrorCodes.hpp"
#include "../myHelpers.hpp"


/**
 * @brief PWM helper functions.
 * Namespace to hold PWM helper functions and track them. Some code has been taken from: 
 * https://www.i-programmer.info/programming/hardware/14849-the-pico-in-c-basic-pwm.html
 */
namespace myPWM {
/*
 * ####################### Constants: ###############################
 */
    /**
     * @brief No error. Value 0.
     * No error occured.
     */
    static const int16_t NO_ERROR = MY_NO_ERROR;
    /**
     * @brief Invalid pin. Value -2.
     * An invalid pin was passed
     */
    static const int16_t ERROR_INVALID_PIN = MY_INVALID_PIN;
    /**
     * @brief Pin not initialized. Value -100.
     * The pin is not initialized as a PWM pin.
     */
    static const int16_t ERROR_PIN_NOT_INIT = MY_PWM_PIN_NOT_INIT;
    /**
     * @brief Pin already initialized. Value -101.
     * The pin is already initialized as a PWM pin.
     */
    static const int16_t ERROR_PIN_ALEREADY_INIT = MY_PWM_PIN_ALREADY_INIT;
    /**
     * @brief Invalid slice number. Value -102.
     * An invalid slice number was passed.
     */
    static const int16_t ERROR_INVALID_SLICE = MY_PWM_INVALID_SLICE;
    /**
     * @brief Slice already enabled. Value -103.
     * The slice has already been enabled.
     */
    static const int16_t ERROR_ALREADY_ENABLED = MY_PWM_SLICE_ALREADY_ENABLED;
    /**
     * @brief Slice already disabled. Value -104.
     * The slice has already been disabled.
     */
    static const int16_t ERROR_ALREADY_DISABLED = MY_PWM_SLICE_ALREADY_DISABLED;
    /**
     * @brief Phase correct enabled. Value -105.
     * Phase correct has already been enabled for this slice.
     */
    static const int16_t ERROR_PHASE_CORRECT_ENABLED = MY_PWM_PHASE_CORRECT_ENABLED;
    /**
     * @brief Phase correct disabled. Value -106.
     * Phase correct is already disabled on this slice.
     */
    static const int16_t ERROR_PHASE_CORRECT_DISABLED = MY_PWM_PHASE_CORRECT_DISABLED;
    /**
     * @brief Invalid frequency. Value -107.
     * An invalid frequency was passed.
     */
    static const int16_t ERROR_INVALID_FREQUENCY = MY_PWM_INVALID_FREQ;
    /**
     * @brief Invalid channel. Value -108.
     * An invalid channel was passed.
     */
    static const int16_t ERROR_INVALID_CHANNEL = MY_PWM_INVALID_CHANNEL;
    /**
     * @brief Invalid duty. Value -109.
     * An invalid duty was passed. Valid values are 0-100 inclusive.
     */
    static const int16_t ERROR_INVALID_DUTY = MY_PWM_INVALID_DUTY;
    /**
     * @brief Invalid divisor integer. Value -110.
     * The integer portion of the supplied divisor is invalid. Must be greater than 0.
     */
    static const int16_t ERROR_INVALID_DIVISOR_INT = MY_PWM_INVALID_DIVISOR_INT;
    /**
     * @brief Invalid divisor fraction. Value -111.
     * The fraction portion of the supplied divisor is invalid. Must be 0-15 inclusive.
     */
    static const int16_t ERROR_INVALID_DIVISOR_FRAC = MY_PWM_INVALID_DIVISOR_FRAC;
/*
 * ####################### Variables: ###############################
 */
    /** @private */
    uint32_t    _pinStatus = 0x00000000; // Store if the pins are initialized.
    /** @private */
    uint8_t     _sliceStatus = 0x00; // Store if the slices are enabled.
    /** @private */
    uint8_t     _phaseStatus = 0x00; // Store if slice is in phase correct mode.
    /** @private */
    uint16_t    _wrapValues[MY_PWM_NUM_SLICE] = { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}; // Store the wrap values of the slices.
    /** @private */
    uint16_t    _levelValues[MY_PWM_NUM_SLICE][MY_PWM_NUM_CHANNEL] = { 0x0000 }; // Store the level values.
    /** @private */
    uint8_t     _clkDivInt[MY_PWM_NUM_SLICE] = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }; // Clock divisor integer portion
    /** @private */
    uint8_t     _clkDivFrac[MY_PWM_NUM_SLICE] = { 0x00 }; // Clock divisor fraction portion
/*
 * ############################## Functions: #########################
 */
/* Validation functions: */
    /**
     * @brief Verify slice number.
     * Verifies if a number is a valid slice number.
     * @param slice Slice number to check.
     * @return true Is a valid slice.
     * @return false Is not a valid slice.
     */
    bool isSlice(const uint8_t slice) {
        if (slice >= MY_PWM_NUM_SLICE) { return false; }
        return true;
    }
    /**
     * @brief Verify channel number.
     * Verifies if a given number is a valid channel number.
     * @param channel Channel number to verify.
     * @return true Channel is valid.
     * @return false Channel is not valid.
     */
    bool isChannel(const uint8_t channel) {
        if (channel >= MY_PWM_NUM_CHANNEL) { return false; }
        return true;
    }
/* Status  / wrap / level value functions: */
    /**
     * @brief Get if the pin is initialized.
     * Get if the pin is initialized, Returns 0 or 1 for initialized state, or if an invalid
     * pin number was passed, it will return an error code.
     * @param pin Pin to check.
     * @return int16_t Returns 0 for not initialized, 1 for initialized, negative for error code.
     */
    int16_t getPinInit(const uint8_t pin) {
        if (myHelpers::isPin(pin) == false) { return MY_INVALID_PIN; }
        return (bool)(_pinStatus & (1<<pin));
    }
    /** @private */
    void __setPinInit__(const uint8_t pin, const bool value) {
        if (value == true) {
            _pinStatus |= (1 << pin);
        } else {
            _pinStatus &= ~(1 << pin);
        }
    }
    /**
     * @brief Get if the slice is enabled.
     * Get if slice is enabled. Returns true if enabled, false if not enabled, or invalid slice passed.
     * @param slice Slice to check.
     * @return true If the slice is enabled.
     * @return false If the slice is disabled, or invalid.
     */
    bool getEnabled(const uint8_t slice) {
        if (isSlice(slice) == false) { return false; }
        return (bool)(_sliceStatus & (1 << slice));
    }
    /**
     * @brief Get the if slice controlling given pin is enabled.
     * Return true if the slice controlling the given pin is enabled, returns false if disabled,
     * or if an invalid pin was passed.
     * @param pin Pin to check.
     * @return true Slice is enabled.
     * @return false Slice is disabled.
     */
    bool getPinEnabled(const uint8_t pin) {
        if (myHelpers::isPin(pin) == false) { return false; }
        uint8_t slice = pwm_gpio_to_slice_num(pin);
        return (bool)(_sliceStatus & (1 << slice));
    }
    /** @private */
    void __setEnabled__(const uint8_t slice, const bool value) {
        pwm_set_enabled(slice, value);
        if (value == true) {
            _sliceStatus |= (1 << slice);
        } else {
            _sliceStatus &= ~(1 << slice);
        }
    }
    /**
     * @brief Get slice phase correct mode.
     * Returns true if the slice is in phase correct mode, and false if not, or slice is
     * invalid.
     * @param slice Slice to check.
     * @return true Slice is in phase correct mode.
     * @return false Slice is not in phase correct mode.
     */
    bool getPhaseCorrect(const uint8_t slice) {
        if (isSlice(slice) == false) { return false; }
        return (bool)(_phaseStatus & (1 << slice));
    }
    /**
     * @brief Get the phase correct given a pin.
     * Returns true if the slice controlling a given pin is in phase correct mode, returns
     * false if not, or pin is invalid.
     * @param pin Pin to get the phase correct of.
     * @return true Phase correct is enabled.
     * @return false Phase correct is disabled, or pin is invalid.
     */
    bool getPinPhaseCorrect(const uint8_t pin) {
        if (myHelpers::isPin(pin) == false) { return false; }
        uint8_t slice = pwm_gpio_to_slice_num(pin);
        return (bool)(_phaseStatus & (1 << slice));
    }
    /** @private */
    void __setPhaseCorrect__(const uint8_t slice, const bool value) {
        pwm_set_phase_correct(slice, value);
        if (value == true) {
            _phaseStatus |= (1 << slice);
        } else {
            _phaseStatus &= ~(1 << slice);
        }
    }
    /**
     * @brief Get the slice wrap value.
     * Gets the wrap value of a given slice. Returns positive (including zero) for the wrap value,
     * otherwise if an invalid slice is given an error code is returned.
     * @param slice Slice to get the value of.
     * @return int32_t returns positive(including zero) for wrap value, negative for error code.
     */
    int32_t getWrap(const uint8_t slice) {
        if (isSlice(slice) == false) { return ERROR_INVALID_SLICE; }
        return _wrapValues[slice];
    }
    /**
     * @brief Get the wrap value of a slice given a pin.
     * Gets the wrap value for the slice that drives a pin. Returns postive (including zero) for
     * the wrap value. Otherwise if an invalid pin is passed an error code is returned.
     * @param pin Pin to get the wrap value of.
     * @return int32_t Positive (including zero) for wrap value, negative for error code.
     */
    int32_t getPinWrap(const uint8_t pin) {
        if (myHelpers::isPin(pin) == false) { return MY_INVALID_PIN; }
        uint8_t slice = pwm_gpio_to_slice_num(pin);
        return _wrapValues[slice];
    }
    /** @private */
    void __setWrap__(const uint8_t slice, const uint16_t wrap) {
        pwm_set_wrap(slice, wrap);
        _wrapValues[slice] = wrap;
    }
    /**
     * @brief Get the level of a channel.
     * Get the current level given a slice and channel. Returns positive (including zero) for the
     * level. If an invalid slice or channel is provided, an error code is returned.
     * @param slice Slice to get the level of.
     * @param channel Channel of the slice.
     * @return int32_t Returns positive (zero included) for level, negative for error code.
     */
    int32_t getLevel(const uint8_t slice, const uint8_t channel) {
        if (isSlice(slice) == false) { return ERROR_INVALID_SLICE; }
        if (isChannel(channel) == false) { return ERROR_INVALID_CHANNEL; }
        return _levelValues[slice][channel];
    }
    /**
     * @brief Get the level of a pin.
     * Get the current level of the controlling slice and channel given a pin. Returns positive
     * (including zero) for the level. Passing an invalid pin will return an error code.
     * @param pin Pin to get the level of.
     * @return int32_t Positive(including zero) level, negative error code.
     */
    int32_t getPinLevel(const uint8_t pin) {
        if (myHelpers::isPin(pin) == false) { return MY_INVALID_PIN; }
        uint8_t slice = pwm_gpio_to_slice_num(pin);
        uint8_t channel = pwm_gpio_to_channel(pin);
        return _levelValues[slice][channel];
    }
    /** @private */
    void __setLevel__(uint8_t slice, uint8_t channel, uint16_t value) {
        pwm_set_chan_level(slice, channel, value);
        _levelValues[slice][channel] = value;
    }
    /**
     * @brief Get the clock divisor integer.
     * Get the integer portion of the clock divisor. Positive, including zero, is the 8 bit 
     * integer. If an invalid slice is passed, an error is raised.
     * @return uint8_t Integer portion of clock divisor.
     */
    int16_t getClockDivisorInt(const uint8_t slice) { 
        if(isSlice(slice) == false) { return ERROR_INVALID_SLICE; }
        return _clkDivInt[slice];
    }
    /**
     * @brief Get the clock divisor integer of the slice given a pin.
     * Get the integer portion of the divisor for the slice that controls a given pin. Returns
     * positive, excluding zero, for the 8 bit integer portion, otherwise if an invalid pin is 
     * passed an error code is returned.
     * @param pin Pin to set the divisor of.
     * @return int16_t Positive, excluding zero is the 8 bit integer, negative for error code.
     */
    int16_t getPinClockDivisorInt(const uint8_t pin) {
        if (myHelpers::isPin(pin) == false) { return ERROR_INVALID_PIN; }
        uint8_t slice = pwm_gpio_to_slice_num(pin);
        return _clkDivInt[slice];
    }
    /**
     * @brief Get the Clock Divisor Fraction portion.
     * Return the clock divisor fration portion. Positive, including zero is the 4 bit
     * fraction.
     * @return uint8_t Fraction portion of clock divisor.
     */
    int16_t getClockDivisorFrac(const uint8_t slice) {
        if (isSlice(slice) == false) { return ERROR_INVALID_SLICE; }
        return _clkDivFrac[slice];
    }
    /**
     * @brief Get the clock divisor for the slice controlling the given pin. Positive, 
     * including zero is the 4 bit fraction portion of the divisor. Otherwise if 
     * an invalid pin is passed, an error code is returned.
     * @param pin Pin to get the divisor for.
     * @return int16_t Positive (including zero) is the 4 bit fraction portion, negative for error code.
     */
    int16_t getPinClockDivisorFrac(const uint8_t pin) {
        if (myHelpers::isPin(pin) == false) { return ERROR_INVALID_PIN; }
        uint8_t slice = pwm_gpio_to_slice_num(pin);
        return _clkDivFrac[slice];
    }
    /**
     * @brief Set the Clock Divisor.
     * Sets the clock divisor given an integer and a fraction, wraps pwm_set_clkdiv_int_frac. Returns
     * 0 (NO_ERROR) if set okay, otherwise if an invalid slice, integer, or fraction are passed, an error
     * code is returned.
     * @param slice Slice to set divisor for.
     * @param integer 8 bit integer portion of divisor. Must be greater than 0.
     * @param fraction 4 bit fraction portion of divisor Must be 0-15 inclusive.
     * @return int16_t Returns 0 (NO_ERROR) if set okay, negative for error code.
     */
    int16_t setClockDivisor(const uint8_t slice, const uint8_t integer, const uint8_t fraction) {
        if (isSlice(slice) == false) { return ERROR_INVALID_SLICE; }
        if (integer == 0) { return ERROR_INVALID_DIVISOR_INT; }
        if (fraction > 15) { return ERROR_INVALID_DIVISOR_FRAC; }
        pwm_set_clkdiv_int_frac(slice, integer, fraction);
        _clkDivInt[slice] = integer;
        _clkDivFrac[slice] = fraction;
        return NO_ERROR;
    }
    /**
     * @brief Set the clock divisor for the slice driving the given pin.
     * Set the clock divisor for the sice controlling the given pin. Returns 0 for set
     * okay, otherwise if an invalid pin, integer, or fraction is passed, and error code
     * is returned.
     * @param pin Pin to set the divisor for.
     * @param integer 8 bit integer portion of the divisor. Must be greater than 0.
     * @param fraction 4 bit fraction portion of the divisor. Must be 0-15 inclusive.
     * @return int16_t Returns 0 (NO_ERROR) if set okay, negative for error code.
     */
    int16_t setPinClockDivisor(const uint8_t pin, const uint8_t integer, const uint8_t fraction) {
        if (myHelpers::isPin(pin) == false) { return ERROR_INVALID_PIN; }
        if (integer == 0) { return ERROR_INVALID_DIVISOR_INT; }
        if (fraction > 15) { return ERROR_INVALID_DIVISOR_FRAC; }
        uint8_t slice = pwm_gpio_to_slice_num(pin);
        pwm_set_clkdiv_int_frac(slice, integer, fraction);
        _clkDivInt[slice] = integer;
        _clkDivFrac[slice] = fraction;
        return NO_ERROR;
    }
/* Initialize functions: */
    /**
     * @brief Initialize a pin as PWM.
     * Initialize a pin as PWM, if the pin is initialized okay, it will return 0. If the pin
     * passed is an invalid pin, or if the pin is already initialized, an error code is returned.
     * @param pin Pin to initialize
     * @return int16_t Returns 0 (NO_ERROR) for initialized okay, or negative for error code.
     */
    int16_t initializePin(const uint8_t pin) {
        if (myHelpers::isPin(pin) == false) { return MY_INVALID_PIN; }
        if (getPinInit(pin) == true) { return ERROR_PIN_ALEREADY_INIT; }
        gpio_set_function(pin, GPIO_FUNC_PWM);
        __setPinInit__(pin, true);
        return NO_ERROR;
    }
    /**
     * @brief Deinitialize a pin.
     * Deinitialize a pin. Sets gpio funciton to NULL. It will return 0 if deinitialized okay. If
     * the pin passed is invalid, or if the pin is not initialized it will return an error code.
     * @param pin Pin to Deinitialize
     * @return int16_t Returns 0 (NO_ERROR) for deinitialized okay, or negative for error code.
     */
    int16_t deinitializePin(const uint8_t pin) {
        if (myHelpers::isPin(pin) == false) { return MY_INVALID_PIN; }
        if (getPinInit(pin) == false) { return ERROR_PIN_NOT_INIT; }
        gpio_set_function(pin, GPIO_FUNC_NULL);
        __setPinInit__(pin, false);
        return NO_ERROR;
    }
/* Enable functions: */
    /**
     * @brief Enable a slice.
     * Enable a PWM slice. Returns 0 for enabled okay, otherwise if an invalid slice number, or if
     * the slice is already enabled, it will return an error code.
     * @param slice Slice to enable.
     * @return int16_t Return 0 (NO_ERROR) for enabled okay, negative for error code.
     */
    int16_t enable(const uint8_t slice) {
        if (isSlice(slice) == false) { return ERROR_INVALID_SLICE; }
        if (getEnabled(slice) == true) {return ERROR_ALREADY_ENABLED; }
        __setEnabled__(slice, true);
        return NO_ERROR;
    }
    /**
     * @brief Enable the slice controlling a given pin.
     * Select and enable the controlling slice of the given pin. Returns 0 (NO_ERROR) if enabled
     * okay, otherwise if already enabled, or an invalid pin is passed, an error code is returned.
     * @param pin Pin to enable.
     * @return int16_t Returns 0 (NO_ERROR) if enabled okay, negative for error code.
     */
    int16_t enablePin(const uint8_t pin) {
        if (myHelpers::isPin(pin) == false) { return ERROR_INVALID_PIN; }
        uint8_t slice = pwm_gpio_to_slice_num(pin);
        if (getEnabled(slice) == true) { return ERROR_ALREADY_ENABLED; }
        __setEnabled__(slice, true);
        return NO_ERROR;
    }
    /**
     * @brief Disable a slice.
     * Disable a PWM slice. Returns 0 for disabled okay, otherwise if an invalid slice number, or if
     * the slice is already disabled, an error code will be returned.
     * @note This will disable both channels on the slice.
     * @param slice Slice to disable.
     * @return int16_t Returns 0 (NO_ERROR) for disabled okay, negative for error code.
     */
    int16_t disable(const uint8_t slice) {
        if (isSlice(slice) == false) { return ERROR_INVALID_SLICE; }
        if (getEnabled(slice) == false) { return ERROR_ALREADY_DISABLED; }
        __setEnabled__(slice, false);
        return NO_ERROR;
    }
    /**
     * @brief Disable a slice given a pin.
     * Disable the controlling slice of a given pin. Returns 0 if disabled okay, otherwise,
     * if an invalid pin or the slice is alredy disabled an error code is returned.
     * @note This will disable the PWM on both channels of the slice.
     * @param pin Pin to disable.
     * @return int16_t Returns 0 (NO_ERROR) for disabled okay, negative for error code.
     */
    int16_t disablePin(const uint8_t pin) {
        if (myHelpers::isPin(pin) == false) { return ERROR_INVALID_PIN; }
        uint8_t slice = pwm_gpio_to_slice_num(pin);
        if (getEnabled(slice) == false) { return ERROR_ALREADY_DISABLED; }
        __setEnabled__(slice, false);
        return NO_ERROR;
    }
/* Phase correct functions */
    /**
     * @brief Enable phase correct given the slice.
     * Enables phase correct on a given slice. Returns 0 for enabled okay, otherwise if an invalid
     * slice number, or if phase correct is already enabled, an error code is returned.
     * @param slice Slice to enable phase correct for.
     * @return int16_t Returns 0 if phase correct enabled okay, negative for error code.
     */
    int16_t enablePhaseCorrect(const uint8_t slice) {
        if (isSlice(slice) == false) { return ERROR_INVALID_SLICE; }
        if (getPhaseCorrect(slice) == true) { return ERROR_PHASE_CORRECT_ENABLED; }
        __setPhaseCorrect__(slice, true);
        return NO_ERROR;
    }
    /**
     * @brief Enable phase correct given a pin.
     * Enables phase correct on the slice controlling a given pin. Returns 0 (NO_ERROR) if enabled
     * okay, or if an invalid pin is passed, an error code is returned.
     * @param pin Pin to set phase correct on.
     * @return int16_t Returns 0 (NO_ERROR) if enabled okay, negaitive for error code.
     */
    int16_t enablePinPhaseCorrect(const uint8_t pin) {
        if (myHelpers::isPin(pin) == false) { return ERROR_INVALID_PIN; }
        uint8_t slice = pwm_gpio_to_slice_num(pin);
        if (getPhaseCorrect(slice) == true) { return ERROR_PHASE_CORRECT_ENABLED; }
        __setPhaseCorrect__(slice, true);
        return NO_ERROR;
    }
    /**
     * @brief Disable phase correct.
     * Disables phase correct for a given slice. Returns 0 for disabled okay, otherwise if an invalid
     * slice number, or if phase correct is already disabled, it will return an error code.
     * @param slice Slice to disable phase correct for.
     * @return int16_t Returns 0 (NO_ERROR) for disabled okay, negative for error code.
     */
    int16_t disablePhaseCorrect(const uint8_t slice) {
        if (isSlice(slice) == false) { return ERROR_INVALID_SLICE; }
        if (getPhaseCorrect(slice) == false) { return ERROR_PHASE_CORRECT_DISABLED; }
        __setPhaseCorrect__(slice, false);
        return NO_ERROR;
    }
    /**
     * @brief Disable phase correct given a pin.
     * Selects the appropriate slice and disables phase correct on it. Returns 0 if disabled okay,
     * otherwise if an invalid pin is passed, and error code is returned.
     * @param pin Pin to disable phase correct for.
     * @return int16_t Return 0 (NO_ERROR) for disabled okay, negative for error code.
     */
    int16_t disablePinPhaseCorrect(const uint8_t pin) {
        if (myHelpers::isPin(pin) == false) { return ERROR_INVALID_PIN; }
        uint8_t slice = pwm_gpio_to_slice_num(pin);
        if (getPhaseCorrect(slice) == false) { return ERROR_PHASE_CORRECT_DISABLED; }
        __setPhaseCorrect__(slice, false);
        return NO_ERROR;
    }
/* Wrap functions: */
    /**
     * @brief Set the Slice wrap value.
     * Sets the wrap value of a given slice.  Returns 0 (NO_ERROR) if set okay, otherwise if an 
     * invalid slice is passed an error code is returned.
     * @param slice Slice to set wrap of.
     * @param wrap Wrap value to set.
     * @return int16_t Returns 0 (NO_ERROR) if set okay, negative for error code.
     */
    int16_t setSliceWrap(const uint8_t slice, const uint16_t wrap) {
        if (isSlice(slice) == false) { return ERROR_INVALID_SLICE; }
        __setWrap__(slice, wrap);
        return NO_ERROR;
    }
    /**
     * @brief Set the slice wrap value, given a pin.
     * Gets the appropriate slice, and sets its wrap value.  Returns 0 (NO_ERROR) if wrap set okay,
     * otherwise if an invalid pin is passed, an error code is returned.
     * @param pin Pin to set wrap of.
     * @param wrap Wrap value to set.
     * @return int16_t Returns 0 (NO_ERROR) for set okay, negative for error code.
     */
    int16_t setPinWrap(const uint8_t pin, const uint16_t wrap) {
        if (myHelpers::isPin(pin) == false) { return MY_INVALID_PIN; }
        uint8_t slice = pwm_gpio_to_slice_num(pin);
        __setWrap__(slice, wrap);
        return NO_ERROR;
    }
/* Frequency functions: */
    /**
     * @brief Set the frequence of a slice.
     * Calculates and set the wrap value of a slice given the slice, and a desired frequency in hz.
     * Returns positive (including zero) is the wrap value set. Otherwise if an invalid slice, or 
     * invalid frequency is passed, an error code is returned.
     * @param slice Slice to set frequency of.
     * @param freq Frequency requested in hz.
     * @return int32_t Returns positive (including zero) for the wrap value set, negaitve
     * for error code.
     */
    int32_t setFrequency(const uint8_t slice, const uint32_t freq) {
        if (isSlice(slice) == false) { return ERROR_INVALID_SLICE; }
        if (freq == 0 or freq > MY_PWM_DEFAULT_FREQ) { return ERROR_INVALID_FREQUENCY; }
        uint32_t clock = MY_PWM_DEFAULT_FREQ;
        if (getPhaseCorrect(slice) == true) { clock = clock / 2; }
        uint32_t divider16 = clock / freq / 4096 + (clock % (freq * 4096) != 0);
        if ((divider16 / 16) == 0) { divider16 = 16; }
        uint32_t wrap = clock * 16 / divider16 / freq - 1;
        pwm_set_clkdiv_int_frac(slice, divider16/16, (divider16 &0x0F));
        __setWrap__(slice, wrap);
        return (int32_t)wrap;
    }
    /**
     * @brief Set the frequency of a slice given a pin.
     * Calculates and set the wrap value for the slice that drives a given pin, and a desired 
     * frequency in hz. Returns positive (including zero) for the wrap value set. Otherwise if 
     * an invalid pin, or frequency is passed an error code is returned.
     * @param pin Pin to set frequency of.
     * @param freq Frequency requested in hz.
     * @return int32_t Returns positive (including zero) for the wrap value set, negative for
     * error code.
     */
    int32_t setPinFrequency(const uint8_t pin, const uint32_t freq) {
        if (myHelpers::isPin(pin) == false) { return MY_INVALID_PIN; }
        if (freq == 0 or freq > MY_PWM_DEFAULT_FREQ) { return ERROR_INVALID_FREQUENCY; }
        const uint8_t slice = pwm_gpio_to_slice_num(pin);
        uint32_t clock = MY_PWM_DEFAULT_FREQ;
        if (getPhaseCorrect(slice) == true) { clock = clock / 2; }
        uint32_t divider16 = clock / freq / 4096 + (clock % (freq * 4096) != 0);
        if ((divider16 / 16) == 0) { divider16 = 16; }
        uint32_t wrap = clock * 16 / divider16 / freq - 1;
        pwm_set_clkdiv_int_frac(slice, divider16/16, (divider16 &0x0F));
        __setWrap__(slice, wrap);
        return (int32_t)wrap;
    }
/* Level functions: */
    /**
     * @brief Set the level given slice and channel.
     * Sets the level given a slice and channel. Returns 0 (NO_ERROR) for set okay, otherwise if 
     * an invalid slice or channel are given, an error code is returned.
     * @param slice Slice to set.
     * @param channel Channel to set.
     * @param value Value to set level to.
     * @return int16_t Returns 0 (NO_ERROR) for set okay, negative for error code.
     */
    int16_t setLevel(const uint8_t slice, const uint8_t channel, const uint16_t value) {
        if (isSlice(slice) == false) { return ERROR_INVALID_SLICE; }
        if (isChannel(channel) == false) { return ERROR_INVALID_CHANNEL; }
        __setLevel__(slice, channel, value);
        return NO_ERROR;
    }
    /**
     * @brief Set the level given a pin.
     * Sets the level given a pin. Returns 0 (NO_ERROR) if set okay, otherwise if an
     * invalid pin is passed, an error code is returned.
     * @param pin 
     * @param value 
     * @return int16_t 
     */
    int16_t setPinLevel(const uint8_t pin, const uint16_t value) {
        if (myHelpers::isPin(pin) == false) { return ERROR_INVALID_PIN; }
        uint8_t slice = pwm_gpio_to_slice_num(pin);
        uint8_t channel = pwm_gpio_to_channel(pin);
        __setLevel__(slice, channel, value);
        return NO_ERROR;
    }
/* Duty functions: */
    /**
     * @brief Calculate and set level given duty cycle.
     * Calculate and set the level of a given slice and channel. Returns 0 (NO_ERROR) if
     * the level was set okay, otherwise if an invalid slice, channel or duty are passed
     * an error code is returned.
     * @param slice Slice to set level of.
     * @param channel Channel to set level of.
     * @param duty Duty cycle to set. Valid values 0-100 inclusive.
     * @return int32_t Returns 0 (NO_ERROR) if set okay, negative for error code.
     */
    int32_t setDuty(const uint8_t slice, const uint8_t channel, const uint8_t duty) {
        // __breakpoint();
        if (isSlice(slice) == false) { return ERROR_INVALID_SLICE; }
        if (isChannel(channel) == false) { return ERROR_INVALID_CHANNEL; }
        if (duty > 100) { return ERROR_INVALID_DUTY; }
        uint16_t level = getWrap(slice) * duty / 100;
        __setLevel__(slice, channel, level);
        return NO_ERROR;
    }
    /**
     * @brief Set the level of a pin given a duty cycle.
     * Calculate and set the level given a duty cycle. Returns 0 (NO_ERROR) if the level
     * was set okay, otherwise if an invalid duty or pin are passed, and error code is returned.
     * @param pin Pin to set the level of.
     * @param duty Duty cycle. Valid values 0-100 inclusive.
     * @return int32_t Return 0 (NO_ERROR) if set okay, negative for error code.
     */
    int32_t setPinDuty(const uint8_t pin, const uint8_t duty) {
        if (myHelpers::isPin(pin) == false) { return ERROR_INVALID_PIN; }
        uint8_t slice = pwm_gpio_to_slice_num(pin);
        uint8_t channel = pwm_gpio_to_channel(pin);
        uint16_t level = getWrap(slice) * duty / 100;
        __setLevel__(slice, channel, level);
        return NO_ERROR;
    }
};
#endif