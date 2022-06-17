/*
 * My bit banged SPI.
 *      SPI info and code from http://www.gammon.com.au/spi
 */

#ifndef MY_BIT_BANG_SPI_H
#define MY_BIT_BANG_SPI_H

#include <hardware/gpio.h>
#include <pico/time.h>

namespace mySPIMaster {
/* Constants */
    const uint8_t   MODE_0 = 0x00;    // CPOL = 0     CPHA = 0
    const uint8_t   MODE_1 = 0x01;    // CPOL = 0     CPHA = 1
    const uint8_t   MODE_2 = 0x02;    // CPOL = 1     CPHA = 0
    const uint8_t   MODE_3 = 0x03;    // CPOL = 1     CPHA = 1
    const bool      MSB_FIRST = true;
    const bool      LSB_FIRST = false;  
/* Variables */
    uint8_t     _sck;       // SPI Clock pin
    uint8_t     _miso;      // SPI RX pin
    uint8_t     _mosi;      // SPI TX pin
    bool        _cpol;      // Clock polarity.
    bool        _cpha;      // Clock phase. (when to sample bit.)
    bool        _msbFirst;  // Bit order. true = MSB first, false = LSB first. Defaults to MSB first.
    uint64_t    _delayUS;   // Amount of time to delay before transitions in microseconds. Defaults to 1.
/* Functions */
    bool __validateMode(const uint8_t mode) {
        if (mode > MODE_3) { return false; }
        return true;
    }

    void __setIdle__() {
        gpio_put(_sck, _cpol);
        gpio_put(_mosi, !_cpha);
    }

    void initialize(const uint8_t sck, const uint8_t miso, const uint8_t mosi, const uint8_t mode=MODE_0, 
                        const bool msbFirst=true, uint64_t delayUS=1) {
    // Store pins and states for later.
        _sck = sck;
        _miso = miso;
        _mosi = mosi;
        _msbFirst = msbFirst;
        _delayUS = delayUS;
        switch (mode) {
            case MODE_0:
                _cpol = false;
                _cpha = false;
                break;
            case MODE_1:
                _cpol = false;
                _cpha = true;
            case MODE_2:
                _cpol = true;
                _cpha = false;
            case MODE_3:
                _cpol = true;
                _cpha = true;
        }
    // Set gpio Functions:
        gpio_set_function(_sck,  GPIO_FUNC_SIO);
        gpio_set_function(_miso, GPIO_FUNC_SIO);
        gpio_set_function(_mosi, GPIO_FUNC_SIO);
    // Set gpio Directions:
        gpio_set_dir(_sck,  GPIO_OUT);
        gpio_set_dir(_miso, GPIO_IN);
        gpio_set_dir(_mosi, GPIO_OUT);
    // Set Idle states:
        __setIdle__();
    
    }

    uint8_t transfer(const uint8_t value) {
        uint8_t mask;
        uint8_t readValue = 0x00;

        // Set read / write mask
        for (uint8_t i=0; i<8; i++) {
            if (_msbFirst == true) {
                mask = (1 << (7-i));
            } else {
                mask = (1<<i);
            }
        // Set MOSI:
            gpio_put(_mosi, (bool)(value & mask));
        // Toggle clock :
            gpio_put(_sck, !_cpol);
        // Sample MISO on first edge, CPHA = false;
            if (_cpha == false) {
                if (gpio_get(_miso) == true){
                    readValue |= mask;
                }
            }
        // Delay:
            sleep_us(_delayUS);
        // Toggle clock:
            gpio_put(_sck, _cpol);
        // Sample MISO on second edge, CPHA = true:
            if (_cpha == true) {
                if (gpio_get(_miso) == true) {
                    readValue |= mask;
                }
            }
        // Delay:
            sleep_us(_delayUS);
        }
        return readValue;
    }
};

#endif