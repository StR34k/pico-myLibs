/**
 * @file mySNx4HC595.hpp
 * @author Peter Nearing (pnearing@protonmail.com)
 * @brief Class to use a 595 shift register.
 * @version 0.1
 * @date 2022-06-23
 * 
 * @copyright Copyright (c) 2022
 */

/**      
 *      A Class to use a 595 shift register.
 *      Use startWrite to start a write to the shift register, then writeBit or writeByte to send data to 
 *      the register. stopWrite is then called to latch in the data.  Daisy chained shift registers can be
 *      written to by calling write bit / write byte multiple times. If you want to define clear without
 *      without defining enable, you can set enable to MY_NOT_A_PIN to disable it. To use PWM on the enable
 *      pin, call initialize with a uint8_t value to initilize the PWM, otherwise we default back to GPIO,
 *      and calling setEnable with a uint8_t value, will be turned into a boolean.
 * 
 *      Datasheet: https://www.ti.com/lit/ds/symlink/sn74hc595.pdf
 * 
 */
#ifndef MY_SNX4HC595_H
#define MY_SNX4HC595_H

#include <hardware/gpio.h>
#include <hardware/pwm.h>

#include "../myStandardDefines.hpp"
#include "../myErrorCodes.hpp"
#include "../myHelpers.hpp"

class my595 {
    public:
    /* Constants: */
        // Errors:
        static const int16_t NO_ERROR = MY_NO_ERROR;                                        /**< 0 : No error. */
        static const int16_t ERROR_ENABLE_NOT_DEFINED = MY_ERROR_MY595_ENABLE_NOT_DEFINED;  /**< -400 : Enable pin not defined. */
        static const int16_t ERROR_CLEAR_NOT_DEFINED = MY_ERROR_MY595_CLEAR_NOT_DEFINED;    /**< -401 : Clear pin not defined. */


    /* Constructors: */
        /**
         * @brief Construct a new my595 object
         * 
         * @param latch Latch pin number.
         * @param clock Clock pin number.
         * @param data Data pin number.
         */
        my595(const uint8_t latch, const uint8_t clock, const uint8_t data) : _latchPin (latch),
                    _clkPin (clock), _dataPin(data) {}
        /**
         * @brief Construct a new my595 object
         * 
         * @param latch Latch pin number
         * @param clock Clock pin number.
         * @param data Data pin number.
         * @param enable Enable pin number.
         */
        my595(const uint8_t latch, const uint8_t clock, const uint8_t data, const uint8_t enable) :
                    _latchPin (latch), _clkPin (clock), _dataPin (data), _enablePin (enable) {}
        /**
         * @brief Construct a new my595 object
         * 
         * @param latch Latch pin number.
         * @param clock Clock pin number.
         * @param data Data pin number.
         * @param enable Enable pin number.
         * @param clear Clear pin number.
         */
        my595(const uint8_t latch, const uint8_t clock, const uint8_t data, const uint8_t enable,
                    const uint8_t clear) : _latchPin (latch), _clkPin (clock), _dataPin (data),
                    _enablePin (enable), _clearPin (clear) {}
    /* Functions: */
        int16_t initialize();
        int16_t initialize(const bool enable);
        int16_t initialize(const uint8_t enable);

        void    startWrite(); // Lowers the latch.
        void    writeBit(const bool value); // Shift in one bit.
        void    writeByte(const uint8_t value, const bool bitOrder=MSB_FIRST); // Shift in one byte.
        void    stopWrite(); // raise the latch.
        int16_t clear();
        int16_t setEnable(const bool value);
        int16_t setEnable(const uint8_t value);

