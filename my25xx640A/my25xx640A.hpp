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
    // Block protect constants: (Shifted by 2)
        static const uint8_t BP_NONE            = 0x00; // 0b ---- 00--
        static const uint8_t BP_UPPER_QUARTER   = 0x01; // 0b ---- 01--
        static const uint8_t BP_UPPER_HALF      = 0x02; // 0b ---- 10--
        static const uint8_t BP_ALL             = 0x03; // 0b ---- 11--
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
        /**
         * @brief Not in reading state. Value -330.
         * Not in reading state when read requested.
         */
        static const int16_t ERROR_NOT_READING = MY_ERROR_MY25xx640A_NOT_READING;
        /**
         * @brief Not in writing state. Value -331.
         * Not in writing state when write requested.
         */
        static const int16_t ERROR_NOT_WRITING = MY_ERROR_MY25xx640A_NOT_WRITING;
        /**
         * @brief Not held. Value -332.
         * EEPROM not held when action requested.
         */
        static const int16_t ERROR_NOT_HELD = MY_ERROR_MY25xx640A_NOT_HELD;
        /**
         * @brief Invalid block protect value. Value -333.
         * Invalid block protect value passed.
         */
        static const int16_t ERROR_INVALID_BLOCK = MY_ERROR_MY25xx640A_INVALID_BLOCK;
        /**
         * @brief Address write protected. Value -334.
         * The address is write protected at time of write, or is already write protected
         * when write protect was called.
         */
        static const int16_t ERROR_WRITE_PROTECTED = MY_ERROR_MY25xx640A_WRITE_PROTECTED;
        /**
         * @brief Not write protected. Value -335.
         * The write protect was already disabled when write protect clear was called.
         */
        static const int16_t ERROR_NOT_WRITE_PROTECTED = MY_ERROR_MY25xx640A_NOT_WRITE_PROTECTED;
