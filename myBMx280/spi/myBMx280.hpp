#ifndef MY_BMx280_H
#define MY_BMx280_H

#include <hardware/spi.h>

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
		// Data registers:
		static const uint8_t REG_DATA_START_ADDR	= 0XF7;
		static const uint8_t BME280_DATA_LEN		= 8;
		static const uint8_t BMP280_DATA_LEN		= 6;
		// Chipsets:
		static const uint8_t CHIPMODEL_BMP280 = 0x58; // No humidity.
		static const uint8_t CHIPMODEL_BME280 = 0x60; // Has humidity.
		// Oversampling rate, off turns reading off.:
		static const uint8_t OSR_OFF	= 0X00;
		static const uint8_t OSR_X1		= 0X01;
		static const uint8_t OSR_X2		= 0X02;
		static const uint8_t OSR_X4		= 0X03;
		static const uint8_t OSR_X8		= 0X04;
		static const uint8_t OSR_X16	= 0X05;
		// Chip Modes:
		static const uint8_t MODE_SLEEP		= 0X00;
		static const uint8_t MODE_FORCED	= 0X01;
		static const uint8_t MODE_NORMAL	= 0X03;
		// Standby Times:		
		static const uint8_t STANDBY_TIME_500US		= 0x00;
		static const uint8_t STANDBY_TIME_62500US	= 0x01;
		static const uint8_t STANDBY_TIME_125MS		= 0x02;
		static const uint8_t STANDBY_TIME_250MS		= 0x03;
		static const uint8_t STANDBY_TIME_50MS		= 0x04;
		static const uint8_t STANDBY_TIME_1000MS	= 0x05;
		static const uint8_t STANDBY_TIME_10MS		= 0x06;
		static const uint8_t STANDBY_TIME_20MS		= 0x07;
		static const uint8_t STANDBY_TIME_2000MS	= 0x06; // For BMP280 only.
		static const uint8_t STANDBY_TIME_4000MS	= 0x07; // For BMP280 only.
		// Filter Modes:
		static const uint8_t FILTER_OFF	= 0x00;
		static const uint8_t FILTER_2	= 0x01;
		static const uint8_t FILTER_4	= 0x02;
		static const uint8_t FILTER_8	= 0x03;
		static const uint8_t FILTER_16	= 0x04;
		// SPI Modes:
		static const uint8_t SPI_3WIRE_FALSE	= 0x00;
		static const uint8_t SPI_3WIRE_TRUE		= 0x01;
		// Reset Value:
		static const uint8_t RESET_VALUE = 0xB6;
		// Default Sensor config:
		static const uint8_t DEFAULT_CONFIG 		= 0b10000000;// Default config, Standby 500us, filter off, spi3wire off.
		static const uint8_t DEFAULT_HUM_SETTINGS 	= 0b00000001;// Default humidity settings, hum OSR x1.
		static const uint8_t DEFAULT_MEAS_SETTINGS	= 0b00100101;// Default settings, temp OSR x1, pres OSR x1, forced mode.
		// Config Masks:
		static const uint8_t CONFIG_STANDBY_MASK	= 0b11100000;
		static const uint8_t CONFIG_FILTER_MASK		= 0b00011100;
		static const uint8_t CONFIG_3WIRE_MASK		= 0b00000001;
		// Measure Control Masks:
		static const uint8_t MEAS_TEMP_OSR_MASK		= 0b11100000;
		static const uint8_t MEAS_PRES_OSR_MASK		= 0b00011100;
		static const uint8_t MEAS_MODE_MASK			= 0b00000011;
		// Humidity Control Masks:
		static const uint8_t HUM_OSR_MASK			= 0b00000111;
	/* Public variables: */
		bool have_humidity = false;
		bool forced_mode = false;
		float temperature = 0.0;
		float pressure = 0.0;
		float humidity = 0.0;

	/* Public functions: */
		myBMx280(spi_inst_t *spiObj, const uint8_t csPin, const uint8_t clkPin, const uint8_t misoPin, const uint8_t mosiPin);
		uint8_t getID();								// Get the chip ID byte.
		uint8_t getStatus();							// Get the status byte.
		uint8_t getConfig();							// Get the configuration byte.
		void	setConfig(const uint8_t value);			// Set the configuration byte.
		uint8_t getMeasCtrl();							// Get the measure control byte.
		void	setMeasCtrl(const uint8_t value);		// Set the Measure control byte.
		uint8_t getHumCtrl();							// Get the humidity control byte.
		void	setHumCtrl(const uint8_t value);		// Set the Humidity control byte.
		uint8_t	getStandbyTime();						// Set the inactive duration in normal mode. (standby time)
		bool	setStandbyTime(const uint8_t value);	// Set the inactive duration in normal mode. (standby time) Returns True if set, False if not set.
		uint8_t getFilter();							// Get the IIR filter setting.
		bool	setFilter(const uint8_t value);			// Set the IIR filter setting. Return true if set, false if not set.
		uint8_t getTemperatureOSR();					// Get the temperature oversampling rate
		bool	setTemperatureOSR(const uint8_t value);	// Set the temperature oversampling rate. Returns true if set, false if not set.
		uint8_t getPressureOSR();						// Get the pressure oversampling rate.
		bool	setPressureOSR(const uint8_t value);	// Set the pressure oversampling rate. Returns true if set, false if not set.
		uint8_t getHumidityOSR();						// Get the humidity oversampling rate.
		bool 	setHumidityOSR(const uint8_t value);	// Set the humidity oversampling rate. returns true if set, false if not set.
		uint8_t getMode();								// get the operating mode of the chip.
		bool 	setMode(const uint8_t value);			// Set the operating mode of the chip.
		void reset();
		bool initialize(const uint8_t config=DEFAULT_CONFIG, const uint8_t measCtrl=DEFAULT_MEAS_SETTINGS, const uint8_t humCtrl=DEFAULT_HUM_SETTINGS);
		void update();

	private:
	/* Private Variables: */
		// SPI Variables:
		spi_inst_t *_spiObj;
		uint8_t _csPin;
		uint8_t _sckPin;
		uint8_t _misoPin;
		uint8_t _mosiPin;
		// Temperature calibration variables:
		uint16_t	_dig_t1;
		int16_t 	_dig_t2, _dig_t3;
		// Pressure calibration variables:
		uint16_t	_dig_p1;
		int16_t		_dig_p2, _dig_p3, _dig_p4, _dig_p5;
		int16_t		_dig_p6, _dig_p7, _dig_p8, _dig_p9;
		// Humidity calibration variables:
		uint8_t		_dig_h1, _dig_h3;
		int16_t		_dig_h2, _dig_h4, _dig_h5;
		int8_t		_dig_h6;
		// "tfine" calibration value:
		int32_t _t_fine;

	/* Private Functions: */
		void __selectChip__();
		void __deselectChip__();
		void __readRegisters__(const uint8_t reg, uint8_t *buffer, const uint8_t length);
		void __writeRegister__(const uint8_t reg, const uint8_t value);
		uint8_t __readUint8__(const uint8_t reg);
		int8_t __readInt8__(const uint8_t reg);
		uint16_t __readUint16__(const uint8_t reg);
		int16_t __readInt16__(const uint8_t reg);
		void __readTemperatureCalibration__();
		void __readPressureCalibration__();
		void __readHumidityCalibration__();
		int32_t __calculateTFine__(const int32_t rawTemperature);
		float __calculateTemperature__();
		float __calculatePressure__(const int32_t rawPressure);
		float __calculateHumidity__(const int32_t rawHumidity);

};

