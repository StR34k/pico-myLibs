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
		static const uint8_t REG_TEMP_DIG_T1_ADDR	= 0X88;	// UINT16
		static const uint8_t REG_TEMP_DIG_T2_ADDR	= 0X8A;	// INT16
		static const uint8_t REG_TEMP_DIG_T3_ADDR	= 0X8C;	// INT16
		// Pressure calibration registers:
		static const uint8_t REG_PRES_DIG_P1_ADDR	= 0X8E;	// UINT16
		static const uint8_t REG_PRES_DIG_P2_ADDR	= 0X90;	// INT16
		static const uint8_t REG_PRES_DIG_P3_ADDR	= 0X92;	// INT16
		static const uint8_t REG_PRES_DIG_P4_ADDR	= 0X94;	// INT16
		static const uint8_t REG_PRES_DIG_P5_ADDR	= 0X96;	// INT16
		static const uint8_t REG_PRES_DIG_P6_ADDR	= 0X98;	// INT16
		static const uint8_t REG_PRES_DIG_P7_ADDR	= 0X9A;	// INT16
		static const uint8_t REG_PRES_DIG_P8_ADDR	= 0X9C;	// INT16
		static const uint8_t REG_PRES_DIG_P9_ADDR	= 0X9E;	// INT16
		// Humidity calibration registers:
		static const uint8_t REG_HUM_DIG_H1_ADDR	= 0XA1;	// UINT8
		static const uint8_t REG_HUM_DIG_H2_ADDR	= 0XE1;	// INT16
		static const uint8_t REG_HUM_DIG_H3_ADDR	= 0XE3;	// UINT8
		static const uint8_t REG_HUM_DIG_H4_ADDR	= 0XE4;	// INT16 * REGISTER E5 CONTAINS DATA FOR H5, TREAT SPECIAL
		static const uint8_t REG_HUM_DIG_H5_ADDR	= 0XE5;	// INT16 * REGISTER E5 CONTAINS DATA FOR H4, TREAT SPECIAL.
		static const uint8_t REG_HUM_DIG_H6_ADDR	= 0XE7;	// INT8
		// Chipsets:
		static const uint8_t CHIPMODEL_BMP280 = 0x58; // No humidity.
		static const uint8_t CHIPMODEL_BME280 = 0x60; // Has humidity.
		// Default Sensor config:
		static const uint8_t DEFAULT_CONFIG 		= 0b10000000;// Default config, Standby 500us, filter off, spi3wire off.
		static const uint8_t DEFAULT_HUM_SETTINGS 	= 0b00000001;// Default humidity settings, hum OSR x1.
		static const uint8_t DEFAULT_MEAS_SETTINGS	= 0b00100101;// Default settings, temp OSR x1, pres OSR x1, forced mode.
	/* Public variables: */
		bool have_humidity = false;
		float temperature;
		float pressure;
		float humidity;


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
		void __readPressureCalibration__();
		void __readHumidityCalibration__();
		uint8_t __readUint8__(const uint8_t reg);
		int8_t __readInt8__(const uint8_t reg);
		uint16_t __readUint16__(const uint8_t reg);
		int16_t __readInt16__(const uint8_t reg);
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
			have_humidity = true;
			break;
		case CHIPMODEL_BMP280:
			have_humidity = false;
			break;
		default:
			return false;
	}
// Read Calibrations:
	__readTemperatureCalibration__();
	__readPressureCalibration__();
	if (have_humidity == true) {
		__readHumidityCalibration__();
	}
// Write configureation:
	__writeRegister__(REG_CONFIG_ADDR, config);
	if (have_humidity == true) {
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
	_dig_t1 = __readUint16__(REG_TEMP_DIG_T1_ADDR);
	_dig_t2 = __readInt16__(REG_TEMP_DIG_T2_ADDR);
	_dig_t3 = __readInt16__(REG_TEMP_DIG_T3_ADDR);
	printf ("t1:%i t2:%i t3:%i", _dig_t1, _dig_t2, _dig_t3);
	while (true) {;}
}

void myBMx280::__readPressureCalibration__() {

}

void myBMx280::__readHumidityCalibration__() {

}

uint8_t myBMx280::__readUint8__(const uint8_t reg) {
	uint8_t value;
	__readRegisters__(reg, &value, 1);
	return value;
}

int8_t myBMx280::__readInt8__(const uint8_t reg) {
	uint8_t value;
	__readRegisters__(reg, &value, 1);
	return (int8_t)value;
}

uint16_t myBMx280::__readUint16__(const uint8_t reg) {
	uint8_t buffer[2];
	__readRegisters__(reg, buffer, 2);
	return (uint16_t)(((uint16_t)buffer[1] << 8) | (uint16_t)buffer[0]);
}

int16_t myBMx280::__readInt16__(const uint8_t reg) {
	uint8_t buffer[2];
	__readRegisters__(reg, buffer, 2);
	return (int16_t)(((int16_t)buffer[1] << 8) | (int16_t)buffer[0]);
}

#endif