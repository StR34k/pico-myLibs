/**
 * @file mySNx4HC165.hpp
 * @author Peter Nearing (pnearing@protonmail.com)
 * @brief Class to handle a 165 piso shift register.
 * @version 0.1
 * @date 2022-06-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MY_SNX4HC165_H
#define MY_SNX4HC165_H

#include <hardware/gpio.h>

#include "../myStandardDefines.hpp"
#include "../myErrorCodes.hpp"
#include "../myHelpers.hpp"

/**
 * @brief Class to handle a 165 shift register.
 * Class to handle a 165 shift register. Datasheet: https://www.ti.com/lit/ds/symlink/sn74hc165.pdf
 */
class my165{
    public:
/* Constants: */
    /**
     * @brief No error. Value 0
     * No error occured.
     */
    static const int16_t NO_ERROR = MY_NO_ERROR;
    /**
     * @brief Enable not defined. Value -350
     * The enable pin was not defined at time of operation.
     */
    static const int16_t ERROR_ENABLE_NOT_DEFINED = MY_ERROR_MY165_ENABLE_NOT_DEFINED;
    /**
     * @brief Clock already enabled. Value -351
     * The clock inhibit is already in enabled state when operation was requested.
     */
    static const int16_t ERROR_ALREADY_ENABLED = MY_ERROR_MY165_ALREADY_ENABLED;
    /**
     * @brief Clock already disabled. Value -352
     * The clock inhibit is alread in disabled state when opertation was requested.
     */
    static const int16_t ERROR_ALREADY_DISABLED = MY_ERROR_MY165_ADREADY_DISABLED;

/* Constructors: */
    /**
     * @brief Construct a new my165 object. No enable pin.
     * 
     * @param clkPin Clock pin.
     * @param dataPin Data pin.
     * @param loadPin Load pin.
     */
    my165(const uint8_t clkPin, const uint8_t dataPin, const uint8_t loadPin) : _clkPin (clkPin),
                _dataPin(dataPin), _loadPin (loadPin) {}
    /**
     * @brief Construct a new my165 object. With enable  pin.
     * 
     * @param clkPin Clock pin.
     * @param dataPin Data pin.
     * @param loadPin Load pin.
     * @param enablePin Enable pin.
     */
    my165(const uint8_t clkPin, const uint8_t dataPin, const uint8_t loadPin, const uint8_t enablePin) :
                _clkPin (clkPin), _dataPin (dataPin), _loadPin (loadPin), _enablePin (enablePin) {}
/* Functions: */
    /**
     * @brief Initialize the gpio pins.
     * 
     * @return int16_t returns 0 (NO_ERROR) for initialize okay, negaitve for error code.
     */
    int16_t initialize();
    /**
     * @brief Start a read.
     * Loads values from parralell input and sets up to tranfer.
     */
    void startRead();
    /**
     * @brief Read a single bit.
     * Reads a bit and clocks in the next bit.
     * @return Bit read.
     */
    bool readBit();
    /**
     * @brief Read a byte.
     * Reads a byte from the shift register.
     * @param bitOrder True for MSB first(default), false for LSB first.
     * @return uint8_t Byte read.
     */
    uint8_t readByte(const bool bitOrder=MSB_FIRST);
    /**
     * @brief Stop a read.
     * Resets all pins to idle states.
     */
    void stop();
    /**
     * @brief Set the Clock Inhibit. (enable)
     * Set the clock inhibit pin, returns 0 (NO_ERROR) for set okay, negative for error code.
     * @param value True to enable clock, false to disable.
     * @return int16_t return 0(NO_ERROR) for okay, negative for error code.
     */
    int16_t setEnable(const bool value);
    /**
     * @brief Get the clock inhibit. (enable)
     * 
     * @return true Clock is enabled.
     * @return false Clock is disabled.
     */
    bool    getEnable();

    private:
/* Pins: */
    const uint8_t _clkPin = MY_NOT_A_PIN;
    const uint8_t _dataPin = MY_NOT_A_PIN;
    const uint8_t _loadPin = MY_NOT_A_PIN;
    const uint8_t _enablePin = MY_NOT_A_PIN;
/*Variables: */
    bool _enable = true;
};


int16_t my165::initialize(){
    if (myHelpers::isPin(_clkPin) == false) { return MY_INVALID_PIN; }
    if (myHelpers::isPin(_dataPin) == false) { return MY_INVALID_PIN; }
    if (myHelpers::isPin(_loadPin) == false) { return MY_INVALID_PIN; }
// Init clock data and load pins:
    gpio_init(_clkPin);
    gpio_init(_dataPin);
    gpio_init(_loadPin);
    gpio_set_dir(_clkPin, GPIO_OUT);
    gpio_set_dir(_dataPin, GPIO_IN);
    gpio_set_dir(_loadPin, GPIO_OUT);
    gpio_put(_clkPin, false);
    gpio_put(_loadPin, true);
    gpio_set_pulls(_dataPin, true, false);
// Init Enable pin if defined:
    if (myHelpers::isPin(_enablePin) == true) {
        gpio_init(_enablePin);
        gpio_set_dir(_enablePin, GPIO_OUT);
        gpio_put(_enablePin, false);
    }

    return NO_ERROR;
}

void my165::startRead() {
// Pulse load pin to load values:
    sleep_ms(500);
    gpio_put(_loadPin, false);
    sleep_ms(500);
    gpio_put(_loadPin, true);
    gpio_put(_clkPin, true);
}

uint8_t my165::readByte(const bool bitOrder) {
    uint8_t value = 0x00;
    __breakpoint();
    for (uint8_t i=0; i<9; i++) {

        gpio_put(_clkPin, true); // Set clock high.
        sleep_ms(500);
    // Read bit:
        if (bitOrder == MSB_FIRST) { // MSB first
            value <<= 1;
            if (gpio_get(_dataPin) == true) { value |= 0x01; }
        } else {    // LSB first.
            value >>= 1;
            if (gpio_get(_dataPin) == true) { value |= 0x80; }
        }
        gpio_put(_clkPin, false); // Set clock low.
        sleep_ms(500);
        __breakpoint();
    }
    return value;
}

void my165::stop() {
    gpio_put(_clkPin, false);
    gpio_put(_loadPin, true);
}
#endif