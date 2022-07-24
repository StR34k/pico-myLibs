/**
 * @file my25xx640A.hpp
 * @author Peter Nearing (pnearing@protonmail.com)
 * @brief Class to use my 25xx640A EEPROM spi chip.
 * @version 0.1
 * @date 2022-07-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MY_25xx640A_H
#define MY_25xx640A_H

#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <pico/time.h>

#include "../mySPI/mySPI.hpp"

class my25xx640A {

    public:
/* ############# Public constants: ############### */
    // EEPROM constants:
        static const int16_t MAX_ADDRESS = 0x1FFF; // 0b ---1 1111 1111 1111
        static const int8_t PAGE_SIZE = 32; // Write page size.
    // Error Codes:
        /**
         * @brief No error. Value 0.
         * No error occured.
         */
        static const int16_t NO_ERROR = MY_NO_ERROR;
        /**
         * @brief Invalid chip select pin. Value -153.
         * Invalid cs pin was passed.
         */
        static const int16_t ERROR_INVALID_CS_PIN = MY_ERROR_SPI_INVALID_CS_PIN;
        /**
         * @brief Invalid address. Value -325.
         * Invalid address was passed.
         */
        static const int16_t ERROR_INVALID_ADDRESS = MY_ERROR_MY25xx640A_INVALID_ADDRESS;
        /**
         * @brief EEPROM is held. Value -326.
         * EEPROM is held when action requested.
         */
        static const int16_t ERROR_HELD = MY_ERROR_MY25xx640A_EEPROM_HELD;
        /**
         * @brief EEPROM is busy. Value -327.
         * EEPROM is already busy, or busy when action requested.
         */
        static const int16_t ERROR_BUSY = MY_ERROR_MY25xx640A_EEPROM_BUSY;
        /**
         * @brief EEPROM is idle. Value -328.
         * EEPROM is already idle, or idle when action requested.
         */
        static const int16_t ERROR_IDLE = MY_ERROR_MY25xx640A_EEPROM_IDLE;
        /**
         * @brief Hold not defined. Value -329.
         * Hold pin was not defined during construction.
         */
        static const int16_t ERROR_HOLD_NOT_DEFINED = MY_ERROR_MY25xx640A_HOLD_NOT_DEFINED;
        static const int16_t ERROR_NOT_READING = MY_ERROR_MY25xx640A_NOT_READING;
        static const int16_t ERROR_NOT_WRITING = MY_ERROR_MY25xx640A_NOT_WRITING;

/* ############ Constructors: ############### */
        /**
         * @brief Construct a new my25xx640A Object.
         * Create a new my25xx640A object. If holdPin, or wpPin are set to invalid pins, then their
         * functionality is disabled.
         * @param spiPort SPI port to use.
         * @param sckPin Clock pin.
         * @param misoPin MISO pin.
         * @param mosiPin MOSI pin.
         * @param csPin Chip Select pin.
         * @param holdPin Hold pin.
         * @param wpPin Write protect pin.
         */
        my25xx640A(spi_inst_t *spiPort, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin,
                        const uint8_t csPin, const uint8_t holdPin, const uint8_t wpPin) : _sckPin (sckPin),
                        _misoPin (misoPin), _mosiPin (mosiPin), _csPin (csPin), _holdPin (holdPin),
                        _wpPin (wpPin) {
            _spiPort = spiPort;
        }
