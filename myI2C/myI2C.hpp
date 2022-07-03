/**
 * @file myI2c.hpp
 * @author Peter Nearing (pnearing@protonmail.com)
 * @brief I2C helper functions.
 * @version 0.1
 * @date 2022-06-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MY_I2C_H
#define MY_I2C_H

#include <hardware/gpio.h>
#include <hardware/i2c.h>

#include "../myStandardDefines.hpp"
#include "../myErrorCodes.hpp"
#include "../myHelpers.hpp"
/**
 * @brief I2C helper functions.
 * 
 */
namespace myI2C {
/*
 * ######################## Constants: #################################
 */
// Error codes:
    /**
     * @brief No error. Value 0.
     * No error occured.
     */
    static const int16_t NO_ERROR = MY_NO_ERROR;
    /**
     * @brief Invalid SDA pin. Value -450.
     * The provided SDA pin is invalid.
     */
    static const int16_t ERROR_INVALID_SDA_PIN = MY_ERROR_MYI2C_INVALID_SDA_PIN;
    /**
     * @brief Invalid SCL pin. Value -451.
     * The provided SCL pin is invalid.
     */
    static const int16_t ERROR_INVALID_SCL_PIN = MY_ERROR_MYI2C_INVALID_SCL_PIN;
    /**
     * @brief I2C already initialized. Value -452.
     * The I2C port was already initialized when initialize was called.
     */
    static const int16_t ERROR_INITIALIZED = MY_ERROR_MYI2C_INITIALIZED;
    /**
     * @brief I2C not initialized. Value -453.
     * The I2C port was already deinitialized when deinitialize was called.
     */
    static const int16_t ERROR_NOT_INITIALIZED = MY_ERROR_MYI2C_NOT_INITIALIZED;
// Pin Mappings:
    /** @private */
    static const uint8_t _sdaPins[MY_I2C_NUM_PORTS][MY_I2C_NUM_PINS] =
        {
            { 0, 4,  8, 12, 16, 20, 24, 28 },
            { 2, 6, 10, 14, 18, MY_NOT_A_PIN, MY_NOT_A_PIN, MY_NOT_A_PIN},
        };
    /** @private */
    static const uint8_t _sclPins[MY_I2C_NUM_PORTS][MY_I2C_NUM_PINS] =
        {
            { 1, 5,  9, 13, 17, 21, 25, 29 },
            { 3, 7, 11, 15, 19, MY_NOT_A_PIN, MY_NOT_A_PIN, MY_NOT_A_PIN },
        };
// Pin index's:
    /** @private */
    static const uint8_t _PIN_SDA = 0;
    /** @private */
    static const uint8_t _PIN_SCL = 1;
/*
 * ########################### Variables: #################################
 */
    /** @private */
    uint8_t _status = 0b00001100; // State of the i2c ports
    uint8_t _pins[MY_I2C_NUM_PORTS][2] = { {MY_NOT_A_PIN, MY_NOT_A_PIN}, {MY_NOT_A_PIN, MY_NOT_A_PIN} };
/*
 * #################### Validation Functions: #############################
 */

