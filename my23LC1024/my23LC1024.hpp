
#ifndef MY23LC1024_H
#define MY23LC1024_H
#include "../myStandardDefines.hpp"
#include "../myErrorCodes.hpp"
#include "../myBitBangSPI/myBitBangSPI.hpp"
#include <pico/stdlib.h>
#include <pico/time.h>
#include <hardware/spi.h>

class my23LC1024 {

    public:
    /* Constants: */
        // Communication Modes:
        static const uint8_t COMM_MODE_SPI  = 0x00;
        static const uint8_t COMM_MODE_SDI  = 0x01;
        static const uint8_t COMM_MODE_SQI  = 0x02;
	    // Sram instruction set:
		static const uint8_t READ_INSTRUCTION	= 0x03; // Start read
		static const uint8_t WRITE_INSTRUCTION	= 0x02; // Start write
		static const uint8_t EDIO_INSTRUCTION	= 0x3B; // Enter SDI comms mode.
		static const uint8_t EQIO_INSTRUCTION	= 0x38; // Enter SQI comms mode.
		static const uint8_t RSTIO_INSTRUCTION	= 0xFF; // Reset comms mode.
		static const uint8_t RDMR_INSTRUCTION	= 0x05; // Read mode register.
		static const uint8_t WRMR_INSTRUCTION	= 0x01; // Write mode register.
        // Sram operating modes:
        static const uint8_t SRAM_MODE_BYTE     = 0x00; // Sram single byte mode.
        static const uint8_t SRAM_MODE_SEQ      = 0x40; // Sram sequential mode, default mode, and our mode.
        static const uint8_t SRAM_MODE_PAGE     = 0x80; // Sram 32 byte page mode.
        // Valid address Mask, and length:
        static const uint32_t VALID_ADDRESS_MASK    = 0x0001FFFF; // 0b 0000 0000 0000 0001 1111 1111 1111 1111
        static const uint32_t MAX_LENGTH = VALID_ADDRESS_MASK +1; // Set the max length.
        static const uint32_t ADDRESS_HIGH_BYTE_MASK    = 0x00010000; // The mask for a valid high byte.
        static const uint32_t ADDRESS_MIDDLE_BYTE_MASK  = 0x0000FF00; // The mask for a valid middle byte.
        static const uint32_t ADDRESS_LOW_BYTE_MASK     = 0x000000FF; // The mask for a valid low byte.
    /* status byte values: */
        // Reading / Writing states
        static const uint8_t STATUS_STATE_MASK      = 0X03; // Sram state mask.     0b 0000 0011
        static const uint8_t STATUS_STATE_IDLE      = 0x00; // Sram is idle.        0b 0000 0000
        static const uint8_t STATUS_STATE_READ      = 0x01; // Sram is reading.     0b 0000 0001
        static const uint8_t STATUS_STATE_WRITE     = 0x02; // Sram is writing.     0b 0000 0010
        static const uint8_t STATUS_STATE_READ_BIT  = 0X01; // sram reading bit.    0b 0000 0001
        static const uint8_t STATUS_STATE_WRITE_BIT = 0X02; // Sram writing bit.    0b 0000 0010
        // Hold bit:
        static const uint8_t STATUS_HOLD_MASK       = 0X04; // Bit set if held.     0b 0000 0100
        static const uint8_t STATUS_IS_HELD         = 0x04; // Value if bit is set. 0b 0000 0100
        // Error codes:
        static const int32_t ERROR_NO_ERROR = MY_NO_ERROR;                          // No error.
        static const int32_t ERROR_INVAID_ADDRESS = MY_ERROR_MY23LC1024_INVALID_ADDRESS;       // An invalid address was passed to a function.
        static const int32_t ERROR_SRAM_HELD = MY_ERROR_MY23LC1024_SRAM_HELD;                  // Sram has been held by the hold pin.
        static const int32_t ERROR_SRAM_BUSY = MY_ERROR_MY23LC1024_SRAM_BUSY;                  // Sram is already busy.
        static const int32_t ERROR_SRAM_IDLE = MY_ERROR_MY23LC1024_SRAM_IDLE;                  // Sram is already idle.
        static const int32_t ERROR_HOLD_NOT_DEFINED = MY_ERROR_MY23LC1024_HOLD_NOT_DEFINED;    // Hold pin is not defined.
        static const int32_t ERROR_SIO2_NOT_DEFINED = MY_ERROR_MY23LC1024_SIO2_NOT_DEFINED;    // Sio2 pin is not defined.
        static const int32_t ERROR_SRAM_NOT_HELD = MY_ERROR_MY23LC1024_SRAM_NOT_HELD;          // Sram is not held.
        static const int32_t ERROR_NOT_READING = MY_ERROR_MY23LC1024_NOT_READING;              // Sram is not in the reading state.
        static const int32_t ERROR_NOT_WRITING = MY_ERROR_MY23LC1024_NOT_WRITING;              // Sram is not in the writing state.
        static const int32_t ERROR_HOLD_NOT_AVAILABLE = MY_ERROR_MY23LC1024_HOLD_NOT_AVAILABLE;// Hold function is not available.
    /* Constructor: */
        // HW SPI Constructors:
        my23LC1024(spi_inst_t *spiPort, const uint8_t csPin, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin); // HW SPI no hold.
        my23LC1024(spi_inst_t *spiPort, const uint8_t csPin, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin, const uint8_t holdPin); // HW SPI with hold.
        // Bit Bang comms Constructors:
        my23LC1024(const uint8_t csPin, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin); // Bit banged spi / sdi, no hold.
        my23LC1024(const uint8_t csPin, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin, const uint8_t holdPin); // Bit banged spi / sdi with hold.
        my23LC1024(const uint8_t csPin, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin, const uint8_t holdPin, const uint8_t sio2Pin); // Sqi. no hold.
    /* Functions: */
        bool            initialize(const uint8_t commsMode);                        // Return true if comms verified, false if not.
        bool inline     validateAddress(const int32_t address);                     // Return true if address is valid, false if not.
        bool inline     isIdle();                                                   // Return True if currently idle.
        bool inline     isBusy();                                                   // Return true if currently reading or writing.
        bool inline     isReading();                                                // Return true if currently in a read.
        bool inline     isWriting();                                                // return true if currently in a write.
        bool inline     isHeld();                                                   // Return true if comms are held.
        int32_t         setHold();                                                  // Set the hold. returns 0 on success or negative for error code.
        int32_t         clearHold();                                                // Clear the hold. returns 0 on success or negative for error code.
        int32_t inline  getNextAddress();                                           // Return the next address. -1 means address hasn't been set.
        int32_t inline  getLastAddress();                                           // Return the last address read / written. -1 means no address has been read / written.
        int32_t         startRead(const int32_t address);                           // Start the read process. return 0 for okay, negative for error code.
        int32_t         read();                                                     // Read a single byte returns byte read. 0 -> 255 byte read, negative for error code.
        int32_t         startWrite(const int32_t address);                          // start the write process returns 0 for okay, negative for error code.
        int32_t         write(const uint8_t value);                                 // write a single byte. returns 0 for okay, negative for error code.
        int32_t         stop();                                                     // stop the read / write process. returns 0 for okay, negative for error code.

