/**
 * @file myADC.hpp
 * @author Peter Nearing (pnearing@protonmail.com)
 * @brief High level functions for the ADC.
 * @version 0.1
 * @date 2022-06-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MY_ADC_H
#define MY_ADC_H

#include <hardware/adc.h>

#include "../myStandardDefines.hpp"
#include "../myErrorCodes.hpp"

// Status feild assignments:
#define _MY_CHANNEL_0_MASK      0b00000001
#define _MY_CHANNEL_1_MASK      0b00000010
#define _MY_CHANNEL_2_MASK      0b00000100
#define _MY_CHANNEL_3_MASK      0b00001000
#define _MY_TEMP_CHANNEL_MASK   0b00010000
#define _MY_ADC_MASK            0b00100000

/**
 * @brief High level functions for the ADC.
 * Helper functions to track and set state of ADC pins.
 */
namespace myADC {
/* Constants: */
    // Error constants:
    /**
     * @brief No error. Value: 0
     * No error.
     */
    const int16_t NO_ERROR                      = MY_NO_ERROR;                          // 0 : No error.
    /**
     * @brief Invalid channel. Value -200
     * An invalid channel was provided.
     */
    const int16_t ERROR_INVALID_CHANNEL         = MY_ERROR_ADC_INVALID_CHANNEL;         // -200 : Channel is invalid
    /**
     * @brief Invalid pin. Value -201
     * An invalid pin was provided, ie pin is not associated with a channel.
     */
    const int16_t ERROR_INVALID_PIN             = MY_ERROR_ADC_INVALID_PIN;             // -201 : Pin is not associated with a channel.
    /**
     * @brief ADC not initialized. Value -202
     * Initialize has not been called for the ADC at time of read.
     */
    const int16_t ERROR_ADC_NOT_INIT            = MY_ERROR_ADC_ADC_NOT_INIT;            // -202 : ADC has not been initialized at time of read.
    /**
     * @brief Channel not intialized. Value -203
     * Channel was not initialized at time of read.
     */
    const int16_t ERROR_CHANNEL_NOT_INIT        = MY_ERROR_ADC_CHANNEL_NOT_INIT;        // -203 : Channel has not been initialize at time of read.
    /**
     * @brief Channel already initialized.
     * Channel was already initialized when init was called.
     */
    const int16_t ERROR_CHANNEL_ALREADY_INIT    = MY_ERROR_ADC_CHANNEL_ALREADY_INIT;    // -204 : Channel already initialized.
/* Variables: */
    /** @private */
    volatile uint8_t _status = 0x00; // Status byte, stores if channels / adc has been init.
/* Functions: */
// Validation functions:
    /**
     * @brief Return true if valid channel.
     * Verify that a channel provided is valid for the rp2040.
     * @param channel Channel to verify.
     * @return true Channel is valid.
     * @return false Channel is invalid.
     */
    bool inline validChannel(const uint8_t channel) {
        if (channel > (MY_ADC_NUM_CHANNELS - 1)) { return false; }
        return true;
    }
    /**
     * @brief Return true if channel is a user channel.
     * Verify that a channel provided is a valid user channel (not connected to temp sensor.)
     * @param channel Channel to verify
     * @return true Channel is a user channel.
     * @return false Channel is not a user channel.
     */
    bool inline validUserChannel(const uint8_t channel) {
        if (channel > (MY_ADC_NUM_USER_CHANNELS - 1)) { return false; }
        return true;
    }
    /**
     * @brief Return true if a pin is an ADC pin.
     * Verify that a pin provided is a valid ADC pin on the rp2040.
     * @param pin 
     * @return true 
     * @return false 
     */
    bool inline validPin(const uint8_t pin) {
        if (pin < MY_ADC_CHANNEL_0_PIN or pin > MY_ADC_CHANNEL_3_PIN) { return false; }
        return true;
    }
// Lookup functions:
    /**
     * @brief Convert a channel to a pin number.
     * 
     * @param channel Channel to convert.
     * @return int16_t Positive is pin number, negative is error code
     */
    int16_t inline channelToPin (uint8_t channel) {
        if (validUserChannel(channel) == false) { return ERROR_INVALID_CHANNEL; }
        return (channel + MY_ADC_CHANNEL_0_PIN);
    }
    /**
     * @brief Convert a pin number to a channel.
     * 
     * @param pin Pin to convert.
     * @return int16_t Positive is channel number, negative is error code.
     */
    int16_t inline pinToChannel (uint8_t pin) {
        if (validPin(pin) == false) { return ERROR_INVALID_PIN; }
        return (pin - MY_ADC_CHANNEL_0_PIN);
    }
// Status byte functions:
    /**
     * @brief Return true if ADC init has been called.
     * 
     * @return true ADC has been initialized.
     * @return false ADC has not been intitialized.
     */
    bool inline getADCInit() {
        return (bool)(_status & _MY_ADC_MASK);
    }
    /**
     * @brief Return true if channel has been initialized.
     * 
     * @param channel Channel to verify.
     * @return true if channel has been initialized.
     * @return false if channel is not initialized, or is invalid.
     */
    bool inline getChannelInit(uint8_t channel) { 
        if (validUserChannel(channel) == false) { return false; }
        return (bool)(_status & (1<<channel));
    }
    /**
     * @brief Return true if pin has been initialized.
     * 
     * @param pin Pin to verify.
     * @return true If channel pin is associated with is initialized.
     * @return false If pin is not initialized, or is invalid.
     */
    bool inline getPinInit(uint8_t pin) {
        if (validPin(pin) == false) { return false; }
        return (bool) (_status & (1 << pinToChannel(pin)));
    }
    /**
     * @brief Return true if temperature sensor is initialized.
     * 
     * @return true Temperature sensor initialized.
     * @return false Temperature sensor not initialized.
     */
    bool inline getTemperatureInit() {
        return (bool) (_status & _MY_TEMP_CHANNEL_MASK);
    }
    // Set ADC INIT:
    /** @private */
    void inline _setADCInit() {
        _status |= _MY_ADC_MASK;
    }
    // Set Channel as init.
    /** @private */
    void inline _setChannelInit(const uint8_t channel) {
        if (validUserChannel(channel) == false) { return; }
        _status |= (1 << channel);
    }
    // Set Pin as init.
    /** @private */
    void inline _setPinInit(const uint8_t pin) {
        if (validPin(pin) == false) { return; }
        _status |= (1 << pinToChannel(pin));

    }
    // Set temp as init:
    /** @private */
    void inline _setTemperatureInit(const bool value) {
        if (value == true) {
            _status |= _MY_TEMP_CHANNEL_MASK;
        } else {
            _status &= ~_MY_TEMP_CHANNEL_MASK;
        }
    }
// Initialize functions:
    /**
     * @brief Call adc_init.
     * Calles adc_init from the pico-sdk. This function is safe to call multiple times, as we only call
     * adc_init once.
     * @return true If adc_init was called.
     * @return false If adc_init was not called, ie: adc_init already called.
     */
    bool inline initialize() { 
        if (getADCInit() == false) {
            adc_init(); 
            _setADCInit();
            return true;
        }
        return false;
    }
    /**
     * @brief Select the pin and call adc_gpio_init.
     * Selects the pin given a channel, and call adc_gpio_init for the pin, this is safe to call multiple
     * times as it tracks if it's been called. Returns an error code if the ADC hasn't been initialized,
     * or if the channel is an invalid value, or if the channel has already been initialized.
     * @param channel Channel to initialize.
     * @return int16_t Returns 0 (NO_ERROR) for no error, negative for error code.
     */
    int16_t inline initChannel(const uint8_t channel) {
        if (getADCInit() == false) { return ERROR_ADC_NOT_INIT; }
        if (validUserChannel(channel) == false) { return ERROR_INVALID_CHANNEL; }
        if (getChannelInit(channel) == true) { return ERROR_CHANNEL_ALREADY_INIT; }
        adc_gpio_init(channelToPin(channel));
        _setChannelInit(channel);
        return NO_ERROR;
    }
    /**
     * @brief Call adc_gpio_init for a given pin.
     * Calls adc_gpio_init for a given pin, this is safe to call multiple times as the state is tracked.
     * It will return an error code if the ADC hasn't been initialized, if the pin is an invalid ADC pin,
     * or if the pin has already been initialized.
     * @param pin Pin to initialize.
     * @return int16_t Returns 0 (NO_ERROR) for no error, negative for error code.
     */
    int16_t inline initPin(const uint8_t pin) {
        if (getADCInit() == false) { return ERROR_ADC_NOT_INIT; }
        if (validPin(pin) == false) { return ERROR_INVALID_PIN; }
        if (getPinInit(pin) == true) { return ERROR_CHANNEL_ALREADY_INIT; }
        adc_gpio_init(pin);
        _setPinInit(pin);
        return NO_ERROR;
    }
    /**
     * @brief Initialize the temperature sensor.
     * Calls adc_set_temp_sensor_enabled with true, and tracks if it's been called, this function is
     * therefore safe to call multiple times. It will return an error code if the ADC has not been 
     * initialized, or if the temperature channel has already been initialized.
     * @return int16_t Returns 0 (NO_ERROR) for init okay, and negative for error code. (already init.)
     */
    int16_t inline initTemperature() {
        if (getADCInit() == false) { return ERROR_ADC_NOT_INIT; }
        if (getTemperatureInit() == true) { return ERROR_CHANNEL_ALREADY_INIT; }
        adc_set_temp_sensor_enabled(true);
        _setTemperatureInit(true);
        return NO_ERROR;
    }
    /**
     * @brief De intialize the temperature sensor.
     * Calls adc_set_temp_sensor_enabled with false, and tracks it. this function is therefore safe to call
     * multiple times. Returns an error code if the temperature channel is not initialized.
     * @return int16_t Return 0 (NO_ERROR) for deinit okay, and negative for error code (already de init.) 
     */
    int16_t inline deinitTemperature() {
        if (getTemperatureInit() == false) { return ERROR_CHANNEL_NOT_INIT; }
        adc_set_temp_sensor_enabled(false);
        _setTemperatureInit(false);
        return NO_ERROR;
    }
// Raw Read functions:
    /**
     * @brief Return the raw ADC reading for a channel.
     * Selects the given channel and preforms a read, and returns the value. Returns an error code if
     * the ADC has not been initialized, an invalid channel has been given, or if the given channel has
     * not been initialized.
     * @param channel Channel to read
     * @return int16_t Positive (including zero) adc reading, negative for error code.
     */
    int16_t readChannelRaw(const uint8_t channel) {
        if (getADCInit() == false) { return ERROR_ADC_NOT_INIT; }
        if (validUserChannel(channel)== false) { return ERROR_INVALID_CHANNEL; }
        if (getChannelInit(channel) == false) { return ERROR_CHANNEL_NOT_INIT; }
        adc_select_input(channel);      // Select channel.
        return (int16_t)(adc_read());   // return reading.
    }
    /**
     * @brief Return the raw ADC for a given pin.
     * Selects the appropriate channel given a pin, preforms a read, and returns the value. Returns an
     * error code if the ADC has not been initialized, the pin is an invalid ADC pin, or if the pin
     * has not been initialized.
     * @param pin Pin to read.
     * @return int16_t Positive (including zero) adc reading, negative for error code.
     */
    int16_t readPinRaw(const uint8_t pin) {
        if (getADCInit() == false) { return ERROR_ADC_NOT_INIT; }
        if (validPin(pin) == false) { return ERROR_INVALID_PIN; }
        if (getPinInit(pin) == false) { return ERROR_CHANNEL_NOT_INIT; }
        adc_select_input(pinToChannel(pin));    // Select channel.
        return (int16_t)(adc_read());           // return reading.
    }
    /**
     * @brief Read the raw temperature value.
     * Selects the internal temperature sensor, preforms the read, and returns the value. Returns an
     * error code if the ADC has not been initialized, or if the internal temperature sensor channel
     * has not been initialized.
     * @return int16_t Positive (including zero) adc reading, negative for error code.
     */
    int16_t readTemperatureRaw() {
        if (getADCInit() == false) { return ERROR_ADC_NOT_INIT; }
        if (getTemperatureInit() == false) { return ERROR_CHANNEL_NOT_INIT; }
        adc_select_input(MY_ADC_TEMP_CHANNEL);  // Select temp channel.
        return (int16_t)(adc_read());       // return reading.
    }
// Converted read functions:
    /**
     * @brief Calculate the voltage of a channel given VREF.
     * Selects the given channel, preforms a read, converts it to a voltage given VREF,
     * and returns the value. Returns an error code if the ADC has not been intialized,
     * if the channel is invalid, or the channel has not been initialized.
     * @param channel Channel to read.
     * @param vRef Voltage applied to the vref pin, default 3.3v
     * @return float: Positive (including zero) is Voltage on channel. Negative is error code.
     */
    float readChannelVoltage(const uint8_t channel, const float vRef=3.3f) {
        const float conversion_factor = vRef / (1 << 12);   // Calculate conversion factor
        int16_t reading = readChannelRaw(channel);  // Get the channel reading.
        if (reading < 0) { return (float)reading; } // If error returned return the error.
        return (float) ((float)reading * conversion_factor);
    }
    /**
     * @brief Calculate the voltage of a pin, given VREF
     * Selects the appropriate channel given a pin, preforms a read, converts it to a voltage
     * given VREF, and returns the value.  Returns an error code if the ADC has not been
     * initialized, the pin is an invalid ADC pin, or if the pin has not been initialized.
     * @param pin Pin to read.
     * @param vRef Voltage applied to the VREF pin, default 3.3v
     * @return float: Positive (including zero) is Voltage on pin. Negative is error code.
     */
    float readPinVoltage(const uint8_t pin, const float vRef=3.3f) {
        const float conversion_factor = vRef / (1 << 12);   // Calculate conversion factor
        int16_t reading = readPinRaw(pin);  // Get the raw reading.
        if (reading < 0) { return (float)reading; } // if error returned pass on the error.
        return (float) ((float)reading * conversion_factor);
    }
    /**
     * @brief Return temperature of sensor.
     * Selects the internal temperature sensor channel, preforms a read, and converts it to either
     * degress celcius or degrees farenheit. Returns an error code if the ADC has not been initialized,
     * or if the internal temperature sensor channel has not been initialized.
     * @param returnDegC True, return DegC, False, retrun DegF.
     * @return float Values greater than -199.99... is the temperature. Values of -200 or less are
     *              error codes.
     */
    float readTemperature(const bool returnDegC=true) {
        float conversion_factor = 3.3f / (1 << 12); // Calculate the conversion factor.
        int16_t reading = readTemperatureRaw(); // Get the raw reading
        if (reading < 0) { return (float)reading; }
        float degC = (27.0f -(((float)reading*conversion_factor) - 0.706f) / 0.001721f);
        if (returnDegC == true) {
            return degC; // return Deg C.
        } else {
            return ((degC * (9.0f / 5.0f)) + 32.0f); // return Deg F
        }
    }
};

#endif