myBMx280::myBMx280(spi_inst_t *spiObj, const uint8_t csPin, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin) {
    _spiObj = spiObj;
    _csPin = csPin;
    _sckPin = sckPin;
    _misoPin = misoPin;
    _mosiPin = mosiPin;
}

uint8_t myBMx280::getID() {
	uint8_t ID;
	__readRegisters__(REG_ID_ADDR, &ID, 1);
	return ID;
}

uint8_t myBMx280::getStatus() {
	uint8_t status;
	__readRegisters__(REG_STATUS_ADDR, &status, 1);
	return status;
}

uint8_t myBMx280::getConfig() {
	uint8_t config;
	__readRegisters__(REG_CONFIG_ADDR, &config, 1);
	return config;
}

void myBMx280::setConfig(const uint8_t value) {
	uint8_t writeValue = value & ~CONFIG_3WIRE_MASK; // Make sure 3 wire mode is disabled.
	__writeRegister__(REG_CONFIG_ADDR, writeValue);
}

uint8_t myBMx280::getMeasCtrl() {
	uint8_t measCtrl;
	__readRegisters__(REG_CTRL_MEAS_ADDR, &measCtrl, 1);
	return measCtrl;
}

void myBMx280::setMeasCtrl(const uint8_t value) {
	uint8_t mode = value & MEAS_MODE_MASK;
	if (mode == 0x01 or mode == 0x02) {
		forced_mode = true;
	} else {
		forced_mode = false;
	}
	__writeRegister__(REG_CTRL_MEAS_ADDR, value);
}