    private:
    /* Variables: */
        spi_inst_t *_spiPort;       // The spi object from the sdk
        uint8_t _csPin;             // The cs pin
        uint8_t _sckPin;            // the sck pin
        uint8_t _misoPin;           // the rx pin
        uint8_t _mosiPin;           // the tx pin
        uint8_t _holdPin;           // the hold pin.
        uint8_t _sio2Pin;           // the sio2 pin.
        uint8_t _commsMode;         // the comms mode.
        bool    _useHWSPI = false;  // true if we are useing hw spi.
        uint8_t _status = 0x00;     // the status byte. Packed uint8.
        int32_t _nextAddress = -1;  // The next address to read / write.
        int32_t _lastAddress = -1;  // The last address read / written.
        uint8_t _lastState;         // The last reading or writing state. for mimicing hold.

    /* Functions: */
        void inline __selectChip__();                                               // Lower the cs line.
        void inline __deselectChip__();                                             // raise the cs line.
        void inline __incrementAddress__();                                         // Increment the stored address, wrapping at max address.
        void inline __setAddress__(int32_t value);                                 // Set the address we're working on.
        uint8_t inline __getState__();                                              // return the state bits from the status byte.
        void inline __setStateIdle__();                                             // Set the state bits in the status byte to idle.
        void inline __setStateRead__();                                             // Set the state bits in the status byte to reading.
        void inline __setStateWrite__();                                            // Set the state bits in the status byte to writing.
        void inline __setStatusHold__();                                            // set the hold bit in the status byte.
        void inline __clearStatusHold__();                                          // Clear the hold bit in the  status byte.
        void inline __setSPIPinModes__();                                           // Set the pin modes for Bit banged SPI.
        void inline __setSDIPinModes__(const bool isOutput);                        // Set the pin modes for bit banged SDI.
        void inline __setSQIPinModes__(const bool isOutput);                        // Set the pin modes for bit banged SQI.
        void        __resetComms__();                                               // Reset the comms mode to SPI.
        int32_t    __HWSPIRead__(uint8_t *buffer, const int32_t length);            // Use hw spi to read.
        int32_t    __HWSPIWrite__(const uint8_t *buffer, const int32_t length);     // Use hw spi to write.
        int32_t     __SPIRead__(uint8_t *buffer, const int32_t length);             // Bit banged SPI read.
        int32_t     __SPIWrite__(const uint8_t *buffer, const int32_t length);      // Bit banged SPI write.
        void        __writeByte__(const uint8_t value);                             // Select the right write function and transfer a single byte.
        int32_t    __writeBuffer__(const uint8_t *buffer, const int32_t length);    // Select the right write function and transfer a series of bytes. Returns the number of bytes written.
        uint8_t     __readByte__();                                                 // Select the right read function and read a single byte. returns byte read.
        int32_t    __readBuffer__(uint8_t *buffer, const int32_t length);           // Select the right read function and read a series of bytes. returns number of bytes read.
        uint8_t     __readModeRegister__();                                         // Return the value of the mode register.
        void        __writeModeRegister__(uint8_t value);                           // Set the value of the mode register.

};

