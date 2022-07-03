/**
 * @file myAT24C32.hpp
 * @author Peter Nearing (pnearing@protonmail.com)
 * @brief Class to use an AT24C32 32K EEPROM.
 * @version 0.1
 * @date 2022-07-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MY_AT24C32_H
#define MY_AT24C32_H

#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <pico/error.h>

#include "../myStandardDefines.hpp"
#include "../myErrorCodes.hpp"
#include "../myHelpers.hpp"
#include "../myI2C/myI2C.hpp"

class myAT24C32 {

    public:
/******************** Public Constants: *******************/
    //EEPROM constants:
        /**
         * @brief Length of EEPROM in bytes.
         */
        static const uint16_t EEPROM_LENGTH = 4096;
        /**
         * @brief Length of EEPROM Page in bytes.
         */
        static const uint8_t PAGE_LENGTH = 32;
        /**
         * @brief Number of 32 byte pages.
         */
        static const uint8_t NUM_PAGES = 128;
        /**
         * @brief Max address.
         */
        static const uint16_t MAX_ADDRESS = 0x0FFF;
    // Error codes:
        /**
         * @brief No error. Value 0.
         * No error occured.
         */
        static const int16_t NO_ERROR = MY_NO_ERROR;
        /**
         * @brief Timeout reached. Value -7.
         * The EEPROM timed out.
         */
        static const int16_t ERROR_TIMEOUT = MY_TIMEOUT;
        /**
         * @brief Invalid SDA pin. Value -450.
         * Invalid SDA pin was passed.
         */
        static const int16_t ERROR_INVALID_SDA_PIN = MY_ERROR_MYI2C_INVALID_SDA_PIN;
        /**
         * @brief Invalid SCL pin. Value -451.
         * Invalid SCL pin was passed.
         */
        static const int16_t ERROR_INVALID_SCL_PIN = MY_ERROR_MYI2C_INVALID_SCL_PIN;
        /**
         * @brief I2C bus already initialized. Value -452.
         * The I2C bus is already initialized.
         */
        static const int16_t ERROR_INITIALIZED = MY_ERROR_MYI2C_INITIALIZED;
        /**
         * @brief I2C Address not responding. Value -454.
         * The supplied I2C address is not responding.
         */
        static const int16_t ERROR_NOT_RESPONDING = MY_ERROR_MYI2C_ADDRESS_NOT_RESPONDING;
        /**
         * @brief Invalid address. Value -600.
         * The address supplied is invalid.
         */
        static const int16_t ERROR_INVALID_ADDRESS = MY_ERROR_MYAT24C32_INVALID_ADDRESS;
        /**
         * @brief Invalid length. Value -601.
         * The length provided is too long.
         */
        static const int16_t ERROR_INVALID_LENGTH = MY_ERROR_MYAT24C32_INVALID_LENGTH;
/****************** Constructor: **************************/
        /**
         * @brief Construct a new my AT24C32 object.
         * 
         * @param spiPort HW spi port instance to use.
         * @param sdaPin SDA Pin.
         * @param sclPin SCL Pin.
         * @param address I2C address of the EEPROM.
         */
        myAT24C32(i2c_inst_t *i2cPort, const uint8_t sdaPin, const uint8_t sclPin, const uint8_t address) :
                        _sdaPin(sdaPin), _sclPin(sclPin), _address(address) {
            _i2cPort = i2cPort;
        }
/****************** Public Funtions *************************/
        
        /**
         * @brief Validate an address.
         * Validates an address, returns true if address is valid, false if not.
         * @param address Address to validate.
         * @return true Address is valid.
         * @return false Address is not valid.
         */
        bool isValidAddress(const uint16_t address);
        /**
         * @brief Read from the EERPOM
         * Read from the EEPROM, returns 0 (NO_ERROR) if read okay, otherwise if a comms error
         * occurs, an error code is returned.
         * @param address Address to start read from.
         * @param buffer Buffer to read into.
         * @param length Number of bytes to read.
         * @return int16_t Returns 0 (NO_ERROR) if read okay, negative for error code.
         */
        int16_t read(const uint16_t address, uint8_t *buffer, size_t length);
        /**
         * @brief Write to the EEPROM
         * Preforms a write to the EEPROM. Returns 0 (NO_ERROR) if written okay, otherwise if an invalid
         * address or length are provided, an error code is returned. This blocks until the write is
         * complete.
         * @note The EEPROM writes in 32 byte pages only, the data is therefore split up into 32 byte chunks
         * and written. The address counter does not roll over to the first address of the first page on write,
         * so the length of the write can only be to the end of the EEPROM.
         * @param address Address to start write at.
         * @param buffer Data buffer to write.
         * @param length Number of bytes to write. Valid values are determined by the space between the address and the end of the EEPROM.
         * @return int16_t Returns 0 (NO_ERROR) if written okay, negative for error code.
         */
        int16_t write(const uint16_t address, uint8_t *buffer, const size_t length);
        /**
         * @brief Initialize the EEPROM.
         * Initializes the eeprom, and preforms a read to verify comms. Returns 0 (NO_ERROR) if 
         * initialized okay, otherwise it will return an error code.
         * @param initI2C True = Initialize I2C bus (default). False = Don't initialize I2C bus, Assumes i2c bus already setup.
         * @return int16_t Returns 0 (NO_ERROR) for init okay. Negative for error code.
         */
        int16_t initialize(const bool initI2C=true);
    private:
        
