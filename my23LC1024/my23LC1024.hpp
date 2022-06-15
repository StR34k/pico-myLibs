
#ifndef MY23LC1024_H
#define MY23LC1024_H
#include "../myStandardDefines.hpp"
#include <pico/stdlib.h>
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
        // Valid address Mask, and length:
        static const uint32_t VALID_ADDRESS_MASK    = 0x0001FFFF; // 0b 0000 0000 0000 0001 1111 1111 1111 1111
        static const uint32_t MAX_LENGTH = VALID_ADDRESS_MASK +1; // Set the max length.
    /* Constructor: */
        // HW SPI Constructors:
        my23LC1024(spi_inst_t *spiPort, const uint8_t csPin, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin);
        my23LC1024(spi_inst_t *spiPort, const uint8_t csPin, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin, const uint8_t holdPin);
        // Bit Bang comms Constructors:
        my23LC1024(const uint8_t csPin, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin);
        my23LC1024(const uint8_t csPin, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin, const uint8_t holdPin);
        my23LC1024(const uint8_t csPin, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin, const uint8_t holdPin, const uint8_t sio2Pin);
    /* Functions: */
        bool        initialize(const uint8_t commsMode);                        // Return true if comms verified, false if not.
        bool        startRead(const uint32_t address);                          // Start the read process.
        uint8_t     read();                                                     // Read a single byte returns byte read.
        uint32_t    readBuffer(uint8_t *buffer, uint32_t length);               // Read a series of bytes. returns number of bytes read.
        bool        startWrite(const uint32_t address);                         // start the write process.
        void        write(const uint8_t value);                                 // write a single byte.
        uint32_t    writeBuffer(const uint8_t *value, const uint32_t length);   // Write a series of bytes.
        bool        stop();                                                     // stop the read / write process.


    private:
    /* Variables: */
        spi_inst_t *_spiPort;   // The spi object from the sdk
        uint8_t _csPin;         // The cs pin
        uint8_t _sckPin;        // the sck pin
        uint8_t _misoPin;       // the rx pin
        uint8_t _mosiPin;       // the tx pin
        uint8_t _holdPin;       // the hold pin.
        uint8_t _sio2Pin;       // the sio2 pin.
        uint8_t _commsMode;     // the comms mode.
        bool _useHWSPI = true;  // if we are useing hw spi
    /* Functions: */
        void        __selectChip__();                                               // Lower the cs line.
        void        __deselectChip__();                                             // raise the cs line.        
        void        __resetComms__();                                               // Reset the comms mode to SPI.
        uint16_t    __HWSPIRead__(uint8_t *buffer, const uint16_t length);          // Use hw spi to read.
        uint16_t    __HWSPIWrite__(const uint8_t *buffer, const uint16_t length);   // Use hw spi to write.
        void        __writeByte__(const uint8_t value);                             // Select the right write function and transfer a single byte.
        uint16_t    __writeBuffer__(const uint8_t *buffer, const uint16_t length);  // Select the right write function and transfer a series of bytes. Returns the number of bytes written.
        uint8_t     __readByte__();                                                 // Select the right read function and read a single byte. returns byte read.
        uint16_t    __readBuffer__(uint8_t *buffer, const uint16_t length);         // Select the right read function and read a series of bytes. returns number of bytes read.
        uint8_t     __readModeRegister__();                                         // Return the value of the mode register.
        void        __writeModeRegister__(uint8_t value);                           // Set the value of the mode register.

};

/*
 * Constructors:
 */
// HW SPI Constructors:
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
    _spiPort = _spiPort;
    _csPin = csPin;
    _sckPin = sckPin;
    _misoPin = misoPin;
    _mosiPin = mosiPin;
    _holdPin = holdPin;
    _sio2Pin = MY_NOT_A_PIN;
    _useHWSPI = true;
}
// Bit Bang comms Constructors:
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
/*
 * Public functions:
 */
