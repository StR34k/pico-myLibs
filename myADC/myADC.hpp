#ifndef MY_ADC_H
#define MY_ADC_H

#include <hardware/adc.h>

#define MY_CHANNEL_0 0
#define MY_CHANNEL_1 1
#define MY_CHANNEL_2 2
#define MY_CHANNEL_3 3
#define MY_TEMP_CHANNEL 4

#define MY_CHANNEL_0_PIN 26
#define MY_CHANNEL_1_PIN 27
#define MY_CHANNEL_2_PIN 28
#define MY_CHANNEL_3_PIN 29

#define MY_NUM_USER_CHANNELS 4
#define MY_NUM_CHANNELS 5

namespace myADC {
/* Constants: */
    // Error constants:
    const int16_t NO_ERROR                      = MY_NO_ERROR;                          // No error.
    const int16_t ERROR_INVALID_CHANNEL         = MY_ERROR_ADC_INVALID_CHANNEL;         // Channel is invalid
    const int16_t ERROR_INVALID_PIN             = MY_ERROR_ADC_INVALID_PIN;             // Pin is not associated with a channel.
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
// Initialize functions:
    // Call adc init.
    void inline initialize() { 
        adc_init(); 
    }
    // Select the pin from a given channel and call adc_gpio_init.
    int16_t inline initChannel(const uint8_t channel) {
        if (validUserChannel(channel) == false) { return ERROR_INVALID_CHANNEL; }
        adc_gpio_init(pinToChannel(channel));
        return NO_ERROR;
    }
    // Call adc_gpio_init for a given pin.
    int16_t inline initPin(const uint8_t pin) {
        if (validPin(pin) == false) { return ERROR_INVALID_PIN; }
        adc_gpio_init(pin);
        return NO_ERROR;
    }
    // Initialize the temperature sensor.
    void inline initTemperature() {
        adc_set_temp_sensor_enabled(true);
    }
// Raw Read functions:
    // Return the raw ADC reading for a given channel.
    int16_t readChannelRaw(const uint8_t channel) {
        if (validUserChannel(channel)== false) { return ERROR_INVALID_CHANNEL; }
        adc_select_input(channel);      // Select channel.
        return (int16_t)(adc_read());   // return reading.
    }
    // Select the appropriate channel given a pin, and return the raw reading.
    int16_t readPinRaw(const uint8_t pin) {
        if (validPin(pin) == false) { return ERROR_INVALID_PIN; }
        adc_select_input(pinToChannel(pin));    // Select channel.
        return (int16_t)(adc_read());           // return reading.
    }
    // Select the temperature channel and return the raw reading.
    int16_t readTemperatureRaw() {
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
        if (reading <0) { return (float)reading; } // if error returned pass on the error.
        return (float) ((float)reading*conversion_factor);
    }
    // Return the temperature in degress celcius.
    float readTemperature() {
        float conversion_factor = 3.3f / (1<<12); // Calculate the conversion factor.
        int16_t reading = readTemperatureRaw(); // Get the raw reading
        return (float)(27 -(((float)reading*conversion_factor)-0.706)/0.001721); // return Deg C.
    }
};

#endif