/***************** Private constants: ***********************/
    // I2C:
        const uint16_t I2C_TIMEOUT = 50000; // 50 milliseconds.
    // Pins:
        const uint8_t _sdaPin = MY_NOT_A_PIN;
        const uint8_t _sclPin = MY_NOT_A_PIN;
        const uint8_t _address = 0x00;
/***************** Private Variables: ***********************/
        i2c_inst_t *_i2cPort;
/***************** Private Functions: **********************/
        int16_t __read__(const uint16_t address, uint8_t *buffer, const size_t length);
        int16_t __writeByte__(const uint16_t address, uint8_t value);
        int16_t __write__(const uint16_t address, uint8_t *buffer, const size_t length);
};

/****************** Public functions: ************************/
bool myAT24C32::isValidAddress(const uint16_t address) {
    if (address > MAX_ADDRESS) { return false; }
    return true;
}

int16_t myAT24C32::read(const uint16_t address, uint8_t *buffer, const size_t length) {
    int16_t returnValue;
    if (isValidAddress(address) == false) { return ERROR_INVALID_ADDRESS; }
    if (length < 1 or length > EEPROM_LENGTH) { return ERROR_INVALID_LENGTH; }
    returnValue = __read__(address, buffer, length);
    return returnValue;
}

int16_t myAT24C32::write(const uint16_t address, uint8_t *buffer, const size_t length) {
    int16_t returnValue;
    size_t maxLen;
    uint8_t numChunks;
    uint8_t numRemaining;
    int16_t writeAddress;
    int16_t offset;
    uint8_t dataBuffer[32];
    if (isValidAddress(address) == false) { return ERROR_INVALID_ADDRESS; }
    maxLen = (MAX_ADDRESS - address) + 1;
    if (length < 1 or length > maxLen) { return ERROR_INVALID_LENGTH; }
    if (length <= PAGE_LENGTH) {
        returnValue = __write__(address, buffer, length);
        return returnValue;
    }
 
    numChunks = length / PAGE_LENGTH;
    numRemaining = length - (numChunks * PAGE_LENGTH);
    for (uint8_t i=0; i<numChunks; i++) {
        offset = i * 32;
        writeAddress = address + offset;
        for (uint16_t j=0; j<32; j++) {
            uint16_t index = offset + j;
            // printf("i=%i, offset=%i, j=%i index=%i\n", i, offset, j, index);
            dataBuffer[j] = buffer[index];
        }
        returnValue = __write__(writeAddress, dataBuffer, 32);
        if (returnValue < 0) { return returnValue; }
        sleep_ms(10);
    }
    if (numRemaining > 0) {
        offset = numChunks * 32;
        writeAddress = address + offset;
        for (uint8_t i=0; i<numRemaining; i++) {
            dataBuffer[i] = buffer[offset + i];
        }
        returnValue = __write__(writeAddress, dataBuffer, numRemaining);
        return returnValue;
    }
    // printf("Num chunks=%i\n", numChunks);
    // printf("Num remaining=%i\n", numRemaining);
    return NO_ERROR;
}

int16_t myAT24C32::initialize(const bool initI2C) {
    int32_t returnValue;
    uint8_t value;
// Initialize i2c:
    if (initI2C == true) {
        returnValue = myI2C::initializeMaster(_i2cPort, _sdaPin, _sclPin, 100*1000);
        if (returnValue < 0) { return (int16_t)returnValue; }
    }
// Verify I2C address:

// Read address 0 to verify comms:
    returnValue = __read__(0x0000, &value, 1);
    // printf("Got value: %02x\n", value);
    return (int16_t)returnValue;
}

/****************** Private functions: *********************/

int16_t myAT24C32::__read__(const uint16_t address, uint8_t *buffer, const size_t length) {
    int32_t returnValue;
    uint8_t addressBuffer[2];
    // preform "dummy write" to set address.
    addressBuffer[0] = (uint8_t)(address >> 8);
    addressBuffer[1] = (uint8_t)address;
    returnValue = i2c_write_timeout_us(_i2cPort, _address, addressBuffer, 2, true, I2C_TIMEOUT);
    if (returnValue < 0) {
        if (returnValue == PICO_ERROR_GENERIC) { return ERROR_NOT_RESPONDING; }
        if (returnValue == PICO_ERROR_TIMEOUT) { return ERROR_TIMEOUT; }
    }
    returnValue = i2c_read_timeout_us(_i2cPort, _address, buffer, length, false, I2C_TIMEOUT);
    if (returnValue < 0) {
        if (returnValue == PICO_ERROR_GENERIC) { return ERROR_NOT_RESPONDING; }
        if (returnValue == PICO_ERROR_TIMEOUT) { return ERROR_TIMEOUT; }
    }
    return NO_ERROR;
}

int16_t myAT24C32::__write__(const uint16_t address, uint8_t *buffer, const size_t length) {
    int32_t returnValue;
    uint8_t dataBuffer[length+2];
    // puts("__write__");
// Create the buffer:
    dataBuffer[0] = (uint8_t)(address>>8);
    dataBuffer[1] = (uint8_t)address;
    for (size_t i=0; i<length; i++) {
        dataBuffer[i+2] = buffer[i];
    }
    sleep_ms(10);
    returnValue = i2c_write_timeout_us(_i2cPort, _address, dataBuffer, length+2, false, I2C_TIMEOUT);
    if (returnValue < 0) {
        if (returnValue == PICO_ERROR_GENERIC) { return ERROR_NOT_RESPONDING; }
        if (returnValue == PICO_ERROR_TIMEOUT) { return ERROR_TIMEOUT; }
    }
    sleep_ms(10); // Sleep to let the write complete.
    return NO_ERROR;
}
#endif