/* ############ Public functions: ############# */
        /**
         * @brief Check if an address is valid.
         * Verifies a passed address is valid.
         * @param address Address to validate.
         * @return true Address is valid.
         * @return false Address is invalid.
         */
        bool isValidAddress(const int16_t address);
        /**
         * @brief Start a read.
         * Starts a read from the EEPROM, returns 0 (NO_ERROR) if started okay, otherwise if the EEPROM
         * is held, or EEPROM is not idle, or an invalid address is passed, an error code is returned.
         * @param address Address to start read at.
         * @return int16_t Returns 0 (NO_ERROR) if started okay, negative for error code.
         */
        int16_t startRead(const int16_t address);
        /**
         * @brief Read a buffer from the EEPROM
         * Reads a buffer from the EEPROM. Returns 0 (NO_ERROR) if read okay, otherwise if the
         * EEPROM is idle, or held an error code is returned.
         * @param recvBuffer Buffer to store read data in.
         * @param len Length of buffer to read.
         * @return int16_t Returns 0(NO_ERROR) if read okay, negative for error code.
         */
        int16_t read(uint8_t *recvBuffer, size_t len);
        /**
         * @brief Read and return a single byte.
         * Reads and returns a single byte. Returns 0-255 (positive) for byte read, otherwise
         * if not reading, or held an error code is returned.
         * @return int16_t Returns 0-255 (Positive) for value read, negative for error code.
         */
        int16_t readByte();
        /**
         * @brief Initialize the EEPROM.
         * Initializes the SPI bus, and sets the current state. Returns 0 (NO_ERROR) if intialized okay, otherwise
         * if an invalid pin is passed during construction, an error code is returned.
         * @param initSPI True = initialize SPI bus, False = do not initialize SPI bus, assumes already initialized.
         * @param VCC Float, VCC for the EEPROM, determines clock speed, defaults to 3.3v.
         * @return int16_t Returns 0 if initialized okay, negative for error code.
         */
        int16_t initialize(const bool initSPI=true, const float VCC=3.3f);

    private:
/* ############## Private constants: ########### */
    // SPI COMMANDS:
        const uint8_t CMD_READ  = 0x03;      // Read bytes.
        const uint8_t CMD_WRITE = 0x02;      // Write byte / page.
        const uint8_t CMD_WRDI  = 0x04;      // Write disable.
        const uint8_t CMD_WREN  = 0x06;      // Write enable.
        const uint8_t CMD_RDSR  = 0x05;      // Read status register.
        const uint8_t CMD_WRSR  = 0x01;      // Write status register.
    // Timing:
        const uint8_t WRITE_TIME_MS = 5; // Internal write cyce time.
    // State masks and values:
        const uint8_t STATE_READ_WRITE_MASK = 0x03; // 0b ---- --11
        const uint8_t STATE_IDLE_VALUE      = 0x00; // 0b ---- --00
        const uint8_t STATE_READ_VALUE      = 0x01; // 0b ---- --01
        const uint8_t STATE_WRITE_VALUE     = 0x02; // 0b ---- --10
        const uint8_t STATE_HELD_MASK       = 0x04; // 0b ---- -1--
        const uint8_t STATE_WP_MASK         = 0x08; // 0b ---- 1---
    //Pins:
        const uint8_t _sckPin = MY_NOT_A_PIN;
        const uint8_t _misoPin = MY_NOT_A_PIN;
        const uint8_t _mosiPin = MY_NOT_A_PIN;
        const uint8_t _csPin = MY_NOT_A_PIN;
        const uint8_t _wpPin = MY_NOT_A_PIN;
        const uint8_t _holdPin = MY_NOT_A_PIN;
/* ############ Private variables: ############## */
    // SPI:
        spi_inst_t *_spiPort;
    // State:
        bool _haveHold = false;
        bool _haveWP = false;
        uint8_t _state = 0x00;
/* ############### Private Functions: ############# */
    // State functions:
        inline bool __isIdle__();      // Check read write bits = Idle.
        inline void __setIdle__();     // Set read write bits to idle.
        inline bool __isReading__();   // Check read write bits = reading.
        inline void __setReading__();  // Set read write bits to reading.
        inline bool __isWriting__();   // Check read write bits = writing.
        inline void __setWriting__();  // Set read write bits to writing.
        inline bool __isHeld__();      // Check Hold bit.
        inline void __setHold__(const bool value); // Set / clear hold bit.
        inline bool __isWriteProtect__(); // Check write protect bit.
        inline void __setWriteProtect__(const bool value); // Set / clear write protect bit.

};

