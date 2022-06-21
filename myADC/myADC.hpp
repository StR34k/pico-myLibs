/*
 * myADC
 *  Higer level interface to the raspberry pi pico.
 *      // Checks:
 *              validChannel: returns true if channel is valid. (includes temp sensor.)
 *              validPin    : returns true if pin is valid.
 *              validUserChannel: returns true if channel is a valid user channel. (doesn't include temp sensor)
 *      // Lookups:
 *              pinToChannel: convert a pin number to a channel number: returns positive channel number, negative error code.
 *              channelToPin: convert a channel number to a pin number: returns positive: pin number, negative error code.
 *      // Initialize:
 *              initialize  : calles adc_init()
 *              initPin     : calls adc_gpio_init() for the pin. returns 0 for okay, negative for error code.
 *              initChannel : calls adc_gpio_init() for the associated pin for the channel, returns 0 for okay, negative for error code.
 *              initTemperature: Initialize the temperature sensor.
 *              deinitTemperature: de initialize the temperature sensor.
 *      // Read Raw:
 *              readChannelRaw:  return the raw value for the channel. returns positvie for value, negative for error code.
 *              readPinRaw:      return the raw value for the channel associated to pin. Returns positive for value, negative for error code.
 *              readTemperatureRaw: return the raw value for the temp sensor. returns positive for raw value.
 *      // Read Converted:  
 *              readChannelVoltage:    returns the voltage given a channel. Params: float VREF which defautls to 3.3v. return positive for voltage, negaitve for error code.
 *              readPinVoltage:         returns the voltage given a pin.    Params: float VREF which defaults to 3.3v. return positive for voltage, negative for error code.
 *              readTemperature:    returns the temperature. Params: bool returnDegC, true to return degC, false to return degF. returns temperature. or error code. *NOTE: temperatures less than -199.999* collide with error codes.
 * 
 * 
 */ 


#ifndef MY_ADC_H
#define MY_ADC_H

#include <hardware/adc.h>
#include "../myErrorCodes.hpp"

// Channel numbers:
#define MY_CHANNEL_0 0
#define MY_CHANNEL_1 1
#define MY_CHANNEL_2 2
#define MY_CHANNEL_3 3
#define MY_TEMP_CHANNEL 4
// Pin numbers:
#define MY_CHANNEL_0_PIN 26
#define MY_CHANNEL_1_PIN 27
#define MY_CHANNEL_2_PIN 28
#define MY_CHANNEL_3_PIN 29
// Num channels:
#define MY_NUM_USER_CHANNELS 4
#define MY_NUM_CHANNELS 5
// Status feild assignments:
#define _MY_CHANNEL_0_MASK      0b00000001
#define _MY_CHANNEL_1_MASK      0b00000010
#define _MY_CHANNEL_2_MASK      0b00000100
#define _MY_CHANNEL_3_MASK      0b00001000
#define _MY_TEMP_CHANNEL_MASK   0b00010000
#define _MY_ADC_MASK            0b00100000

namespace myADC {
/* Constants: */
    // Error constants:
    const int16_t NO_ERROR                      = MY_NO_ERROR;                          // 0 : No error.
    const int16_t ERROR_INVALID_CHANNEL         = MY_ERROR_ADC_INVALID_CHANNEL;         // -200 : Channel is invalid
    const int16_t ERROR_INVALID_PIN             = MY_ERROR_ADC_INVALID_PIN;             // -201 : Pin is not associated with a channel.
    const int16_t ERROR_ADC_NOT_INIT            = MY_ERROR_ADC_ADC_NOT_INIT;            // -202 : ADC has not been initialized at time of read.
    const int16_t ERROR_CHANNEL_NOT_INIT        = MY_ERROR_ADC_CHANNEL_NOT_INIT;        // -203 : Channel has not been initialize at time of read.
/* Variables: */
    volatile uint8_t _status = 0x00; // Status byte, stores if channels / adc has been init.
/* Functions: */
// Validation functions:
    bool inline validChannel(const uint8_t channel) {
        if (channel > (MY_NUM_CHANNELS - 1)) { return false; }
        return true;
    }

    bool inline validUserChannel(const uint8_t channel) {
        if (channel > (MY_NUM_USER_CHANNELS - 1)) { return false; }
        return true;
    }

    bool inline validPin(const uint8_t pin) {
        if (pin < MY_CHANNEL_0_PIN or pin > MY_CHANNEL_3_PIN) { return false; }
        return true;
    }
// Lookup functions:
    int16_t inline channelToPin (uint8_t channel) {
        if (validUserChannel(channel) == false) { return ERROR_INVALID_CHANNEL; }
        return (channel + MY_CHANNEL_0_PIN);
    }