/***********************************************************************************
 * Constructors:
 ***********************************************************************************/
/* HW SPI Constructors: */
// HW SPI without hold.
my23LC1024::my23LC1024(spi_inst_t *spiPort, const uint8_t csPin, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin) {
    _spiPort = spiPort;
    _csPin = csPin;
    _sckPin = sckPin;
    _misoPin = misoPin;
    _mosiPin = mosiPin;
    _holdPin = MY_NOT_A_PIN;
    _sio2Pin = MY_NOT_A_PIN;
    _useHWSPI = true;
}
// HW SPI with hold.
my23LC1024::my23LC1024(spi_inst_t *spiPort, const uint8_t csPin, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin, const uint8_t holdPin) {
    _spiPort = spiPort;
    _csPin = csPin;
    _sckPin = sckPin;
    _misoPin = misoPin;
    _mosiPin = mosiPin;
    _holdPin = holdPin;
    _sio2Pin = MY_NOT_A_PIN;
    _useHWSPI = true;
}
/* Bit Bang comms Constructors: */
// SPI / SDI with no hold.
my23LC1024::my23LC1024(const uint8_t csPin, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin) {
    _csPin = csPin;
    _sckPin = sckPin;
    _misoPin = misoPin;
    _mosiPin = mosiPin;
    _holdPin = MY_NOT_A_PIN;
    _sio2Pin = MY_NOT_A_PIN;
    _useHWSPI = false;
}
// SPI / SDI with hold.
my23LC1024::my23LC1024(const uint8_t csPin, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin, const uint8_t holdPin) {
    _csPin = csPin;
    _sckPin = sckPin;
    _misoPin = misoPin;
    _mosiPin = mosiPin;
    _holdPin = holdPin;
    _sio2Pin = MY_NOT_A_PIN;    
    _useHWSPI = false;
}
// SQI
my23LC1024::my23LC1024(const uint8_t csPin, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin, const uint8_t holdPin, const uint8_t sio2Pin) {
    _csPin = csPin;
    _sckPin = sckPin;
    _misoPin = misoPin;
    _mosiPin = mosiPin;
    _holdPin = holdPin;
    _sio2Pin = sio2Pin;
    _useHWSPI = false;
}
/**************************************************************************
 * Public functions:
 **************************************************************************/