/* ############ Constructors: ############### */
        /**
         * @brief Construct a new my25xx640A object, no hold, no write protect.
         * Create a new my25xx640A object. Without hold and write protect pin. When writing assumes
         * wp is disabled, and attempts the write, and assumes success. If write prtotect is enabled
         * no data is actually written.
         * @param spiPort SPI instance.
         * @param sckPin Clock pin.
         * @param misoPin MISO pin.
         * @param mosiPin MOSI pin.
         * @param csPin Chip select pin.
         */
        my25xx640A(spi_inst_t *spiPort, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin,
                        const uint8_t csPin) : _sckPin (sckPin), _misoPin (misoPin), _mosiPin (mosiPin),
                        _csPin (csPin) {
            _spiPort = spiPort;
        }
        /**
         * @brief Construct a new my25xx640A object, with hold, no write protect.
         * Creates a new my25xx640A object with hold, and without write protect. If write protection is
         * enabled and write is called, the write assumes success, even though no data is written.
         * @param spiPort SPI port instance.
         * @param sckPin Clock pin.
         * @param misoPin MISO pin.
         * @param mosiPin MOSI pin.
         * @param csPin Chip select pin.
         * @param holdPin Hold pin.
         */
        my25xx640A(spi_inst_t *spiPort, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin,
                        const uint8_t csPin, const uint8_t holdPin) : _sckPin (sckPin), _misoPin (misoPin),
                        _mosiPin (mosiPin), _csPin (csPin), _holdPin (holdPin) {
            _spiPort = spiPort;
        }
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
         * @brief Check if a block is valid.
         * Verifies a block setting for write protection mode.
         * @param block Block to verify
         * @return true Block is valid.
         * @return false Block is invalid.
         */
        bool isValidBlock(const uint8_t block);
        /**
         * @brief Check if an address is write protected.
         * Checks if an address is write protected, Returns true if address is write protected, false if not.
         * If an invalid address is passes, it will always return true, even if write protection is disabled.
         * @param address Address to verify.
         * @return true Address is write protected, or invalid.
         * @return false Address is not write protected.
         */
        bool isAddressWriteProtected(const int16_t address);
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
         * @brief Start a write.
         * Starts a write at given address. Returns 0 (NO_ERROR) if started okay, otherwise if
         * not idle an error code is returned.
         * @param address Address to start write at
         * @return int16_t Returns 0 (NO_ERROR) if started okay, negative for error code.
         */
        int16_t startWrite(const int16_t address);
        /**
         * @brief Write a buffer to the EEPROM.
         * Writes a buffer to the EEPROM. Returns 0 if written okay, otherwise if not writing or
         * if held, or if write protected, an error code is returned.
         * @param sendBuffer Buffer to send to EEPROM
         * @param len Length of buffer.
         * @return int16_t Return 0 (NO_ERROR) if written okay, negative for error code.
         */
        int16_t write(const uint8_t *sendBuffer, size_t len);
        /**
         * @brief Write a byte to the EEPROM.
         * Writes a single byte to the EEPROM. Retuns 0 if written okay, Otherwise if not writing, or
         * is held, or if write protected, an error code is returned.
         * @param value Value to write to the EEPROM.
         * @return int16_t Returns 0 (NO_ERROR) for okay, negaitve for error code.
         */
        int16_t writeByte(const uint8_t value);
        /**
         * @brief Stop a read / write.
         * Stops a read / write, returns 0 (NO_ERROR) if stopped okay, otherwise if idle, or if
         * held, or if write protected. an error code is returned.
         * @return int16_t Return 0 (NO_ERROR) if stopped okay, negative of error code.
         */
        int16_t stop();
        /**
         * @brief Set the hold pin.
         * Sets the hold pin. Returns 0 (NO_ERROR) if held okay, otherwise if already held, or
         * not in an active state, an error code is returned.
         * @return int16_t Return 0 (NO_ERROR) on success, negative for error code.
         */
        int16_t setHold();
        /**
         * @brief Clear the hold pin.
         * Clears the hold. Returns 0 (NO_ERROR) on success, otherwise if not held, or idle,
         * an error code is returned.
         * @return int16_t Returns 0 (NO_ERROR) on success, negative for error code.
         */
        int16_t clearHold();
        /**
         * @brief Set the Write Protect Block.
         * Sets the write protect block. Returns 0 (NO_ERROR) if set okay, otherwise if busy, or if
         * the write protect is enabled, an error code is returned.
         * @param block Block value to set. Valid values (0x00-0x03), BP_* values are provided.
         * @return int16_t Returns 0 (NO_ERROR) if set okay, negative for error code.
         */
        int16_t setWriteProtectBlock(const uint8_t block);
        /**
         * @brief Enable write protect.
         * Enables the write protect. If the Write protect pin is not defined, the pin is assumed to be
         * tied high. Returns 0 (NO_ERROR) if set okay, otherwise if the write protect is already enabled,
         * or if the deviece is busy, an error code is returned.
         * @return int16_t Returns 0 (NO_ERROR) if set okay, negative for error code.
         */
        int16_t setWriteProtect();
        /**
         * @brief Disable write protect.
         * Disables the write protect. If the write protect pin is not defined, the pin is assue to be
         * tied high.  Returns 0 (NO_ERROR) if cleared okay, otherwise if already disabled, or if the
         * device is busy an error code is returned.
         * @return int16_t Returns 0 (NO_ERROR) if cleared okay, negative for error code.
         */
        int16_t clearWriteProtect();
        /**
         * @brief Template to write anything to the EEPROM.
         * Template to write anything to the EEPROM. Returns number of bytes written on success, otherwise
         * if not writing, or if held, or if write protected, an error code is returned.
         * @param value Value to write to EEPROM of any type.
         * @return int32_t Positive, number of bytes written, negative for error code.
         */
        template <typename T> int32_t writeAnything(const T &value);

        template <typename T> int32_t readAnything(T &value);
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
    // Status byte masks:
        const uint8_t STATUS_WPEN_MASK  = 0x80; // 0b 1--- ----
        const uint8_t STATUS_BP_MASK    = 0x0C; // 0b ---- 11--
        const uint8_t STATUS_WEL_MASK   = 0x02; // 0b ---- --1-
        const uint8_t STATUS_WIP_MASK   = 0x01; // 0b ---- ---1
    // Block protection addresses:
        const int16_t BP_UPPER_QUARTER_ADDR = 0x1800;
        const int16_t BP_UPPER_HALF_ADDR    = 0x1000;
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
        int16_t _address = 0x0000;
        uint8_t _block = 0x00;
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
        inline int16_t __incAddress__(); // Increment the stored address.
        int16_t __calculateNextBoundary__(); // Use _address to calculate the next boundary.
        uint8_t __readByte__(); // Read and return a single byte.
        void __writeByte__(const uint8_t value); // Write a single byte, and restart write if boundary crossed. Increments address.
        uint8_t __readStatus__(); // Read and return the status byte.
        void __writeStatus__(const uint8_t value); // Write the status register to value.

};

/* ############## Public Functions: ############### */
bool my25xx640A::isValidAddress(const int16_t address) {
    if (address < 0 or address > MAX_ADDRESS) { return false; }
    return true;
}

bool my25xx640A::isValidBlock(const uint8_t block) {
    if (block > BP_ALL) { return false; }
    return true;
}