bool my23LC1024::initialize(const uint8_t commsMode) {
// Store comms mode:
    _commsMode = commsMode;
// Do some basic checks:
    if (commsMode == COMM_MODE_SQI) { // SQI requires hold and sio2 to be defined.
        if (_holdPin == MY_NOT_A_PIN) { return false; } 
        if (_sio2Pin == MY_NOT_A_PIN) { return false; }
    }
// Set pin funtions, modes, and initial states for spi pins:
    gpio_set_function(_csPin,   GPIO_FUNC_SIO); // Set cs as gpio.
    gpio_set_dir(_csPin, GPIO_OUT); // Set cs as output.
    gpio_put(_csPin, true); // Set cs High.
    if (_useHWSPI == true) {
        uint32_t baud = spi_init(_spiPort, 1000*20000); // Init spi at 20 Mhz
        printf("Baud: %i\n", baud);
        gpio_set_function(_sckPin,  GPIO_FUNC_SPI); // set sck as spi.
        gpio_set_function(_misoPin, GPIO_FUNC_SPI); // set miso as spi.
        gpio_set_function(_mosiPin, GPIO_FUNC_SPI); // set mosi as spi.
    } else {
        gpio_set_function(_sckPin,   GPIO_FUNC_SIO); // set sck as gpio.
        gpio_set_function(_misoPin,  GPIO_FUNC_SIO); // Set miso as gpio. 
        gpio_set_function(_mosiPin,  GPIO_FUNC_SIO); // set mosi as gpio.
        gpio_set_dir(_sckPin, GPIO_OUT); // Set sck as output.
        gpio_set_dir(_misoPin, GPIO_IN); // Set miso as input.
        gpio_set_dir(_mosiPin, GPIO_OUT); // Set mosi as output.
        gpio_put(_sckPin, false); // set sck Low
        gpio_put(_mosiPin, false); // set mosi low
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

    // uint8_t command = RDMR_INSTRUCTION;
    // uint8_t mode;
    // __selectChip__();
    // spi_write_blocking(_spiPort, &command, 1);
    // spi_read_blocking(_spiPort, 0x00, &mode, 1);
    // __deselectChip__();
    // printf("Got mode: 0x%x\n", mode);
    return true;
// Initialze comms:
    __resetComms__();
    switch (_commsMode)
    {
    case COMM_MODE_SDI:
        ;
        break;
    case COMM_MODE_SQI:
        ;
        break;    
    }
// Verify mode, which verifies comms:
    // bool verified = false;
    // uint8_t mode = __readModeRegister__();
    // printf("mode: 0x%x\n", mode);
    // return true;
}

/*
 * Private functions:
 */
void my23LC1024::__selectChip__() { gpio_put(_csPin, false); }

void my23LC1024::__deselectChip__() { gpio_put(_csPin, true); }

void my23LC1024::__resetComms__() {
// Only reset comms if in bit bang mode:
    if (_useHWSPI == false) {
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

uint16_t my23LC1024::__HWSPIRead__(uint8_t *buffer, const uint16_t length) {
    int32_t bytesRead = spi_read_blocking(_spiPort, 0x00, buffer, length);
    return (uint16_t)bytesRead;
}

uint16_t my23LC1024::__HWSPIWrite__(const uint8_t *buffer, const uint16_t length) {
    __breakpoint();
    uint8_t value = spi_is_writable(_spiPort);
    printf("isWriteable: %i\n", value);
    int32_t bytesSent = spi_write_blocking(_spiPort, buffer, length);
    // __breakpoint();
    return (uint16_t)bytesSent;
}

uint8_t my23LC1024::__readByte__() {
    if (_useHWSPI == true) {
        uint8_t buffer[1];
        __HWSPIRead__(buffer, 1);
        return buffer[0];
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

uint16_t my23LC1024::__readBuffer__(uint8_t *buffer, uint16_t length) {
    if (_useHWSPI == true) {
        int32_t bytesRead = __HWSPIRead__(buffer, length);
        return (uint16_t)bytesRead;
    } else {
        uint16_t value;
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

uint16_t my23LC1024::__writeBuffer__(const uint8_t *buffer, const uint16_t length) {
    if (_useHWSPI == true) {
        int32_t bytesSent = __HWSPIWrite__(buffer, length);
        return (uint16_t)bytesSent;
    } else {
        uint16_t value;
        switch (_commsMode) {
            case COMM_MODE_SPI:
                value = 0x00;
                break;
            case COMM_MODE_SDI:
                value = 0x00;
                break;
            case COMM_MODE_SQI:
                /*code */
                break;        
        }
        return value;
    }
}

uint8_t my23LC1024::__readModeRegister__() {
    __selectChip__();
    __breakpoint();
    __writeByte__(RDMR_INSTRUCTION);
    uint8_t value = __readByte__();
    __deselectChip__();
    return value;
}
#endif