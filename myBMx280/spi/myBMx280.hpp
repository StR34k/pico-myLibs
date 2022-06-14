#ifndef MY_BMx280_H
#define MY_BMx280_H

#include <hardware/spi.h>
#include <stdlib.h>

class myBMx280 {

	public:
	/* Constants: */
		// Read bit for reading regitsters:
		static const uint8_t READ_BIT = 0x80;
		// General Registers :
		static const uint8_t REG_ID_ADDR			= 0xD0;	// RETURNS UINT8
		static const uint8_t REG_RESET_ADDR			= 0XE0;	// RETURNS UINT8
		static const uint8_t REG_CTRL_HUM_ADDR		= 0XF2;	// RETURNS UINT8
		static const uint8_t REG_STATUS_ADDR		= 0XF3;	// RETURNS UINT8
		static const uint8_t REG_CTRL_MEAS_ADDR		= 0XF4;	// RETURNS UINT8
		static const uint8_t REG_CONFIG_ADDR		= 0XF5;	// RETURNS UINT8
		// Temperature calibration registers:
		static const uint8_t REG_TEMP_DIG_START_ADDR	= 0x88;
		static const uint8_t REG_TEMP_DIG_LEN			= 6;
		// Pressure calibration registers:
		// Humidity calibration registers:
		// Chipsets:
		static const uint8_t CHIPMODEL_BMP280 = 0x58; // No humidity.
		static const uint8_t CHIPMODEL_BME280 = 0x60; // Has humidity.
		// Default Sensor config:
		static const uint8_t DEFAULT_CONFIG 		= 0b10000000;// Default config, Standby 500us, filter off, spi3wire off.
		static const uint8_t DEFAULT_HUM_SETTINGS 	= 0b00000001;// Default humidity settings, hum OSR x1.
		static const uint8_t DEFAULT_MEAS_SETTINGS	= 0b00100101;// Default settings, temp OSR x1, pres OSR x1, forced mode.

	/* Public functions: */
		myBMx280(spi_inst_t *spiObj, uint8_t csPin, uint8_t clkPin, uint8_t misoPin, uint8_t mosiPin);
		bool initialize(const uint8_t config=DEFAULT_CONFIG, const uint8_t measCtrl=DEFAULT_MEAS_SETTINGS, const uint8_t humCtrl=DEFAULT_HUM_SETTINGS);
		uint8_t readID();

	private:
	/* Private Variables: */
		// SPI Variables:
		spi_inst_t *_spiObj;
		uint8_t _csPin;
		uint8_t _sckPin;
		uint8_t _misoPin;
		uint8_t _mosiPin;
		// state variables:
		bool _have_humidity = false;

		// Calibration variables:
		uint16_t	_dig_t1;
		int16_t 	_dig_t2;
		int16_t		_dig_t3;

	/* Private Functions: */
		void __selectChip__();
		void __deselectChip__();
		void __readRegisters__(const uint8_t reg, uint8_t *buffer, const uint8_t length);
		void __writeRegister__(const uint8_t reg, const uint8_t value);
		void __readTemperatureCalibration__();

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

bool myBMx280::initialize(const uint8_t config, const uint8_t measCtrl, const uint8_t humCtrl) {
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
// Read Calibrations:
	__readTemperatureCalibration__();

// Write configureation:
	__writeRegister__(REG_CONFIG_ADDR, config);
	if (_have_humidity == true) {
		__writeRegister__(REG_CTRL_HUM_ADDR, humCtrl);
	}
	__writeRegister__(REG_CTRL_MEAS_ADDR, measCtrl);
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
	__deselectChip__();
}

void myBMx280::__writeRegister__(const uint8_t reg, const uint8_t value) {
	__selectChip__();
	uint8_t buffer[2] = { reg, value };
	spi_write_blocking(_spiObj, buffer, 2);
	__deselectChip__();
}

void myBMx280::__readTemperatureCalibration__() {
	uint8_t buffer[REG_TEMP_DIG_LEN];
	__readRegisters__(REG_TEMP_DIG_START_ADDR, buffer, REG_TEMP_DIG_LEN);
	_dig_t1 = ((uint16_t)buffer[1] << 8) | (uint16_t)buffer[0];
	_dig_t2 = ((int16_t)buffer[3] << 8) | (int16_t)buffer[2];
	_dig_t3 = ((int16_t)buffer[5] << 8) | (int16_t)buffer[4];
	printf("t1: %i t2: %i t3: %i", _dig_t1, _dig_t2, _dig_t3);
	
}
#endif