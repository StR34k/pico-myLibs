#ifndef MY_BMx280_H
#define MY_BMx280_H

#include <hardware/spi.h>
#include <stdlib.h>
class myBMx280 {

	public:
	// Read bit for reading regitsters:
		static const uint8_t READ_BIT = 0x80;
	// Registers :
		static const uint8_t REG_ID_ADDR		= 0xD0;	// RETURNS UINT8
		static const uint8_t REG_RESET_ADDR		= 0XE0;	// RETURNS UINT8
		static const uint8_t REG_CTRL_HUM_ADDR	= 0XF2;	// RETURNS UINT8
		static const uint8_t REG_STATUS_ADDR	= 0XF3;	// RETURNS UINT8
		static const uint8_t REG_CTRL_MEAS_ADDR	= 0XF4;	// RETURNS UINT8
		static const uint8_t REG_CONFIG_ADDR	= 0XF5;	// RETURNS UINT8

	// Chipsets:
		static const uint8_t CHIPMODEL_BMP280 = 0x58; // No humidity.
		static const uint8_t CHIPMODEL_BME280 = 0x60; // Has humidity.

		myBMx280(spi_inst_t *spiObj, uint8_t csPin, uint8_t clkPin, uint8_t misoPin, uint8_t mosiPin);
		bool initialize();
		uint8_t readID();

	private:
		spi_inst_t *_spiObj;
		uint8_t _csPin;
		uint8_t _sckPin;
		uint8_t _misoPin;
		uint8_t _mosiPin;
		bool _have_humidity = false;
		void __selectChip__();
		void __deselectChip__();
		void __readRegisters__(const uint8_t reg, uint8_t *buffer, const uint8_t length);

};

myBMx280::myBMx280(spi_inst_t *spiObj, uint8_t csPin, uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin) {
    _spiObj = spiObj;
    _csPin = csPin;
    _sckPin = sckPin;
    _misoPin = misoPin;
    _mosiPin = mosiPin;
	
}

uint8_t myBMx280::readID() {
	uint8_t ID;
	uint8_t address = REG_ID_ADDR | READ_BIT;
	__readRegisters__(address, &ID, 1);
	return ID;
}

bool myBMx280::initialize() {
    spi_init(_spiObj, 1000*20000); // Init at 20 Mhz
    gpio_set_function(_csPin, GPIO_FUNC_SIO);  // CS pin is GPIO
    gpio_set_function(_sckPin, GPIO_FUNC_SPI); // SCK pin is SPI
    gpio_set_function(_misoPin, GPIO_FUNC_SPI);// MISO pin is SPI
    gpio_set_function(_mosiPin, GPIO_FUNC_SPI);// MOSI pin is SPI
// Setup CS Pin as OUTPUT HIGH.
    gpio_set_dir(_csPin, GPIO_OUT);
    gpio_put(_csPin, true);
// Read Chip ID and set have humidity:
	uint8_t chip_id = readID();
	printf("id 0x%x", chip_id);
	switch (chip_id) {
		case CHIPMODEL_BME280:
			_have_humidity = true;
			break;
		case CHIPMODEL_BMP280:
			_have_humidity = false;
			break;
		default:
			return false;
	}

    return true;
}

void inline myBMx280::__selectChip__() {
	gpio_put(_csPin, false);
}

void inline myBMx280::__deselectChip__() {
	gpio_put(_csPin, true);
}

void myBMx280::__readRegisters__(const uint8_t reg, uint8_t *buffer, const uint8_t length) {
	uint8_t address = reg | READ_BIT;
	__selectChip__();
	spi_write_blocking(_spiObj, &address, 1);
	spi_read_blocking(_spiObj, 0, buffer, length);
}

#endif