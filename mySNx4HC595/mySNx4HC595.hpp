/* Datasheet: https://www.ti.com/lit/ds/symlink/sn74hc595.pdf?ts=1647947714454&ref_url=https%253A%252F%252Fwww.google.com%252F
*/

#ifndef MY_SNX4HC595_H
#define MY_SNX4HC595_H

#include <hardware/gpio.h>

#include "../myStandardDefines.hpp"
#include "../myErrorCodes.hpp"
#include "../myHelpers.hpp"

class my595 {
    public:
    /* Constants: */
        // Errors:
        static const int16_t NO_ERROR = MY_NO_ERROR;
        static const int16_t ERROR_ENABLE_NOT_DEFINED = MY_ERROR_MY595_ENABLE_NOT_DEFINED;  // -400 : Enable pin not defined.
        static const int16_t ERROR_CLEAR_NOT_DEFINED = MY_ERROR_MY595_CLEAR_NOT_DEFINED;    // -401 : Clear pin not defined.


    /* Constructors: */
        my595(const uint8_t latch, const uint8_t clock, const uint8_t data) : _latchPin (latch),
                    _clkPin (clock), _dataPin(data) {}
        my595(const uint8_t latch, const uint8_t clock, const uint8_t data, const uint8_t enable) :
                    _latchPin (latch), _clkPin (clock), _dataPin (data), _enablePin (enable) {}
        my595(const uint8_t latch, const uint8_t clock, const uint8_t data, const uint8_t enable,
                    const uint8_t clear) : _latchPin (latch), _clkPin (clock), _dataPin (data),
                    _enablePin (enable), _clearPin (clear) {}
    /* Functions: */
        int16_t initialize(const bool enable=true);
        void    startWrite(); // Lowers the latch.
        void    writeBit(const bool value); // Shift in one bit.
        void    writeByte(const uint8_t value, const bool bitOrder=MSB_FIRST); // Shift in one byte.
        void    stopWrite(); // raise the latch.
        int16_t clear();
        int16_t setEnable(const bool value);

    private:
        const uint8_t _latchPin = MY_NOT_A_PIN;
        const uint8_t _clkPin = MY_NOT_A_PIN;
        const uint8_t _dataPin = MY_NOT_A_PIN;
        const uint8_t _enablePin = MY_NOT_A_PIN;
        const uint8_t _clearPin = MY_NOT_A_PIN;

};

int16_t my595::initialize(const bool enable) {
// Check pins:
    if (myHelpers::isPin(_latchPin) == false) { return MY_INVALID_PIN; }
    if (myHelpers::isPin(_dataPin) == false) { return MY_INVALID_PIN; }
    if (myHelpers::isPin(_clkPin) == false) { return MY_INVALID_PIN; }
// Init Pins:
    gpio_init(_latchPin);
    gpio_init(_clkPin);
    gpio_init(_dataPin);
    gpio_set_dir(_latchPin, GPIO_OUT);
    gpio_set_dir(_clkPin, GPIO_OUT);
    gpio_set_dir(_dataPin, GPIO_OUT);
    gpio_put(_latchPin, true);
    gpio_put(_clkPin, false);
    gpio_put(_dataPin, false);
// Init clear pin
    if (_clearPin != MY_NOT_A_PIN) {
        gpio_init(_clearPin);
        gpio_set_dir(_clearPin, GPIO_OUT);
        gpio_put(_clearPin, true);
    }
// Init enable pin
    if (_enablePin != MY_NOT_A_PIN) {
        gpio_init(_enablePin);
        gpio_set_dir(_enablePin, GPIO_OUT);
        gpio_put(_enablePin, !enable);
    } else if (enable == false) {
        return ERROR_ENABLE_NOT_DEFINED;
    }
    return NO_ERROR;
}

void my595::startWrite() {
    gpio_put(_latchPin, false); // lower the latch.
}

void my595::writeBit(const bool value) {
    gpio_put(_dataPin, value); // Set data pin.
    gpio_put(_clkPin, true);
    gpio_put(_clkPin, false);
}

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
void my595::stopWrite() {
    gpio_put(_latchPin, true);
}

int16_t my595::clear() {
    if (myHelpers::isPin(_clearPin) == false) { return ERROR_CLEAR_NOT_DEFINED; }
    gpio_put(_clearPin, false);
    gpio_put(_clearPin, true);
    return NO_ERROR;
}

int16_t my595::setEnable(const bool value) {
    if (myHelpers::isPin(_enablePin) == false) { return ERROR_ENABLE_NOT_DEFINED; }
    gpio_put(_enablePin, !value);
    return NO_ERROR;
}

#endif