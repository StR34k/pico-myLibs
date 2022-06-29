/**
 * @file mySPI.hpp
 * @author Peter Nearing(pnearing@protonmail.com)
 * @brief SPI Helper functions.
 * @version 0.1
 * @date 2022-06-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */    
#ifndef MY_SPI_H
#define MY_SPI_H
#include <hardware/spi.h>
#include "../myStandardDefines.hpp"
#include "../myErrorCodes.hpp"
#include "../myHelpers.hpp"

namespace mySPI {
/*
 * #####################################Constants: ##############################
 */
// Pin mappings:
    /** @private */
    static const uint8_t _misoPins[MY_SPI_NUM_PORTS][MY_SPI_NUM_PINS] = { {0, 4, 16, 20}, {8, 12, 24, 28} };
    /** @private */
    static const uint8_t _mosiPins[MY_SPI_NUM_PORTS][MY_SPI_NUM_PINS] = { {3, 7, 19, 23}, {11, 15, 27, MY_NOT_A_PIN} };
    /** @private */
    static const uint8_t _sckPins[MY_SPI_NUM_PORTS][MY_SPI_NUM_PINS] = { {2, 6, 18, 22}, {10, 14, 26, MY_NOT_A_PIN} };
    /** @private */
    static const uint8_t _csPins[MY_SPI_NUM_PORTS][MY_SPI_NUM_PINS] = { {1, 5, 17, 21}, {9, 13, 25, 29} };
// Error codes:
    /**
     * @brief No error. Value 0.
     * No error occurred during operation.
     */
    static const int16_t NO_ERROR = MY_NO_ERROR;
    /**
     * @brief Invalid clock pin. Value -150.
     * The provided clock pin is not a valid clock pin for the spi port.
     */
    static const int16_t ERROR_INVALID_CLOCK_PIN = MY_SPI_INVALID_CLOCK_PIN;
    /**
     * @brief Invalid MISO pin. Value -151.
     * The provided MISO pin is not a valid MISO pin for the spi port.
     */
    static const int16_t ERROR_INVALID_MISO_PIN = MY_SPI_INVALID_MISO_PIN;
    /**
     * @brief Invalid MOSI pin. Value -152.
     * The provided MOSI pin is not a valid MOSI pin for the spi port.
     */
    static const int16_t ERROR_INVALID_MOSI_PIN = MY_SPI_INVALID_MOSI_PIN;
    /**
     * @brief Invalid chip select pin. Value -153.
     * The provided chip select pin is not a valid chip select pin for the spi port.
     */
    static const int16_t ERROR_INVALID_CS_PIN = MY_SPI_INVALID_CS_PIN;
    /**
     * @brief SPI port initialized. Value -154.
     * The SPI port was already initalized when init called.
     */
    static const int16_t ERROR_INITIALIZED = MY_SPI_INITIALIZED;
    /**
     * @brief SPI port no initialized. Value -155.
     * The SPI port is not initialized when de init called.
     */
    static const int16_t ERROR_NOT_INITIALIZED = MY_SPI_NOT_INITIALIZED;
// Pin type ids:
    /** @private */
    static const uint8_t _PIN_CLOCK = 0;
    /** @private */
    static const uint8_t _PIN_MISO = 1;
    /** @private */
    static const uint8_t _PIN_MOSI = 2;
    /** @private */
    static const uint8_t _PIN_CS = 3;
/* 
 * ######################### Variables: ############################
 */
    /** @private */
    uint8_t _portPins[MY_SPI_NUM_PORTS][4] = {                      // Store the pin used for deinit.
        { MY_NOT_A_PIN, MY_NOT_A_PIN, MY_NOT_A_PIN, MY_NOT_A_PIN },
        { MY_NOT_A_PIN, MY_NOT_A_PIN, MY_NOT_A_PIN, MY_NOT_A_PIN }
    };
    /** @private */
    uint8_t _status = 0b00001100;
/*
 * ########################### Validation functions: ##################
 */
    /**
     * @brief Validate a clock pin for given spi port.
     * Returns true if the supplied pin is a clock pin for the spi port, and false if not, or the
     * pin provided is invalid.
     * @param spiPort spi_inst_t Spi port instance.
     * @param pin Pin number to verify.
     * @return true Pin is a valid clock pin.
     * @return false Pin is not a valid clock pin, or is not a vaild pin.
     */
    bool isClockPin(spi_inst_t *spiPort, const uint8_t pin) {
        if (myHelpers::isPin(pin) == false) { return false; }
        const uint8_t spiIndex = spi_get_index(spiPort);
        for (uint8_t i=0; i<MY_SPI_NUM_PINS; i++) {
            if (pin == _sckPins[spiIndex][i]) { return true; }
        }
        return false;
    }
    /**
     * @brief Is a MISO pin.
     * Returns ture if pin is a valid MISO pin for the provided spi port. Returns false if not,
     * or the pin is invalid.
     * @param spiPort spi_inst_t Spi port instance.
     * @param pin Pin to verify.
     * @return true Pin is a valid MISO pin.
     * @return false Pin is not a valid MISO pin, or the pin is invalid.
     */
    bool isMisoPin(spi_inst_t *spiPort, const uint8_t pin) {
        if (myHelpers::isPin(pin) == false) { return false; }
        const uint8_t spiIndex = spi_get_index(spiPort);
        for (uint8_t i=0; i<MY_SPI_NUM_PINS; i++) {
            if (pin == _misoPins[spiIndex][i]) { return true; }
        }
        return false;
    }
    /**
     * @brief Is a MOSI pin.
     * Returns true if pin is a MOSI pin for the provided spi port. Returns false if not, 
     * or the pin is invalid.
     * @param spiPort spi_inst_t Spi port instance.
     * @param pin Pin to verify.
     * @return true Pin is a valid MOSI pin.
     * @return false Pin is not a valid MOSI pin, or the pin is invalid.
     */
    bool isMosiPin(spi_inst_t *spiPort, const uint8_t pin) {
        if (myHelpers::isPin(pin) == false) { return false; }
        const uint8_t spiIndex = spi_get_index(spiPort);
        for (uint8_t i=0; i<MY_SPI_NUM_PINS; i++) {
            if (pin == _mosiPins[spiIndex][i]) { return true; }
        }
        return false;
    }
    /**
     * @brief Is a chip select pin.
     * Returns true if pin is a valid chip select pin for the provided spi port. Returns false
     * if not, or pin is invalid.
     * @param spiPort spi_inst_t Spi port instance.
     * @param pin Pin to verify.
     * @return true Pin is a valid chip select pin.
     * @return false Pin is not a valid chip select pin, or is invalid.
     */
    bool isChipSelectPin(spi_inst_t *spiPort, const uint8_t pin) {
        if (myHelpers::isPin(pin) == false) { return false; }
        const uint8_t spiIndex = spi_get_index(spiPort);
        for (uint8_t i=0; i<MY_SPI_NUM_PINS; i++) {
            if (pin == _csPins[spiIndex][i]) { return true; }
        }
        return false;
    }
/*
 *######################### Public status functions: #######################
 */
    /**
     * @brief Get spi port initialized.
     * Returns true if the specified SPI port is initialized. False if not.
     * @param spiPort spi_inst_t Spi port instance.
     * @return true SPI port is initialized.
     * @return false SPI port is not initialized.
     */
    bool getPortInit(spi_inst_t *spiPort) {
        uint8_t spiIndex = spi_get_index(spiPort);
        return (bool)(_status & (1 << spiIndex));
    }
    /**
     * @brief Get the clock pin for given SPI port.
     * Returns the pin that was set as the clock pin. Returns 255 (MY_NOT_A_PIN) if the pin wasn't set.
     * @param spiPort SPI port instance.
     * @return uint8_t Returns pin number assigned to clock pin, or 255 (MY_NOT_A_PIN) if no pin set.
     */
    uint8_t getClockPin(spi_inst_t *spiPort) {
        uint8_t spiIndex = spi_get_index(spiPort);
        return _portPins[spiIndex][_PIN_CLOCK];
    }
    /**
     * @brief Get the MISO pin for the given SPI port.
     * Returns the pin number that was set as MISO pin. Returns 255(MY_NOT_A_PIN) if no pin set.
     * @param spiPort SPI port instance.
     * @return uint8_t Returns the pin number assigned as MISO. Returns 255 (MY_NOT_A_PIN) if not set.
     */
    uint8_t getMisoPin(spi_inst_t *spiPort) {
        uint8_t spiIndex = spi_get_index(spiPort);
        return _portPins[spiIndex][_PIN_MISO];
    }
    /**
     * @brief Get the MOSI pin for the given SPI port.
     * Returns the pin number that was set as MOSI. Returns 255 (MY_NOT_A_PIN) if not set.
     * @param spiPort SPI port Instance.
     * @return uint8_t Returns the pin number assigned as MOSI. Returns 255 (MY_NOT_A_PIN) if not set.
     */
    uint8_t getMosiPin(spi_inst_t *spiPort) {
        uint8_t spiIndex = spi_get_index(spiPort);
        return _portPins[spiIndex][_PIN_MOSI];
    }
    /**
     * @brief Get the chip select pin for the given SPI port.
     * Returns the pin number that was set as chip select. Returns 255 (MY_NOT_A_PIN) if not set.
     * @param spiPort SPI port Instance.
     * @return uint8_t Returns the pin number assigned as chip select. Returns 255 (MY_NOT_A_PIN) if not set.
     */
    uint8_t getChipSelectPin(spi_inst_t *spiPort) {
        uint8_t spiIndex = spi_get_index(spiPort);
        return _portPins[spiIndex][_PIN_CS];
    }
    /**
     * @brief Check if spi port is initialized as master.
     * Returns true if spi is initialized as a master.a
     * @param spiPort SPI port instance to check.
     * @return true SPI port is initialized as master.
     * @return false SPI port is initialized as slave.
     */
    bool isMaster(spi_inst_t *spiPort) {
        uint8_t spiIndex = spi_get_index(spiPort);
        return (bool)(_status & (1 << (spiIndex + 2)));
    }
/*
 *######################### Private status functions: #######################
 */
    /** @private */
    void __setPortInit__(spi_inst_t *spiPort, const bool value) {
        uint8_t spiIndex = spi_get_index(spiPort);
        if (value == true) {
            _status |= (1<<spiIndex);
        } else {
            _status &= ~(1<<spiIndex);
        }
    }
    /** @private */
    void __setMaster__(spi_inst_t *spiPort, const bool value) {
        const uint8_t spiIndex = spi_get_index(spiPort);
        if (value == true) {
            spi_set_slave(spiPort, false);
            _status |= (1 << (spiIndex + 2));
        } else {
            spi_set_slave(spiPort, true);
            _status &= ~(1 << (spiIndex + 2));
        }
    }
    /** @private */
    void __setPins__(spi_inst_t *spiPort, const uint8_t clockPin, const uint8_t misoPin,
                        const uint8_t mosiPin, const uint8_t csPin) {
        const uint8_t spiIndex = spi_get_index(spiPort);
        _portPins[spiIndex][_PIN_CLOCK] = clockPin;
        _portPins[spiIndex][_PIN_MISO] = misoPin;
        _portPins[spiIndex][_PIN_MOSI] = mosiPin;
        _portPins[spiIndex][_PIN_CS] = csPin;
    }
/*
 * ########################## Initialize functions: ##########################
 */
    /**
     * @brief Initialize SPI port in master mode.
     * Initialzed a SPI port in master mode. If an invalid pin is passed, an error is returned.
     * @param spiPort SPI port instance to initialize.
     * @param clockPin Clock pin.
     * @param misoPin MISO pin.
     * @param mosiPin MOSI pin.
     * @param baudRate Requested baud rate in Hz
     * @return int32_t Returns positive, for achieved baud rate, negative for error code.
     */
    int32_t initializeMaster(spi_inst_t *spiPort, const uint8_t clockPin, const uint8_t misoPin,
                                const uint8_t mosiPin, const uint32_t baudRate) {
        if (getPortInit(spiPort) == true) { return ERROR_INITIALIZED; }
        if (isClockPin(spiPort, clockPin) == false) { return ERROR_INVALID_CLOCK_PIN; }
        if (isMisoPin(spiPort, misoPin) == false) { return ERROR_INVALID_MISO_PIN; }
        if (isMosiPin(spiPort, mosiPin) == false) { return ERROR_INVALID_MOSI_PIN; }
        const int32_t achievedRate = spi_init(spiPort, baudRate);
        gpio_set_function(clockPin, GPIO_FUNC_SPI);
        gpio_set_function(misoPin, GPIO_FUNC_SPI);
        gpio_set_function(mosiPin, GPIO_FUNC_SPI);
        __setPins__(spiPort, clockPin, misoPin, mosiPin, MY_NOT_A_PIN);
        __setMaster__(spiPort, true);
        return achievedRate;
    }
    /**
     * @brief Initialize a SPI port in slave mode.
     * Initialize a SPI port in slave mode. Returns positive for achieved baud rate. Otherwise if
     * an invalid pin, or if the port is already initialized, an error code will be returned.
     * @param spiPort SPI port instance to initialize.
     * @param clockPin Clock pin.
     * @param misoPin MISO pin.
     * @param mosiPin MOSI pin.
     * @param chipSelectPin Chip select pin.
     * @param baudRate Requested baud rate in Hz.
     * @return int32_t Returns positive for baud rate achieved, or negative for error code.
     */
    int32_t initializeSlave(spi_inst_t *spiPort, const uint8_t clockPin, const uint8_t misoPin,
                                const uint8_t mosiPin, const uint8_t chipSelectPin, 
                                const uint32_t baudRate) {
        if (getPortInit(spiPort) == true) { return ERROR_INITIALIZED; }
        if (isClockPin(spiPort, clockPin) == false) { return ERROR_INVALID_CLOCK_PIN; }
        if (isMisoPin(spiPort, misoPin) == false) { return ERROR_INVALID_MISO_PIN; }
        if (isMosiPin(spiPort, mosiPin) == false) { return ERROR_INVALID_MOSI_PIN; }
        if (isChipSelectPin(spiPort, chipSelectPin) == false) { return ERROR_INVALID_CS_PIN; }
        const int32_t achievedRate = spi_init(spiPort, baudRate);
        gpio_set_function(clockPin, GPIO_FUNC_SPI);
        gpio_set_function(misoPin, GPIO_FUNC_SPI);
        gpio_set_function(mosiPin, GPIO_FUNC_SPI);
        gpio_set_function(chipSelectPin, GPIO_FUNC_SPI);
        __setPins__(spiPort, clockPin, misoPin, mosiPin, chipSelectPin);
        __setMaster__(spiPort, false);
        return achievedRate;
    }
    /**
     * @brief Deintialize SPI port.
     * Deinitializes a given SPI port. Returns an error code if not initialized.
     * @param spiPort SPI port instance to deinitialize.
     * @return int16_t Returns 0 (NO_ERROR) if deinit okay, negative for error code.
     */
    int16_t deinitialize(spi_inst_t *spiPort) {
        if (getPortInit(spiPort) == false) { return ERROR_NOT_INITIALIZED; }
        const uint8_t spiIndex = spi_get_index(spiPort);
        spi_deinit(spiPort);
        gpio_set_function(getClockPin(spiPort), GPIO_FUNC_NULL);
        gpio_set_function(getMisoPin(spiPort), GPIO_FUNC_NULL);
        gpio_set_function(getMosiPin(spiPort), GPIO_FUNC_NULL);
        if (myHelpers::isPin(getChipSelectPin(spiPort)) == true) {
            gpio_set_function(getChipSelectPin(spiPort), GPIO_FUNC_NULL);
        }
        __setPins__(spiPort, MY_NOT_A_PIN, MY_NOT_A_PIN, MY_NO_ERROR, MY_NOT_A_PIN);
        return NO_ERROR;
    }
/*
 * ######################### Transfer functions: ######################
 */
    /**
     * @brief Write LSB first blocking.
     * Reverses the bit order of the data, and sends via spi.
     * @param spiPort SPI port instance.
     * @param src Data to send.
     * @param len Length of data in bytes.
     * @return int Bytes written.
     */
    int write_lsb_blocking(spi_inst_t *spiPort, const uint8_t *src, size_t len) {
        int bytes = 0;
        uint8_t reversedByte;
        for (size_t i=0; i<len; i++) {
            reversedByte = myHelpers::reverse8(src[i]);
            spi_write_blocking(spiPort, &reversedByte, 1);
            bytes++;
        }
        return bytes;
    }
    /**
     * @brief Read LSB first blocking.
     * Reads data from SPI and reverses the bit order.
     * @param spiPort SPI port instance.
     * @param repeatedData Data to send during transfer.
     * @param dst Buffer for read data.
     * @param len Length of buffer in bytes.
     * @return int Bytes read.
     */
    int read_lsb_blocking(spi_inst_t *spiPort, uint8_t repeatedData, uint8_t *dst, size_t len) {
        int bytes = 0;
        uint8_t receivedByte = 0;
        for (size_t i=0; i<len; i++) {
            spi_read_blocking(spiPort, repeatedData, &receivedByte, 1);
            dst[i] = myHelpers::reverse8(receivedByte);
            bytes++;
        }
        return bytes;
    }
    /**
     * @brief Read / write LSB first blocking.
     * Transfers a buffer, and reverses the data so it's sent lsb first.
     * @param spiPort SPI port instance.
     * @param src Buffer to send.
     * @param dst Buffer to recieve.
     * @param len Length of buffer in bytes.
     * @return int Number of bytes transfered.
     */
    int write_read_lsb_blocking(spi_inst_t *spiPort, const uint8_t *src, uint8_t *dst, size_t len) {
        int bytes = 0;
        uint8_t reversedByte;
        uint8_t recievedByte;
        for (size_t i=0; i<len; i++) {
            reversedByte = myHelpers::reverse8(src[i]);
            spi_write_read_blocking(spiPort, &reversedByte, &recievedByte, 1);
            dst[i] = myHelpers::reverse8(recievedByte);
            bytes++;
        }
        return bytes;
    }
};
#endif