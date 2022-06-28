/**
 * @file mySNx4HC595.hpp
 * @author Peter Nearing (pnearing@protonmail.com)
 * @brief Class to use a 595 shift register.
 * @version 0.1
 * @date 2022-06-23
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef MY_SNX4HC595_H
#define MY_SNX4HC595_H

#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <hardware/spi.h>

#include "../myStandardDefines.hpp"
#include "../myErrorCodes.hpp"
#include "../myHelpers.hpp"
/**
 * @brief A Class to use a 595 shift register.
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
class my595 {
    public:
    /* Constants: */
        // Errors:
        /**
         * @brief No error. Value: 0
         */
        static const int16_t NO_ERROR = MY_NO_ERROR; 
        /**
         * @brief Enable pin not defined. Value: -400
         */
        static const int16_t ERROR_ENABLE_NOT_DEFINED = MY_ERROR_MY595_ENABLE_NOT_DEFINED;
        /**
         * @brief Clear pin not defined. Value: -401
         */
        static const int16_t ERROR_CLEAR_NOT_DEFINED = MY_ERROR_MY595_CLEAR_NOT_DEFINED;
        /**
         * @brief Operation not available. Value: -402
         * Write bit operation is not available when in hardware spi mode.
         */
        static const int16_t ERROR_OPERATION_NOT_AVAILABLE = MY_ERROR_MY595_OPERATION_NOT_AVAILABLE;

    /* Constructors: */
        /**
         * @brief Construct a new my595 object. Hardware spi.
         * 
         * @param spiPort HW Spi port
         * @param latch Latch pin number.
         * @param clock clock pin number.
         * @param data Data pin number.
         */
        my595(spi_inst_t *spiPort, const uint8_t latch, const uint8_t clock, const uint8_t miso, 
                    const uint8_t mosi) : _latchPin (latch), _clkPin (clock), _dataTXPin (mosi),
                    _dataRXPin (miso) {
            _spiPort = spiPort;
            _useHardware = true;
        }
        /**
         * @brief Construct a new my595 object. With enable, hardware spi mode.
         * 
         * @param spiPort Hardware SPI port.
         * @param latch Latch pin number.
         * @param clock Clock pin number.
         * @param data Data pin number.
         * @param enable Enable pin number.
         */
        my595(spi_inst_t *spiPort, const uint8_t latch, const uint8_t clock, const uint8_t miso, 
                    const uint8_t mosi, const uint8_t enable) : _latchPin (latch), _clkPin (clock),
                    _dataTXPin (mosi), _dataRXPin (miso) {
            _spiPort = spiPort;
            _useHardware = true;
        }
        /**
         * @brief Construct a new my595 object. With enable and clear. Hardware spi.
         * 
         * @param spiPort 
         * @param latch 
         * @param clock 
         * @param data 
         * @param enable 
         * @param clear 
         */
        my595(spi_inst_t *spiPort, const uint8_t latch, const uint8_t clock, const uint8_t miso,
                    const uint8_t mosi, const uint8_t enable, const uint8_t clear) : _latchPin (latch),
                    _clkPin (clock),_dataRXPin (miso), _dataTXPin (mosi), _enablePin (enable),
                    _clearPin(clear) {
            _spiPort = spiPort;
            _useHardware = true;
        }
        /**
         * @brief Construct a new my595 object. Bit banged mode.
         * 
         * @param latch Latch pin number.
         * @param clock Clock pin number.
         * @param data Data pin number.
         */
        my595(const uint8_t latch, const uint8_t clock, const uint8_t data) : _latchPin (latch),
                    _clkPin (clock), _dataTXPin(data) {
            _useHardware = false;
        }
        /**
         * @brief Construct a new my595 object with enable pin. Bit banged mode.
         * 
         * @param latch Latch pin number
         * @param clock Clock pin number.
         * @param data Data pin number.
         * @param enable Enable pin number.
         */
        my595(const uint8_t latch, const uint8_t clock, const uint8_t data, const uint8_t enable) :
                    _latchPin (latch), _clkPin (clock), _dataTXPin (data), _enablePin (enable) {
            _useHardware = false;
        }
        /**
         * @brief Construct a new my595 object with enable and clear pin. Bit banged mode.
         * @note enable pin can be set to MY_NOT_A_PIN, or any other invalid pin number to disable.
         * @param latch Latch pin number.
         * @param clock Clock pin number.
         * @param data Data pin number.
         * @param enable Enable pin number.
         * @param clear Clear pin number.
         */
        my595(const uint8_t latch, const uint8_t clock, const uint8_t data, const uint8_t enable,
                    const uint8_t clear) : _latchPin (latch), _clkPin (clock), _dataTXPin (data),
                    _enablePin (enable), _clearPin (clear) {
            _useHardware = false;
        }
    /* Functions: */
        int16_t initialize();
        int16_t initialize(const bool enable);
        int16_t initialize(const uint8_t enable);

        void    startWrite(); // Lowers the latch.
        int16_t writeBit(const bool value); // Shift in one bit.
        void    writeByte(const uint8_t value, const bool bitOrder=MSB_FIRST); // Shift in one byte.
        void    stopWrite(); // raise the latch.
        int16_t clear();
        int16_t setEnable(const bool value);
        int16_t setEnable(const uint8_t value);

    private:
    /* Variables: */
        spi_inst_t    *_spiPort;
        const uint8_t _latchPin = MY_NOT_A_PIN;
        const uint8_t _clkPin = MY_NOT_A_PIN;
        const uint8_t _dataTXPin = MY_NOT_A_PIN;
        const uint8_t _dataRXPin = MY_NOT_A_PIN;
        const uint8_t _enablePin = MY_NOT_A_PIN;
        const uint8_t _clearPin = MY_NOT_A_PIN;
        uint8_t       _pwmSlice;
        uint8_t       _pwmChannel;
        bool          _usePWM = false;
        bool          _useHardware = false;
    /* Functions: */
        void __initDataPinsGPIO__();
        void __initSPI__();
        void __initClearPin__();
        bool __initEnableGPIO__(const bool value);
        bool __initEnablePWM__(const uint8_t value);
        uint8_t __reverseByte__(const uint8_t value); // Switch a bit from msb to lsb first.
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
    if (myHelpers::isPin(_dataTXPin) == false) { return MY_INVALID_PIN; }
    if (myHelpers::isPin(_clkPin) == false) { return MY_INVALID_PIN; }