bool my25xx640A::isAddressWriteProtected(const int16_t address) {
    if (isValidAddress(address) == false) { return true; }
    if (__isWriteProtect__() == false) { return false; }    
    switch (_block) {
        case BP_ALL:
            return true;
        case BP_UPPER_HALF:
            if (address >= BP_UPPER_HALF_ADDR) { return true; }
            return false;
        case BP_UPPER_QUARTER:
            if (address >= BP_UPPER_QUARTER_ADDR) { return true; }
            return false;
        case BP_NONE:
            return false;
    }
}

int16_t my25xx640A::startRead(const int16_t address) {
    uint8_t dataBuffer[3];
    if (isValidAddress(address) == false) { return ERROR_INVALID_ADDRESS; }
    if (__isIdle__() == false) { return ERROR_BUSY; }
    if (__isHeld__() == true) { return ERROR_HELD; }
    dataBuffer[0] = CMD_READ;
    dataBuffer[1] = uint8_t(address >> 8);
    dataBuffer[2] = uint8_t(address);
    gpio_put(_csPin, false);
    spi_write_blocking(_spiPort, dataBuffer, 3);
    _address = address;
    __setReading__();
    return NO_ERROR;
}

int16_t my25xx640A::read(uint8_t *recvBuffer, size_t len) {
    if (__isReading__() == false) { return ERROR_NOT_READING; }
    if (__isHeld__() == true) { return ERROR_HELD; }
    for (size_t i=0; i<len; i++) {
        recvBuffer[i] = __readByte__();
        __incAddress__();
    }
    return NO_ERROR;
}

int16_t my25xx640A::readByte() {
    if (__isReading__() == false) { return ERROR_NOT_READING; }
    if (__isHeld__() == true ) { return ERROR_HELD; }
    uint8_t readValue = __readByte__();
    __incAddress__();
    return int16_t(readValue);
}

int16_t my25xx640A::startWrite(const int16_t address) {
    uint8_t dataBuffer[3];
    if (isValidAddress(address) == false) { return ERROR_INVALID_ADDRESS; }
    if (__isIdle__() == false) { return ERROR_BUSY; }
    if (__isHeld__() == true) { return ERROR_HELD; }
    if (isAddressWriteProtected(_address) == true) { return ERROR_WRITE_PROTECTED; }
    gpio_put(_csPin, false);
    spi_write_blocking(_spiPort, &CMD_WREN, 1);
    gpio_put(_csPin, true);
    dataBuffer[0] = CMD_WRITE;
    dataBuffer[1] = uint8_t(address >> 8);
    dataBuffer[2] = uint8_t(address);
    gpio_put(_csPin, false);
    spi_write_blocking(_spiPort, dataBuffer, 3);
    _address = address;
    __setWriting__();
    return NO_ERROR;
}

int16_t my25xx640A::write(const uint8_t *sendBuffer, size_t len) {
    uint8_t pageBuffer[PAGE_SIZE];
    if (__isWriting__() == false) { return ERROR_NOT_WRITING; }
    if (__isHeld__() == true) { return ERROR_HELD; }
    for (size_t i=0; i<len; i++) {
        if (isAddressWriteProtected(_address) == true) { return ERROR_WRITE_PROTECTED; }
        __writeByte__(sendBuffer[i]);
    }
    return NO_ERROR;
}

int16_t my25xx640A::writeByte(const uint8_t value) {
    if (__isWriting__() == false) { return ERROR_NOT_WRITING; }
    if (__isHeld__() == true) { return ERROR_HELD; }
    if (isAddressWriteProtected(_address) == true) { return ERROR_WRITE_PROTECTED; }
    __writeByte__(value);
    return NO_ERROR;
}

int16_t my25xx640A::stop() {
    if (__isIdle__() == true) { return ERROR_IDLE; }
    if (__isHeld__() == true) { return ERROR_HELD; }
    gpio_put(_csPin, true);
    if (__isWriting__() == true) {
        sleep_ms(WRITE_TIME_MS);
    }
    __setIdle__();
    return NO_ERROR;
}

int16_t my25xx640A::setHold() {
    if (_haveHold == false) { return ERROR_HOLD_NOT_DEFINED; }
    if (__isHeld__() == true) { return ERROR_HELD; }
    if (__isIdle__() == true) { return ERROR_IDLE; }
    gpio_put(_holdPin, false);
    __setHold__(true);
    return NO_ERROR;
}

int16_t my25xx640A::clearHold() {
    if (_haveHold == false) { return ERROR_HOLD_NOT_DEFINED; }
    if (__isHeld__() == false) { return ERROR_NOT_HELD; }
    if (__isIdle__() == true) { return ERROR_IDLE; }
    gpio_put(_holdPin, true);
    __setHold__(false);
    return NO_ERROR;
}