    /**
     * @brief Validate an I2C address.
     * Validates an I2C address, returns true if an address is valid, false if not valid, or is a 
     * reserved address.
     * @param value I2C address to check.
     * @return true Address is valid.
     * @return false Address is not valid or is reserved.
     */
    bool isValidAddress(const uint8_t value) {
        if ((value & 0x80) > 0) { return false; } // Bit 7 set, invalid.
        if (value < 0x08) { return false; } // reserved addresses.
        if (value > 0x78) { return false; } // reserved addresses.
        return true;
    }   
    /**
     * @brief Validate SDA pin.
     * Returns true if pin provided is a valid SDA pin for the provided i2c port, otherwise
     * if an invalid pin is passed, or its not a SDA pin, it will return false.
     * @param i2cPort I2C port instance.
     * @param pin Pin to validate.
     * @return true Pin is valid SDA pin.
     * @return false Pin is not valid SDA pin, or not a valid pin.
     */
    bool isSdaPin(i2c_inst_t *i2cPort, const uint8_t pin) {
        if (myHelpers::isPin(pin) == false) { return false; }
        const uint8_t i2cIndex = i2c_hw_index(i2cPort);
        for (uint8_t i=0; i<MY_I2C_NUM_PINS; i++) {
            if (pin == _sdaPins[i2cIndex][i]) { return true; }
        }
        return false;
    }
    /**
     * @brief Validate SCL pin.
     * Returns true if pin provided is a valid SCL pin for the I2C port. Otherwise if
     * the pin is not a SCL pin, or not a valid pin, it will return false.
     * @param i2cPort I2C port instance.
     * @param pin Pin to validate.
     * @return true Pin is a valid SCL pin.
     * @return false Pin is not a valid SCL pin, or not a valid pin.
     */
    bool isSclPin(i2c_inst_t *i2cPort, const uint8_t pin) {
        if (myHelpers::isPin(pin) == false) { return false; }
        const uint8_t i2cIndex = i2c_hw_index(i2cPort);
        for (uint8_t i=0; i<MY_I2C_NUM_PINS; i++) {
            if (pin == _sclPins[i2cIndex][i]) { return true; }
        }
        return false;
    }
/*
 * ######################## Public status functions: ##############################
 */
    /**
     * @brief Get if the I2C port has been initialized.
     * Returns true if I2C port provided is initialized, false if not.
     * @param i2cPort I2C port instance to check.
     * @return true Port is initialized.
     * @return false Port is not initialized.
     */
    bool getInitialized(i2c_inst_t *i2cPort) {
        uint8_t i2cIndex = i2c_hw_index(i2cPort);
        return (bool) (_status & (1 << i2cIndex));
    }
    /**
     * @brief Get if master.
     * Return true if I2C port is initialized as a master.
     * @param i2cPort I2C port instance to check.
     * @return true I2C is in master mode.
     * @return false I2C is in slave mode.
     */
    bool getMaster(i2c_inst_t *i2cPort) {
        uint8_t i2cIndex = i2c_hw_index(i2cPort);
        return (bool) (_status & (1 << (i2cIndex + 2)));
    }
    /**
     * @brief Get the SDA pin. 
     * Returns the SDA pin set for the given I2C port. Returns 255 (MY_NOT_A_PIN) if not set.
     * @param i2cPort I2C port instance.
     * @return uint8_t SDA pin.
     */
    uint8_t getSdaPin(i2c_inst_t *i2cPort) {
        const uint8_t i2cIndex = i2c_hw_index(i2cPort);
        return _pins[i2cIndex][_PIN_SDA];
    }
    /**
     * @brief Get the SCL pin.
     * Returns the SCL pin set for the given I2C port. Returns 255 (MY_NOT_A_PIN) if not set.
     * @param i2cPort I2C port instance.
     * @return uint8_t SCL pin.
     */
    uint8_t getSclPin(i2c_inst_t *i2cPort) {
        const uint8_t i2cIndex = i2c_hw_index(i2cPort);
        return _pins[i2cIndex][_PIN_SCL];
    }
/*
 * ######################### Private status functions: #########################
 */
    /** @private */
    void __setInit__(i2c_inst_t *i2cPort, const bool value) {
        const uint8_t i2cIndex = i2c_hw_index(i2cPort);
        if (value == true) {
            _status |= (1 << i2cIndex);
        } else {
            _status &= ~(1 << i2cIndex);
        }
    }
    /** @private */
    void __setMaster__(i2c_inst_t *i2cPort, const bool value, const uint8_t address=0x00) {
        const uint8_t i2cIndex = i2c_hw_index(i2cPort);
        if (value == true) {
            i2c_set_slave_mode(i2cPort, false, address);
            _status |= (1 << (i2cIndex + 2));
        } else {
            i2c_set_slave_mode(i2cPort, true, address);
            _status &= ~(1 << (i2cIndex + 2));
        }
    }
    /** @private */
    void __setPins__(i2c_inst_t *i2cPort, const uint8_t sdaPin, const uint8_t sclPin){
        const uint8_t i2cIndex = i2c_hw_index(i2cPort);
        _pins[i2cIndex][_PIN_SDA] = sdaPin;
        _pins[i2cIndex][_PIN_SCL] = sclPin;
    }
/*
 * ########################### Initialize Functions: #################################
 */
    /**
     * @brief Initialize I2C in master mode.
     * Intialize I2C in master mode, returns the achieved baud rate in Hz, if successful,
     * otherwise if an invalid pin is passed, or port is already initialized an error code is returned.
     * @param i2cPort I2C port instance to initialize.
     * @param sdaPin SDA pin to set.
     * @param sclPin SCL pin to set.
     * @param baud Requested baud in Hz.
     * @return int32_t Positive Achieved baud rate in Hz, negative for error code.
     */
    int32_t initializeMaster(i2c_inst_t *i2cPort, const uint8_t sdaPin, const uint8_t sclPin,
                                    const uint32_t baud) {
        if (getInitialized(i2cPort) == true) { return ERROR_INITIALIZED; }
        if (isSdaPin(i2cPort, sdaPin) == false) { return ERROR_INVALID_SDA_PIN; }
        if (isSclPin(i2cPort, sclPin) == false) { return ERROR_INVALID_SCL_PIN; }
        int32_t acheivedRate = i2c_init(i2cPort, baud);
        gpio_set_function(sdaPin, GPIO_FUNC_I2C);
        gpio_set_function(sclPin, GPIO_FUNC_I2C);
        gpio_pull_up(sdaPin);
        gpio_pull_up(sclPin);
        __setPins__(i2cPort, sdaPin, sclPin);
        __setMaster__(i2cPort, true);
        return acheivedRate;
    }
    /**
     * @brief Initialize I2C in slave mode.
     * Initialize I2C in slave mode, returns the achieved baud rate in Hz if successful. Otherwise if
     * an invalid pin is passed, or port is alreay initialized an error code is returned.
     * @param i2cPort I2C port instance to initialize.
     * @param sdaPin SDA pin.
     * @param sclPin SCL pin.
     * @param baud Requested baud rate.
     * @param address I2C address to respond to.
     * @return int32_t Postive is the achieved baud rate, negative for an error code.
     */
    int32_t initializeSlave(i2c_inst_t *i2cPort, const uint8_t sdaPin, const uint8_t sclPin,
                                    const uint8_t baud, const uint8_t address) {
        if (getInitialized(i2cPort) == true) { return ERROR_INITIALIZED; }
        if (isSdaPin(i2cPort, sdaPin) == false) { return ERROR_INVALID_SDA_PIN; }
        if (isSclPin(i2cPort, sclPin) == false) { return ERROR_INVALID_SCL_PIN; }
        int32_t acheivedRate = i2c_init(i2cPort, baud);
        gpio_set_function(sdaPin, GPIO_FUNC_I2C);
        gpio_set_function(sclPin, GPIO_FUNC_I2C);
        gpio_pull_up(sdaPin);
        gpio_pull_up(sclPin);
        __setPins__(i2cPort, sdaPin, sclPin);
        __setMaster__(i2cPort, false, address);
        return acheivedRate;
    }
    /**
     * @brief Deinitialize an I2C port.
     * Deinitializes an I2C port, returns 0 (NO_ERROR) if deinitialized okay, otherwise
     * if it's already deinitialized, an error code will be returned.
     * @param i2cPort I2C port instance to deinitialize.
     * @return int32_t Returns 0 (NO_ERROR) if deinitialized okay, negative for error code.
     */
    int32_t deinitialize(i2c_inst_t *i2cPort) {
        if (getInitialized(i2cPort) == true) { return ERROR_NOT_INITIALIZED; }
        uint8_t i2cIndex = i2c_hw_index(i2cPort);
        i2c_deinit(i2cPort);
        __setInit__(i2cPort, false);
        gpio_set_function(_pins[i2cIndex][_PIN_SDA], GPIO_FUNC_NULL);
        gpio_set_function(_pins[i2cIndex][_PIN_SCL], GPIO_FUNC_NULL);
        gpio_set_pulls(_pins[i2cIndex][_PIN_SDA], false, false);
        gpio_set_pulls(_pins[i2cIndex][_PIN_SCL], false, false);
        __setPins__(i2cPort, MY_NOT_A_PIN, MY_NOT_A_PIN);
        return NO_ERROR;
    }


};
#endif