uint8_t myBMx280::getHumCtrl() {
	uint8_t humCtrl = 0x00;
	if (have_humidity == true) {
		__readRegisters__(REG_CTRL_HUM_ADDR, &humCtrl, 1);
	}
	return humCtrl;
}

void myBMx280::setHumCtrl(const uint8_t value) {
	if (have_humidity == true) {
		uint8_t measCtrl = getMeasCtrl();
		__writeRegister__(REG_CTRL_HUM_ADDR, value);
		__writeRegister__(REG_CTRL_MEAS_ADDR, measCtrl);
	}

}

void myBMx280::reset() {
	__writeRegister__(REG_RESET_ADDR, RESET_VALUE);
}

uint8_t myBMx280::getStandbyTime(){
	uint8_t standbyTime = getConfig() & CONFIG_STANDBY_MASK;
	standbyTime = standbyTime >> 5;
	return standbyTime;
}

bool myBMx280::setStandbyTime(const uint8_t value) {
	if (value > STANDBY_TIME_20MS) {
		return false;
	}
	uint8_t config = getConfig();
	config &= ~CONFIG_STANDBY_MASK;
	config |= (value << 5);
	setConfig(config);
	return true;
}

uint8_t myBMx280::getFilter() {
	uint8_t filter = getConfig() & CONFIG_FILTER_MASK;
	filter = filter >> 2;
	return filter;
}

bool myBMx280::setFilter(const uint8_t value) {
	if (value > FILTER_16) {
		return false;
	}
	uint8_t config = getConfig();
	config &= ~CONFIG_FILTER_MASK;
	config |= (value << 2);
	setConfig(config);
	return true;
}

uint8_t myBMx280::getTemperatureOSR() {
	uint8_t osr = getMeasCtrl() & MEAS_TEMP_OSR_MASK;
	osr = osr >> 5;
	return osr;
}

bool myBMx280::setTemperatureOSR(const uint8_t value) {
	if (value > OSR_X16) { return false; }
	uint8_t measCtrl = getMeasCtrl();
	measCtrl &= ~MEAS_TEMP_OSR_MASK;
	measCtrl |= (value << 5);
	setMeasCtrl(measCtrl);
	return true;
}

uint8_t myBMx280::getPressureOSR() {
	uint8_t osr = getMeasCtrl() & MEAS_PRES_OSR_MASK;
	osr = osr >> 2;
	return osr;
}

bool myBMx280::setPressureOSR(const uint8_t value) {
	if (value > OSR_X16) { return false; }
	uint8_t measCtrl = getMeasCtrl();
	measCtrl &= ~MEAS_PRES_OSR_MASK;
	measCtrl |= (value << 2);
	setMeasCtrl(measCtrl);
	return true;
}