    private:
    /* Variables: */
        const uint8_t _latchPin = MY_NOT_A_PIN;
        const uint8_t _clkPin = MY_NOT_A_PIN;
        const uint8_t _dataPin = MY_NOT_A_PIN;
        const uint8_t _enablePin = MY_NOT_A_PIN;
        const uint8_t _clearPin = MY_NOT_A_PIN;
        uint8_t       _pwmSlice;
        uint8_t       _pwmChannel;
        bool          _usePWM = false;
    /* Functions: */
        void __initDataPins__();
        void __initClearPin__();
        bool __initEnableGPIO__(const bool value);
        bool __initEnablePWM__(const uint8_t value);
};
/**
 * @brief intiialize(): Initalize the shfit register.
 * 
 *  Initialize with no params assumes output enabled in gpio mode.
 * 
 * @return int16_t Return 0 (MY_NO_ERROR) for okay, negative for error code.
 */
int16_t my595::initialize() {
// Check pins:
    if (myHelpers::isPin(_latchPin) == false) { return MY_INVALID_PIN; }
    if (myHelpers::isPin(_dataPin) == false) { return MY_INVALID_PIN; }
    if (myHelpers::isPin(_clkPin) == false) { return MY_INVALID_PIN; }
// Init Pins:
    __initDataPins__();
// Init clear pin
    __initClearPin__();
// Init enable pin    
    if (__initEnableGPIO__(true) == false) { return ERROR_ENABLE_NOT_DEFINED; }
    _usePWM = false;
    return NO_ERROR;
}
/**
 * @brief initialize(bool): Initialize the shift register.
 *  Initialize the shift register, and if defined set enable to enable value.
 * 
 * @param enable If the output's should be enabled on init.
 * @return int16_t returns 0 (MY_NO_ERROR) for okay, and negative for error code.
 */
int16_t my595::initialize(const bool enable) {
// Check pins:
    if (myHelpers::isPin(_latchPin) == false) { return MY_INVALID_PIN; }
    if (myHelpers::isPin(_dataPin) == false) { return MY_INVALID_PIN; }
    if (myHelpers::isPin(_clkPin) == false) { return MY_INVALID_PIN; }
// Init Pins:
    __initDataPins__();
// Init clear pin
    __initClearPin__();
// Init enable pin
    
    if (__initEnableGPIO__(enable) == false) { return ERROR_ENABLE_NOT_DEFINED; }
    _usePWM = false;
    return NO_ERROR;
}
/**
 * @brief initialize(uint8_t): Initialize the shift register.
 *      Initialize the shift register in PWM mode.
 * @param enable The value to set pwm to 255=always on, 0 = always off.
 * @return int16_t returns 0 (MY_NO_ERROR) for okay, and negative for error code.
 */
int16_t my595::initialize(const uint8_t enable) {
// Check pins:
    if (myHelpers::isPin(_latchPin) == false) { return MY_INVALID_PIN; }
    if (myHelpers::isPin(_dataPin) == false) { return MY_INVALID_PIN; }
    if (myHelpers::isPin(_clkPin) == false) { return MY_INVALID_PIN; }
// Init Pins:
    __initDataPins__();
// Init clear pin
    __initClearPin__();
// Init enable pin
    if (__initEnablePWM__(enable) == false) { return ERROR_ENABLE_NOT_DEFINED; }
    _usePWM = true;
    return NO_ERROR;
}
/**
 * @brief Start Write to the shift register.
 * 
 */
void my595::startWrite() {
    gpio_put(_latchPin, false); // lower the latch.
}
/**
 * @brief Clock in a single bit to the shift register.
 * 
 * @param value Value to clock in.
 */
void my595::writeBit(const bool value) {
    gpio_put(_dataPin, value); // Set data pin.
    gpio_put(_clkPin, true);
    gpio_put(_clkPin, false);
}
/**
 * @brief Write a byte to the shift register
 * 
 * @param value Byte to send to shift register
 * @param bitOrder True = MSB first, False = LSB first.
 */