bool my23LC1024::initialize(const uint8_t commsMode) {
// Store comms mode:
    _commsMode = commsMode;
// Do some basic checks:
    if (commsMode == COMM_MODE_SQI) { // SQI requires hold and sio2 to be defined.
        if (_holdPin == MY_NOT_A_PIN) { return false; } 
        if (_sio2Pin == MY_NOT_A_PIN) { return false; }
    }
// Set pin functions, modes, and initial states:
    gpio_set_function(_csPin,   GPIO_FUNC_SIO); // Set cs as gpio.
    gpio_set_dir(_csPin, GPIO_OUT); // Set cs as output.
    gpio_put(_csPin, true); // Set cs High.
    if (_useHWSPI == true) {
        spi_init(_spiPort, 1000*20000); // init spi at 20 Mhz.
        gpio_set_function(_sckPin,  GPIO_FUNC_SPI); // set sck as spi.
        gpio_set_function(_misoPin, GPIO_FUNC_SPI); // set miso as spi.
        gpio_set_function(_mosiPin, GPIO_FUNC_SPI); // set mosi as spi.
    } else {
        if (_commsMode != COMM_MODE_SPI) { // Bit baged spi is dealt with in myBitBangSPI
            gpio_set_function(_sckPin,   GPIO_FUNC_SIO); // set sck as gpio.
            gpio_set_function(_misoPin,  GPIO_FUNC_SIO); // Set miso as gpio. 
            gpio_set_function(_mosiPin,  GPIO_FUNC_SIO); // set mosi as gpio.
            gpio_set_dir(_sckPin, GPIO_OUT); // Set sck as output.
            gpio_set_dir(_misoPin, GPIO_IN); // Set miso as input.
            gpio_set_dir(_mosiPin, GPIO_OUT); // Set mosi as output.
            gpio_put(_sckPin, false); // set sck Low
            gpio_put(_mosiPin, false); // set mosi low
        }
    }
// Set pin functions, modes, and inital state of hold and sio2:
    if (_holdPin != MY_NOT_A_PIN) {
        gpio_set_function(_holdPin, GPIO_FUNC_SIO); // Set hold as gpio.
        gpio_set_dir(_holdPin, GPIO_OUT); // Set hold as output.
        gpio_put(_holdPin, true); // set hold High.
    }
    if (_sio2Pin != MY_NOT_A_PIN) {
        gpio_set_function(_sio2Pin, GPIO_FUNC_SIO); // set sio2 as gpio.
        gpio_set_dir(_sio2Pin, GPIO_OUT); // set sio2 as output.
        gpio_put(_sio2Pin, true); // set sio2 High.
    }
// Initialze comms:
    __resetComms__();
    if (_useHWSPI == false) {
        switch (_commsMode) {
            case COMM_MODE_SPI:
                mySPIMaster::initialize(_sckPin, _misoPin, _mosiPin);
                break;
            case COMM_MODE_SDI:
                ;
                break;
            case COMM_MODE_SQI:
                ;
                break;    
        }
    }
// Validate comms by checking mode, setting it something, and checking it again.
//   Then reset it back to sequential mode.
    // Read mode:
    uint8_t mode = __readModeRegister__();
    uint8_t expectedMode;
    // Make sure mode is valid:
    if (mode != SRAM_MODE_BYTE and mode != SRAM_MODE_SEQ and mode != SRAM_MODE_PAGE) {
        return false; // Invalid value returned.
    }
    // Write the mode register to a different non zero value. byte mode is 0x00 which could be an error.
    if (mode != SRAM_MODE_SEQ) {
        __writeModeRegister__(SRAM_MODE_SEQ);
        expectedMode = SRAM_MODE_SEQ;
    } else {
        __writeModeRegister__(SRAM_MODE_PAGE); // Page mode is non zero value.
        expectedMode = SRAM_MODE_PAGE;
    }
    // Read the mode register and verify:
    mode = __readModeRegister__();
    if (mode != expectedMode) {
        return false;
    }
    // Set the mode to sequential mode if not already.
    if (mode != SRAM_MODE_SEQ) {
        __writeModeRegister__(SRAM_MODE_SEQ);
    }
    return true;
}