// Init Pins:
    if (_useHardware == false) {
        __initDataPinsGPIO__();
    } else {
        __initSPI__();
    }
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
    if (myHelpers::isPin(_dataTXPin) == false) { return MY_INVALID_PIN; }
    if (myHelpers::isPin(_clkPin) == false) { return MY_INVALID_PIN; }
// Init Pins:
    if (_useHardware == false) {
        __initDataPinsGPIO__();
    } else {
        __initSPI__();
    }
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
    if (myHelpers::isPin(_dataTXPin) == false) { return MY_INVALID_PIN; }
    if (myHelpers::isPin(_clkPin) == false) { return MY_INVALID_PIN; }
// Init Pins:
    if (_useHardware == false) {
        __initDataPinsGPIO__();
    } else {
        __initSPI__();
    }
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
 * @return int16_t Returns 0 for no error, negative is error code.
 */
int16_t my595::writeBit(const bool value) {
    if (_useHardware == true) { return ERROR_OPERATION_NOT_AVAILABLE; }
    gpio_put(_dataTXPin, value); // Set data pin.
    gpio_put(_clkPin, true);
    gpio_put(_clkPin, false);
    return NO_ERROR;
}
/**
 * @brief Write a byte to the shift register
 * 
 * @param value Byte to send to shift register
 * @param bitOrder True = MSB first, False = LSB first.
 */
void my595::writeByte(const uint8_t value, const bool bitOrder) {
    // __breakpoint();
    if (_useHardware == false) {
        uint8_t val = value;    // copy the value so we can shift it 
        for (uint8_t i=0; i<8; i++) {
        // Write the bit:
            if (bitOrder == MSB_FIRST) {
                gpio_put(_dataTXPin, (bool)(val&0x80));
                val <<= 1;
            } else {
                gpio_put(_dataTXPin, (bool)(val&0x01));
                val >>= 1;
            }
        // Toggle the clock:
            gpio_put(_clkPin, true);
            gpio_put(_clkPin, false);
        }
    } else {
        uint8_t val = value;
        if (bitOrder == LSB_FIRST) {
            val = __reverseByte__(value);
        }
        spi_write_blocking(_spiPort, &val, 1);
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

void my595::__initDataPinsGPIO__() {
    gpio_init(_latchPin);
    gpio_init(_clkPin);
    gpio_init(_dataTXPin);
    gpio_set_dir(_latchPin, GPIO_OUT);
    gpio_set_dir(_clkPin, GPIO_OUT);
    gpio_set_dir(_dataTXPin, GPIO_OUT);
    gpio_put(_latchPin, true);
    gpio_put(_clkPin, false);
    gpio_put(_dataTXPin, false);
}

void my595::__initSPI__() {
    spi_init(_spiPort, 1000*5000); // init spi at 5 Mhz.
    gpio_set_function(_clkPin,  GPIO_FUNC_SPI); // set sck as spi.
    gpio_set_function(_dataTXPin, GPIO_FUNC_SPI); // set data (mosi) as spi.
    gpio_set_function(_dataRXPin, GPIO_FUNC_SPI); // Set mosi as spi.
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

uint8_t my595::__reverseByte__(const uint8_t value) {
    uint8_t returnValue = 0x00;
    for (uint8_t i=0; i<8; i++) {
        returnValue <<= 1;
        if ((bool)(value & (1<<i)) == true) {
            returnValue |= 0x01;
        }
    }
    return returnValue;
}
#endif