uint8_t myBMx280::getHumidityOSR() {
	uint8_t osr = getHumCtrl() & HUM_OSR_MASK;
	return osr;
}

bool myBMx280::setHumidityOSR(const uint8_t value) {
	if (value > OSR_X16) { return false; }
	uint8_t humCtrl = getHumCtrl();
	humCtrl &= ~HUM_OSR_MASK;
	humCtrl |= value;
	setHumCtrl(humCtrl);
	return true;
}

uint8_t myBMx280::getMode() {
	uint8_t mode = getMeasCtrl() & MEAS_MODE_MASK;
	return mode;
}

bool myBMx280::setMode(const uint8_t value) {
	if (value > MODE_NORMAL) { return false; }
	uint8_t measCtrl = getMeasCtrl();
	measCtrl &= ~MEAS_MODE_MASK;
	measCtrl |= value;
	setMeasCtrl(measCtrl);
	return true;
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
	uint8_t chip_id = getID();
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
// Determine Mode:
	uint8_t mode = measCtrl & 0x03;
	if (mode == 0x01 or mode == 0x02) {
		forced_mode = true;
	} else {
		forced_mode = false;
	}
// Write configureation:
	__writeRegister__(REG_CONFIG_ADDR, config);
	if (have_humidity == true) {
		__writeRegister__(REG_CTRL_HUM_ADDR, humCtrl);
	}
	__writeRegister__(REG_CTRL_MEAS_ADDR, measCtrl);
    return true;
}