bool inline my23LC1024::validateAddress(const int32_t address) {
    if (address > VALID_ADDRESS_MASK) { return false; }
    if (address < 0) { return false; }
    return true;
}

bool inline my23LC1024::isIdle() {
    return ((_status & STATUS_STATE_MASK) == 0);
}

bool inline my23LC1024::isBusy() {
    return ((_status & STATUS_STATE_MASK) != 0);
}

bool inline my23LC1024::isReading() {
    return (bool)(_status & STATUS_STATE_READ_BIT);
}

bool inline my23LC1024::isWriting() {
    return (bool)(_status & STATUS_STATE_WRITE_BIT);
}

bool inline my23LC1024::isHeld() {
    return (bool)(_status & STATUS_HOLD_MASK);
}

int32_t my23LC1024::setHold() {
    if (_holdPin == MY_NOT_A_PIN) { return ERROR_HOLD_NOT_DEFINED; }
    if (_commsMode == COMM_MODE_SQI) { return ERROR_HOLD_NOT_AVAILABLE; }
    if (isHeld() == true) { return ERROR_SRAM_HELD; }
    if (isIdle() == true) { return ERROR_SRAM_IDLE; }
    gpio_put(_holdPin, false);
    __setStatusHold__();
    return 0;
}

int32_t my23LC1024::clearHold() {
    if (_holdPin == MY_NOT_A_PIN) { return ERROR_HOLD_NOT_DEFINED; }
    if (isHeld() == false) { return ERROR_SRAM_NOT_HELD; }
    if (isIdle() == true) { return ERROR_SRAM_IDLE; }
    gpio_put(_holdPin, true);
    __clearStatusHold__();
    return 0;
}

int32_t inline my23LC1024::getNextAddress() {
    return _nextAddress;
}

int32_t inline my23LC1024::getLastAddress() {
    return _lastAddress;
}

int32_t my23LC1024::startRead(const int32_t address) {
    if (validateAddress(address)) { return ERROR_INVAID_ADDRESS; }
    if (isHeld() == true) { return ERROR_SRAM_HELD; }
    if (isBusy() == true) { return ERROR_SRAM_BUSY; }
    uint8_t buffer[4] = {
        READ_INSTRUCTION,                                       // Send read instruction 
        (uint8_t)((address & ADDRESS_HIGH_BYTE_MASK) >> 16),    // Send high byte of address.
        (uint8_t)((address & ADDRESS_MIDDLE_BYTE_MASK) >> 8),   // Send middle byte of address.
        (uint8_t)(address & ADDRESS_LOW_BYTE_MASK)              // Send low byte of address.
    };
    __selectChip__();
    __writeBuffer__(buffer, 4);
    switch (_commsMode) {
        case COMM_MODE_SDI:
            ; 
            break;
        case COMM_MODE_SQI:
            ;
            break;
    }
    __setAddress__(address);
    __setStateRead__();
    return 0;
}

int32_t my23LC1024::read() {
    if (isIdle() == true) { return ERROR_SRAM_IDLE; }
    if (isHeld() == true) { return ERROR_SRAM_HELD; }
    if (isReading() == false) { return ERROR_NOT_READING; }
    __incrementAddress__();
    return (int32_t)__readByte__();
}

int32_t my23LC1024::startWrite(const int32_t address) {
    if (validateAddress(address)) { return ERROR_INVAID_ADDRESS; }
    if (isHeld() == true) { return ERROR_SRAM_HELD; }
    if (isBusy() == true) { return ERROR_SRAM_BUSY; }
    uint8_t buffer[4] = {
        WRITE_INSTRUCTION,                                       // Send write instruction 
        (uint8_t)((address & ADDRESS_HIGH_BYTE_MASK) >> 16),    // Send high byte of address.
        (uint8_t)((address & ADDRESS_MIDDLE_BYTE_MASK) >> 8),   // Send middle byte of address.
        (uint8_t)(address & ADDRESS_LOW_BYTE_MASK)              // Send low byte of address.
    };
    __selectChip__();
    __writeBuffer__(buffer, 4);
    __setAddress__(address);
    __setStateWrite__();
    return ERROR_NO_ERROR;
}