    int16_t inline pinToChannel (uint8_t pin) {
        if (validPin(pin) == false) { return ERROR_INVALID_PIN; }
        return (pin - MY_CHANNEL_0_PIN);
    }
// Status byte functions:
    // If ADC is init.
    bool inline getADCInit() {
        return (bool)(_status & _MY_ADC_MASK);
    }
    // If a given channel is init:
    bool inline getChannelInit(uint8_t channel) { 
        if (validUserChannel(channel) == false) { return false; }
        return (bool)(_status & (1<<channel));
    }
    // If a given pin is init:
    bool inline getPinInit(uint8_t pin) {
        if (validPin(pin) == false) { return false; }
        return (bool) (_status & (1 << pinToChannel(pin)));
    }
    // If the temp sensor is init:
    bool inline getTemperatureInit() {
        return (bool) (_status & _MY_TEMP_CHANNEL_MASK);
    }
    // Set ADC INIT:
    void inline _setADCInit() {
        _status |= _MY_ADC_MASK;
    }
    // Set Channel as init.
    void inline _setChannelInit(const uint8_t channel) {
        if (validUserChannel(channel) == false) { return; }
        _status |= (1 << channel);
    }
    // Set Pin as init.
    void inline _setPinInit(const uint8_t pin) {
        if (validPin(pin) == false) { return; }
        _status |= (1 << pinToChannel(pin));

    }
    // Set temp as init:
    void inline _setTemperatureInit(const bool value) {
        if (value == true) {
            _status |= _MY_TEMP_CHANNEL_MASK;
        } else {
            _status &= ~_MY_TEMP_CHANNEL_MASK;
        }
    }


// Initialize functions:
    // Call adc init.
    bool inline initialize() { 
        if (getADCInit() == false) {
            adc_init(); 
            _setADCInit();
            return true;
        }
        return false;
    }
    // Select the pin from a given channel and call adc_gpio_init.
    int16_t inline initChannel(const uint8_t channel) {
        if (validUserChannel(channel) == false) { return ERROR_INVALID_CHANNEL; }
        adc_gpio_init(pinToChannel(channel));
        _setChannelInit(channel);
        return NO_ERROR;
    }
    // Call adc_gpio_init for a given pin.
    int16_t inline initPin(const uint8_t pin) {
        if (validPin(pin) == false) { return ERROR_INVALID_PIN; }
        adc_gpio_init(pin);
        _setPinInit(pin);
        return NO_ERROR;
    }
    // Initialize the temperature sensor.
    void inline initTemperature() {
        adc_set_temp_sensor_enabled(true);
        _setTemperatureInit(true);
    }
    // De initialzie the temperature sensor.
    void inline deinitTemperature() {
        adc_set_temp_sensor_enabled(false);
        _setTemperatureInit(false);
    }
// Raw Read functions:
    // Return the raw ADC reading for a given channel.
    int16_t readChannelRaw(const uint8_t channel) {
        if (validUserChannel(channel)== false) { return ERROR_INVALID_CHANNEL; }
        if (getADCInit() == false) { return ERROR_ADC_NOT_INIT; }
        if (getChannelInit(channel) == false) { return ERROR_CHANNEL_NOT_INIT; }
        adc_select_input(channel);      // Select channel.
        return (int16_t)(adc_read());   // return reading.
    }
    // Select the appropriate channel given a pin, and return the raw reading.
    int16_t readPinRaw(const uint8_t pin) {
        if (validPin(pin) == false) { return ERROR_INVALID_PIN; }
        if (getADCInit() == false) { return ERROR_ADC_NOT_INIT; }
        if (getPinInit(pin) == false) { return ERROR_CHANNEL_NOT_INIT; }
        adc_select_input(pinToChannel(pin));    // Select channel.
        return (int16_t)(adc_read());           // return reading.
    }
    // Select the temperature channel and return the raw reading.
    int16_t readTemperatureRaw() {
        if (getADCInit() == false) { return ERROR_ADC_NOT_INIT; }
        if (getTemperatureInit() == false) { return ERROR_CHANNEL_NOT_INIT; }
        adc_select_input(MY_TEMP_CHANNEL);  // Select temp channel.
        return (int16_t)(adc_read());       // return reading.
    }
// Converted read functions:
    // Calculate the voltage given VREF, and a channel.
    float readChannelVoltage(const uint8_t channel, const float vRef=3.3f) {
        const float conversion_factor = vRef / (1 << 12);   // Calculate conversion factor
        int16_t reading = readChannelRaw(channel);  // Get the channel reading.
        if (reading < 0) { return (float)reading; } // If error returned return the error.
        return (float) ((float)reading * conversion_factor);
    }
    // Calculate the voltage given VREF and a pin.
    float readPinVoltage(const uint8_t pin, const float vRef=3.3f) {
        const float conversion_factor = vRef / (1 << 12);   // Calculate conversion factor
        int16_t reading = readPinRaw(pin);  // Get the raw reading.
        if (reading < 0) { return (float)reading; } // if error returned pass on the error.
        return (float) ((float)reading*conversion_factor);
    }
    // Return the temperature in degress celcius / farenheit.
    float readTemperature(const bool returnDegC=true) {
        float conversion_factor = 3.3f / (1 << 12); // Calculate the conversion factor.
        int16_t reading = readTemperatureRaw(); // Get the raw reading
        if (reading < 0) { return (float)reading; }
        float degC = (27.0f -(((float)reading*conversion_factor) - 0.706f) / 0.001721f);
        if (returnDegC == true) {
            return degC; // return Deg C.
        } else {
            return ((degC * (9.0f / 5.0f)) + 32.0); // return Deg F
        }
    }
};

#endif