void myBMx280::update() {
// If in forced mode, force the reading:
	if (forced_mode == true) {
		uint8_t measCtrl = getMeasCtrl();
		setMeasCtrl(measCtrl);
		sleep_us(1);
	}
// Read the data from the chip:
	uint8_t dataBuffer[BME280_DATA_LEN] = {0x00};
	if (have_humidity == true) {
		__readRegisters__(REG_DATA_START_ADDR, dataBuffer, BME280_DATA_LEN);
	} else {
		__readRegisters__(REG_DATA_START_ADDR, dataBuffer, BMP280_DATA_LEN);
	}
// Set raw data values:
	int32_t rawPressure, rawTemperature, rawHumidity;
	rawPressure = ((int32_t)dataBuffer[0] << 12) | ((int32_t)dataBuffer[1] << 4) | ((int32_t)dataBuffer[2] >> 4);
	rawTemperature = ((int32_t)dataBuffer[3] << 12) | ((int32_t)dataBuffer[4] << 4) | ((int32_t)dataBuffer[5] >> 4);
	if (have_humidity == true) {
		rawHumidity = ((int32_t)dataBuffer[6] << 8) | (int32_t)dataBuffer[7];
	}
// Calculate "tFine" calibration value:
	_t_fine = __calculateTFine__(rawTemperature);
// Calculate final values:
	temperature = __calculateTemperature__();
	pressure = __calculatePressure__(rawPressure);
	if (have_humidity == true) {
		humidity = __calculateHumidity__(rawHumidity);
	}
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

void myBMx280::__readTemperatureCalibration__() {
	_dig_t1 = __readUint16__(REG_TEMP_DIG_T1_ADDR);
	_dig_t2 = __readInt16__(REG_TEMP_DIG_T2_ADDR);
	_dig_t3 = __readInt16__(REG_TEMP_DIG_T3_ADDR);
}

void myBMx280::__readPressureCalibration__() {
	_dig_p1 = __readUint16__(REG_PRES_DIG_P1_ADDR);
	_dig_p2 = __readInt16__(REG_PRES_DIG_P2_ADDR);
	_dig_p3 = __readInt16__(REG_PRES_DIG_P3_ADDR);
	_dig_p4 = __readInt16__(REG_PRES_DIG_P4_ADDR);
	_dig_p5 = __readInt16__(REG_PRES_DIG_P5_ADDR);
	_dig_p6 = __readInt16__(REG_PRES_DIG_P6_ADDR);
	_dig_p7 = __readInt16__(REG_PRES_DIG_P7_ADDR);
	_dig_p8 = __readInt16__(REG_PRES_DIG_P8_ADDR);
	_dig_p9 = __readInt16__(REG_PRES_DIG_P9_ADDR);
}

void myBMx280::__readHumidityCalibration__() {
	_dig_h1 = __readUint8__(REG_HUM_DIG_H1_ADDR);
	_dig_h2 = __readInt16__(REG_HUM_DIG_H2_ADDR);
	_dig_h3 = __readUint8__(REG_HUM_DIG_H3_ADDR);
	_dig_h6 = __readInt8__(REG_HUM_DIG_H6_ADDR);
	uint8_t buffer[3];
	__readRegisters__(REG_HUM_DIG_H4_ADDR, buffer, 3);
	_dig_h4 = ((int16_t)buffer[0] <<4) | (0x000F & (int16_t)buffer[1]);
	_dig_h5 = ((int16_t)buffer[2] <<4) | (((int16_t)buffer[1] >> 4) & 0x000F);
}

int32_t myBMx280::__calculateTFine__(const int32_t rawTemperature) {
	int32_t var1, var2;
	var1 = ((((rawTemperature >> 3) - ((int32_t) _dig_t1 << 1))) * ((int32_t) _dig_t2)) >> 11;
	var2 = (((((rawTemperature >> 4) - ((int32_t) _dig_t1)) * ((rawTemperature >> 4) - ((int32_t) _dig_t1))) >> 12) * ((int32_t) _dig_t3)) >> 14;
	return (var1 + var2);
}

float myBMx280::__calculateTemperature__() {
	return (float)((_t_fine * 5 + 128) >> 8) / 100.0; // return Deg Celcius
}

float myBMx280::__calculatePressure__(const int32_t rawPressure) {
	int32_t var1, var2;
	uint32_t pres;
	var1 = (_t_fine >> 1) - (int32_t)64000;
	var2 = (((var1>>2) * (var1>>2)) >> 11) * ((int32_t)_dig_p6);
	var2 = var2 + ((var1*((int32_t)_dig_p5)) << 1);
	var2 = (var2>>2)+(((int32_t)_dig_p4)<<16);
	var1 = (((_dig_p3 * (((var1>>2) * (var1>>2)) >> 13)) >>3) 
					+ ((((int32_t)_dig_p2 * var1)>>1))) >>18;
	var1 = ((((32768+var1))*((int32_t)_dig_p1))>>15);
	if (var1 == 0) { return 0.0; } // Avoid div by 0.
	pres = (((uint32_t)(((int32_t)1048576)-rawPressure) - (var2 >> 12))) * 3125;
	if (pres < 0x80000000) {
		pres = (pres<<1) / ((uint32_t)var1);
	} else {
		pres = ( pres / (uint32_t)var1) * 2;
	}
	var1 = (((int32_t)_dig_p9) * ((int32_t)(((pres>>3) * (pres>>3))>>13)))>>12;
	var2 = (((int32_t)(pres>>2)) * ((int32_t)_dig_p8))>>13;
	pres = (uint32_t)((int32_t)pres + ((var1 + var2 + _dig_p7) >> 4));
	return (float)pres; // Return Pa
}

float myBMx280::__calculateHumidity__(const int32_t rawHumidity) {
	int32_t var1;
	var1 = (_t_fine -(int32_t)76800);
   var1 = (((((rawHumidity << 14) - (((int32_t)_dig_h4) << 20) - (((int32_t)_dig_h5) * var1)) +
   ((int32_t)16384)) >> 15) * (((((((var1 * ((int32_t)_dig_h6)) >> 10) * (((var1 *
   ((int32_t)_dig_h3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
   ((int32_t)_dig_h2) + 8192) >> 14));
   var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)_dig_h1)) >> 4));
   var1 = (var1 < 0 ? 0 : var1);
   var1 = (var1 > 419430400 ? 419430400 : var1);
   return (float)((uint32_t)(var1>>12)) / 1024.0; // Return relative humidity %
}
#endif