int32_t my23LC1024::write(const uint8_t value) {
    if (isIdle() == true) { return ERROR_SRAM_IDLE; }
    if (isHeld() == true) { return ERROR_SRAM_HELD; }
    if (isWriting() == false) { return ERROR_NOT_WRITING; }
    __writeByte__(value);
    __incrementAddress__();
    return ERROR_NO_ERROR;
}

int32_t my23LC1024::stop() {
    if (isIdle() == true) { return ERROR_SRAM_IDLE; }
    if (isHeld() == true) { return ERROR_SRAM_HELD; }
    __deselectChip__();
    if (isReading() == true) {
        switch (_commsMode){
            case COMM_MODE_SDI:
                ;
                break;
            case COMM_MODE_SQI:
                ;
                break;
        }
    }
    __setStateIdle__();
    return ERROR_NO_ERROR;
}
/*********************************************************************
 * Private functions:
 *********************************************************************/
void inline my23LC1024::__selectChip__() {
    gpio_put(_csPin, false);
}

void inline my23LC1024::__deselectChip__() {
    gpio_put(_csPin, true);
}

void inline my23LC1024::__setAddress__(const int32_t value) {
    _lastAddress = _nextAddress;
    _nextAddress = value;
}

void inline my23LC1024::__incrementAddress__() {
    _lastAddress = _nextAddress;
    _nextAddress += 1;
    if (_nextAddress == MAX_LENGTH) { _nextAddress = 0; }
}

uint8_t inline my23LC1024::__getState__() {
    return (_status & STATUS_STATE_MASK);
}

void inline my23LC1024::__setStateIdle__() {
    _status &= ~STATUS_STATE_MASK;
}

void inline my23LC1024::__setStateRead__() {
    _status &= ~STATUS_STATE_MASK;
    _status |= STATUS_STATE_READ_BIT;
}

void inline my23LC1024::__setStateWrite__() {
    _status &= ~STATUS_STATE_MASK;
    _status |= STATUS_STATE_WRITE_BIT;
}

void inline my23LC1024::__setStatusHold__() {
    _status |= STATUS_HOLD_MASK;
}

void inline my23LC1024::__clearStatusHold__() {
    _status &= ~STATUS_HOLD_MASK;
}

void inline my23LC1024::__setSPIPinModes__() {
    gpio_set_dir(_misoPin, GPIO_IN);
    gpio_set_dir(_mosiPin, GPIO_OUT);
}

void inline my23LC1024::__setSDIPinModes__(const bool isOutput) {
    if (isOutput == true) {
        gpio_set_dir(_misoPin, GPIO_OUT);
        gpio_set_dir(_mosiPin, GPIO_OUT);
    } else {
        gpio_set_dir(_misoPin, GPIO_IN);
        gpio_set_dir(_mosiPin, GPIO_IN);
    }
}

void inline my23LC1024::__setSQIPinModes__(const bool isOutput) {
    if (isOutput == true) {
        gpio_set_dir(_misoPin, GPIO_OUT);
        gpio_set_dir(_mosiPin, GPIO_OUT);
        gpio_set_dir(_holdPin, GPIO_OUT);
        gpio_set_dir(_sio2Pin, GPIO_OUT);
    } else {
        gpio_set_dir(_misoPin, GPIO_IN);
        gpio_set_dir(_mosiPin, GPIO_IN);
        gpio_set_dir(_holdPin, GPIO_IN);
        gpio_set_dir(_sio2Pin, GPIO_IN);
    }
}