int16_t my25xx640A::setWriteProtectBlock(const uint8_t block) {
    uint8_t status;
    if (isValidBlock(block) == false) { return ERROR_INVALID_BLOCK; }
    if (__isIdle__() == false) { return ERROR_BUSY; }
    if (__isWriteProtect__() == true) { return ERROR_WRITE_PROTECTED; }
    status = __readStatus__();
    status &= ~STATUS_BP_MASK; // Clear block protect.
    status |= block << 2; // Set block protect.
    __writeStatus__(status);
    _block = block;
    return NO_ERROR;
}

int16_t my25xx640A::setWriteProtect() {
    uint8_t status;
    if (__isWriteProtect__() == true) { return ERROR_WRITE_PROTECTED; }
    if (__isIdle__() == false) { return ERROR_BUSY; }
    status = __readStatus__();
    status |= STATUS_WPEN_MASK;
    __writeStatus__(status);
    if (_haveWP == true) {
        gpio_put(_wpPin, false);
    }
    __setWriteProtect__(true);
    return NO_ERROR;
}

int16_t my25xx640A::clearWriteProtect() {
    uint8_t status;
    if (__isIdle__() == false) { return ERROR_BUSY; }
    if (__isWriteProtect__() == false) { return ERROR_NOT_WRITE_PROTECTED; }
    if (_haveWP == true) {
        gpio_put(_wpPin, true);
    }
    status = __readStatus__();
    status &= ~STATUS_WPEN_MASK;
    __writeStatus__(status);
    __setWriteProtect__(false);
    return NO_ERROR;
}

template <typename T> int32_t my25xx640A::writeAnything(const T &value) {
    const uint8_t *p = (const uint8_t *) &value;
    int32_t i;
    if (__isWriting__() == false) { return ERROR_NOT_WRITING; }
    if (__isHeld__() == true) { return ERROR_HELD; }
    for (i=0; i<sizeof(value); i++) {
        if (isAddressWriteProtected(_address) == true) { return ERROR_WRITE_PROTECTED; }
        __writeByte__(*p++);
    }
    return i;
}

template <typename T> int32_t my25xx640A::readAnything( T &value) {
    uint8_t *p = (uint8_t *)&value;
    int32_t i;
    if (__isReading__() == false) { return ERROR_NOT_READING; }
    if (__isHeld__() == true) { return ERROR_HELD; }
    for (i=0; i<sizeof(value); i++) {
        if (isAddressWriteProtected(_address) == true) { return ERROR_WRITE_PROTECTED; }
        *p++ = __readByte__();
    }
    return i;
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

    if (myHelpers::isPin(_holdPin) == true) {
        gpio_init(_holdPin);
        gpio_set_dir(_holdPin, GPIO_OUT);
        gpio_put(_holdPin, true);
    }

    if (myHelpers::isPin(_wpPin) == true) {
        gpio_init(_wpPin);
        gpio_set_dir(_wpPin, GPIO_OUT);
        gpio_put(_wpPin, true);
    }
    return NO_ERROR;
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

inline int16_t my25xx640A::__incAddress__() {
    int16_t returnValue = _address++;
    if (_address > MAX_ADDRESS) { _address = 0; }
    return returnValue;
}

int16_t my25xx640A::__calculateNextBoundary__() {
    return (_address + (PAGE_SIZE - (_address % PAGE_SIZE) -1));
}

uint8_t my25xx640A::__readByte__() {
    uint8_t value;
    spi_read_blocking(_spiPort, 0x00, &value, 1);
    return value;
}

void my25xx640A::__writeByte__(const uint8_t value) {
    uint8_t addressBuffer[3];
    spi_write_blocking(_spiPort, &value, 1);
    int16_t boundary = __calculateNextBoundary__();
    if (_address == boundary) {
        gpio_put(_csPin, true);
        sleep_ms(WRITE_TIME_MS);
        gpio_put(_csPin, false);
        spi_write_blocking(_spiPort, &CMD_WREN, 1);
        gpio_put(_csPin, true);
        __incAddress__();
        addressBuffer[0] = CMD_WRITE;
        addressBuffer[1] = uint8_t(_address >> 8);
        addressBuffer[2] = uint8_t(_address);
        gpio_put(_csPin, false);
        spi_write_blocking(_spiPort, addressBuffer, 3);
    } else {
        __incAddress__();
    }
}

uint8_t my25xx640A::__readStatus__() {
    uint8_t status;
    gpio_put(_csPin, false);
    spi_write_blocking(_spiPort, &CMD_RDSR, 1);
    spi_read_blocking(_spiPort, 0x00, &status, 1);
    gpio_put(_csPin, true);
    return status;
}

void my25xx640A::__writeStatus__(const uint8_t value) {
    gpio_put(_csPin, false);
    spi_write_blocking(_spiPort, &CMD_WRSR, 1);
    spi_write_blocking(_spiPort, &value, 1);
    gpio_put(_csPin, true);
}
#endif