/* ############## Public Functions: ############### */
bool my25xx640A::isValidAddress(const int16_t address) {
    if (address < 0 or address > MAX_ADDRESS) { return false; }
    return true;
}

int16_t my25xx640A::startRead(const int16_t address) {
    uint8_t dataBuffer[3];
    if (isValidAddress(address) == false) { return ERROR_INVALID_ADDRESS; }
    if (__isHeld__() == true) { return ERROR_HELD; }
    if (__isIdle__() == false) { return ERROR_BUSY; }
    dataBuffer[0] = CMD_READ;
    dataBuffer[1] = uint8_t(address >> 8);
    dataBuffer[2] = uint8_t(address);
    gpio_put(_csPin, false);
    spi_write_blocking(_spiPort, dataBuffer, 3);
    __setReading__();
    return NO_ERROR;
}

int16_t my25xx640A::read(uint8_t *recvBuffer, size_t len) {
    if (__isReading__() == false) { return ERROR_NOT_READING; }
    spi_read_blocking(_spiPort, 0x00, recvBuffer, len);
}

int16_t my25xx640A::initialize(const bool initSPI, const float VCC) {
    int32_t returnValue;
    if (initSPI == true) {
        uint32_t baudRate;
        if (VCC >= 4.5f) {
            baudRate = 10000 * 1000; // 10 MHz.
        } else if (VCC >= 2.5f) {
            baudRate = 5000 * 1000; // 5 MHz.
        } else {
            baudRate = 3000 * 1000; // 3 MHz.
        }
        returnValue = mySPI::initializeMaster(_spiPort, _sckPin, _misoPin, _mosiPin, baudRate);
        if (returnValue < 0) { return (int16_t)returnValue; }
    }
// Init gpio pins:
    // cs Pin.
    if (myHelpers::isPin(_csPin) == false) { return ERROR_INVALID_CS_PIN; }
    gpio_init(_csPin);
    gpio_set_dir(_csPin, GPIO_OUT);
    gpio_put(_csPin, true);
    // Hold pin.
    if (myHelpers::isPin(_holdPin) == true) {
        gpio_init(_holdPin);
        gpio_set_dir(_holdPin, GPIO_OUT);
        gpio_put(_holdPin, true);
        _haveHold = true;
    }
    // Write protect pin:
    if (myHelpers::isPin(_wpPin) == true) {
        gpio_init(_wpPin);
        gpio_set_dir(_wpPin, GPIO_OUT);
        gpio_put(_wpPin, true);
        _haveWP = true;
    }
}

/* ############## Private Functions: #################### */
inline bool my25xx640A::__isIdle__() {
    return ((_state & STATE_READ_WRITE_MASK) == STATE_IDLE_VALUE);
}

inline void my25xx640A::__setIdle__() {
    _state &= ~ STATE_READ_WRITE_MASK;
}

inline bool my25xx640A::__isReading__() {
    return ((_state & STATE_READ_WRITE_MASK) == STATE_READ_VALUE);
}

inline void my25xx640A::__setReading__() {
    _state &= ~ STATE_READ_WRITE_MASK;
    _state |= STATE_READ_VALUE;
}

inline bool my25xx640A::__isWriting__() {
    return ((_state & STATE_READ_WRITE_MASK) == STATE_WRITE_VALUE);
}

inline void my25xx640A::__setWriting__() {
    _state &= ~ STATE_READ_WRITE_MASK;
    _state |= STATE_WRITE_VALUE;
}

inline bool my25xx640A::__isHeld__() {
    return bool(_state & STATE_HELD_MASK);
}

inline void my25xx640A::__setHold__(const bool value) {
    if (value == true) {
        _state |= STATE_HELD_MASK;
    } else {
        _state &= ~ STATE_HELD_MASK;
    }
}

inline bool my25xx640A::__isWriteProtect__() {
    return bool(_state & STATE_WP_MASK);
}

inline void my25xx640A::__setWriteProtect__(const bool value) {
    if (value == true) {
        _state |= STATE_WP_MASK;
    } else {
        _state &= ~ STATE_WP_MASK;
    }
}


#endif