void my23LC1024::__resetComms__() {
    if (_useHWSPI == true) {
        __selectChip__();
        uint8_t resetCommand = RSTIO_INSTRUCTION;
        spi_write_blocking(_spiPort, &resetCommand, 1);
        __deselectChip__();
    } else {
        gpio_put(_mosiPin, true);           // Set mosi high
        gpio_set_dir(_misoPin, GPIO_OUT);    // set Miso output
        gpio_put(_mosiPin, true);           // set miso high.
        if (_holdPin != MY_NOT_A_PIN) {
            gpio_put(_holdPin, true);           // set hold pin high if exists.
        }
        if (_sio2Pin != MY_NOT_A_PIN) {
            gpio_put(_sio2Pin, true);           // set sio2 pin high if exists.
        }
    // Select and toggle clock 2 times to clear from SQI:
        __selectChip__();
        for (uint8_t i=0; i<2; i++) {
            gpio_put(_sckPin, true);    // Raise clock
            sleep_us(1);                // Wait for a microsecond.
            gpio_put(_sckPin, false);   // lower clock
            sleep_us(1);                // wait for a microsecond.
        }
        __deselectChip__();
    // Select and toggle clock 4 times to clear from SDI:
        __selectChip__();
        for (uint8_t i=0; i<4; i++) {
            gpio_put(_sckPin, true);    // Raise clock
            sleep_us(1);                // Wait for a microsecond.
            gpio_put(_sckPin, false);   // lower clock
            sleep_us(1);                // wait for a microsecond.
        }
        __deselectChip__();
    // Reset miso line to input:
        gpio_set_dir(_misoPin, GPIO_IN);
    }
}

int32_t my23LC1024::__HWSPIRead__(uint8_t *buffer, const int32_t length) {
    return spi_read_blocking(_spiPort, 0x00, buffer, length);
}

int32_t my23LC1024::__HWSPIWrite__(const uint8_t *buffer, const int32_t length) {
    return spi_write_blocking(_spiPort, buffer, length);
}

int32_t my23LC1024::__SPIRead__(uint8_t *buffer, const int32_t length) {

}

int32_t my23LC1024::__SPIWrite__(const uint8_t *buffer, const int32_t length) {

}

uint8_t my23LC1024::__readByte__() {
    if (_useHWSPI == true) {
        uint8_t value;
        __HWSPIRead__(&value, 1);
        return value;
    } else {
        uint8_t value;
        switch (_commsMode) {
            case COMM_MODE_SPI:
                value = 0x00;
                break;
            case COMM_MODE_SDI:
                value = 0x00;
                break;
            case COMM_MODE_SQI:
                value = 0x00;
                break;
        }
        return value;
    }
}

int32_t my23LC1024::__readBuffer__(uint8_t *buffer, int32_t length) {
    if (_useHWSPI == true) {
        return __HWSPIRead__(buffer, length);
    } else {
        int32_t bytesRead;
        switch (_commsMode) {
            case COMM_MODE_SPI:
                bytesRead = 0x00;
                break;
            case COMM_MODE_SDI:
                bytesRead = 0x00;
                break;
            case COMM_MODE_SQI:
                bytesRead = 0x00;
                break;
        }
        return bytesRead;
    }
}

void my23LC1024::__writeByte__(const uint8_t value) {
    if (_useHWSPI == true) {
        uint8_t buffer[1] = { value };
        __HWSPIWrite__(buffer, 1);
    } else {
        switch (_commsMode) {
        case COMM_MODE_SPI:
            ;
            break;
        case COMM_MODE_SDI:
            ;
            break;
        case COMM_MODE_SQI:
            ;
            break;
        }
    }
}

int32_t my23LC1024::__writeBuffer__(const uint8_t *buffer, const int32_t length) {
    if (_useHWSPI == true) {
        return __HWSPIWrite__(buffer, length);
    } else {
        int32_t bytesSent;
        switch (_commsMode) {
            case COMM_MODE_SPI:
                bytesSent = 0x00;
                break;
            case COMM_MODE_SDI:
                bytesSent = 0x00;
                break;
            case COMM_MODE_SQI:
                bytesSent = 0x00;
                break;        
        }
        return bytesSent;
    }
}

uint8_t my23LC1024::__readModeRegister__() {
    __selectChip__();
    __writeByte__(RDMR_INSTRUCTION);
    uint8_t value = __readByte__();
    __deselectChip__();
    return value;
}

void my23LC1024::__writeModeRegister__(const uint8_t value) {
    __selectChip__();
    uint8_t buffer[2] = { WRMR_INSTRUCTION, value };
    __writeBuffer__(buffer, 2);
    __deselectChip__();
}
#endif