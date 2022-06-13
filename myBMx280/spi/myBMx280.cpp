#include "myBMx280.hpp"
#include <hardware/spi.h>
#include <pico/stdlib.h>

myBMx280::myBMx280(spi_inst_t *spiObj, uint8_t csPin, uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin) {
    _spiObj = spiObj;
    _csPin = csPin;
    _sckPin = sckPin;
    _misoPin = misoPin;
    _mosiPin = mosiPin;
}

myBMx280::initialize() {
    spi_init(_spiObj, 1000*20000); // Init at 20 Mhz
    gpio_set_function(_csPin, GPIO_FUNC_SIO);  // CS pin is GPIO
    gpio_set_function(_sckPin, GPIO_FUNC_SPI); // SCK pin is SPI
    gpio_set_function(_misoPin, GPIO_FUNC_SPI);// MISO pin is SPI
    gpio_set_function(_mosiPin, GPIO_FUNC_SPI);// MOSI pin is SPI
// Setup CS Pin as OUTPUT HIGH.
    gpio_set_dir(_csPin, GPIO_OUT);
    gpio_put(_csPin, true);
}