void my595::writeByte(const uint8_t value, const bool bitOrder) {
    uint8_t val = value;    // copy the value so we can shift it 
    for (uint8_t i=0; i<8; i++) {
    // Write the bit:
        if (bitOrder == MSB_FIRST) {
            gpio_put(_dataPin, (bool)(val&0x80));
            val <<= 1;
        } else {
            gpio_put(_dataPin, (bool)(val&0x01));
            val >>= 1;
        }
        gpio_put(_clkPin, true);
        gpio_put(_clkPin, false);
    }
}
/**
 * @brief Stop the write and latch in data.
 * 
 */
void my595::stopWrite() {
    gpio_put(_latchPin, true);
}
/**
 * @brief Pulse the clear pin to clear the shift register.
 * 
 * @return int16_t 0=No error(MY_NO_ERROR), negative error code.
 */
int16_t my595::clear() {
    if (myHelpers::isPin(_clearPin) == false) { return ERROR_CLEAR_NOT_DEFINED; }
    gpio_put(_clearPin, false);
    gpio_put(_clearPin, true);
    return NO_ERROR;
}
/**
 * @brief setEnable(bool): Set the enable pin.
 * 
 * @param value True = output enabled, False = output disabled.
 * @return int16_t returns 0 (MY_NO_ERROR) for okay, negative for error code.
 */
int16_t my595::setEnable(const bool value) {
    if (myHelpers::isPin(_enablePin) == false) { return ERROR_ENABLE_NOT_DEFINED; }
    if (_usePWM == true) {
        if (value == true) {
            pwm_set_chan_level(_pwmSlice, _pwmChannel, 0); 
        } else {
            pwm_set_chan_level(_pwmSlice, _pwmChannel, 255);
        }
    } else {
        gpio_put(_enablePin, !value);
    }
    return NO_ERROR;
}
/**
 * @brief setEnable(uint8_t): Set the enable pin pwm value.
 * 
 * @param value Value to set pwm to (used for brightness).
 * @return int16_t Returns 0(MY_NO_ERROR) for okay, and negative for error code.
 */
int16_t my595::setEnable(const uint8_t value) {
    if (myHelpers::isPin(_enablePin) == false) { return ERROR_ENABLE_NOT_DEFINED; }
    if (_usePWM == true) {
        uint8_t val = 255-value;
        pwm_set_chan_level(_pwmSlice, _pwmChannel, val);
    } else {
        if (value > 127) {
            gpio_put(_enablePin, false);
        } else {
            gpio_put(_enablePin, true);
        }
    }
    return NO_ERROR;
}

void my595::__initDataPins__() {
    gpio_init(_latchPin);
    gpio_init(_clkPin);
    gpio_init(_dataPin);
    gpio_set_dir(_latchPin, GPIO_OUT);
    gpio_set_dir(_clkPin, GPIO_OUT);
    gpio_set_dir(_dataPin, GPIO_OUT);
    gpio_put(_latchPin, true);
    gpio_put(_clkPin, false);
    gpio_put(_dataPin, false);
}

void my595::__initClearPin__() {
    if (myHelpers::isPin(_clearPin) == true) {
        gpio_init(_clearPin);
        gpio_set_dir(_clearPin, GPIO_OUT);
        gpio_put(_clearPin, true);
    }
}

bool my595::__initEnableGPIO__(const bool value) {
    if (myHelpers::isPin(_enablePin) == true) {
        gpio_init(_enablePin);
        gpio_set_dir(_enablePin, GPIO_OUT);
        gpio_put(_enablePin, !value);
        return true;
    }
    return false;
}

bool my595::__initEnablePWM__(const uint8_t value) {
    uint8_t val = 255-value;
    if (myHelpers::isPin(_enablePin) == true) {
        gpio_set_function(_enablePin, GPIO_FUNC_PWM);
        _pwmSlice = pwm_gpio_to_slice_num(_enablePin);
        _pwmChannel = pwm_gpio_to_channel(_enablePin);
        pwm_set_wrap(_pwmSlice, 255);
        pwm_set_chan_level(_pwmSlice, _pwmChannel, val); 
        pwm_set_enabled(_pwmSlice, true);
        return true;
    }
    return false;
}
#endif