/**
 * @file myBMx280.hpp
 * @author Peter Nearing (pnearing@protonmail.com)
 * @brief Class to handle BMx280 chips in i2c mode.
 * @version 0.1
 * @date 2022-06-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef MY_BMx280_H
#define MY_MBx280_H

#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <pico/time.h>
#include <pico/error.h>

#include "../../myStandardDefines.hpp"
#include "../../myErrorCodes.hpp"
#include "../../myHelpers.hpp"

class myBMx280 {
    public:
	/*
     * ##################### Constants: ##############################
     */
// Errors:
        /**
         * @brief No error. Value 0.
         * No error occured.
         */
        static const int16_t NO_ERROR = MY_NO_ERROR;
        /**
         * @brief Timeout error. Value -7.
         * A timeout occured.
         */
        static const int16_t ERROR_TIMEOUT = MY_TIMEOUT;
        /**
         * @brief Invalid SDA pin. Value -450.
         * Invalid SDA pin was provided.
         */
        static const int16_t ERROR_INVALID_SDA_PIN = MY_ERROR_MYI2C_INVALID_SDA_PIN;
        /**
         * @brief Invalid SCL pin. Value -451.
         * Invalid SCL pin was provided.
         */
        static const int16_t ERROR_INVALID_SCL_PIN = MY_ERROR_MYI2C_INVALID_SCL_PIN;
        /**
         * @brief Already initialized. Value -452.
         * The chip is already initialized.
         */
        static const int16_t ERROR_INITIALIZED = MY_ERROR_MYI2C_INITIALIZED;
        /**
         * @brief Invalid I2C address. Value -500.
         * The provided I2C address is not a valid address.
         */
        static const int16_t ERROR_INVALID_ADDRESS = MY_ERROR_MYBMx280_INVALID_ADDRESS;
        /**
         * @brief No response. Value -501.
         * The I2C address didn't respond.
         */
        static const int16_t ERROR_NO_RESPONSE = MY_ERROR_MYBMx280_NO_RESPONSE;
        /**
         * @brief Invalid response. Value -502.
         * The chip provided an invalid respone.
         */
        static const int16_t ERROR_INVALD_RESPONSE = MY_ERROR_MYBMx280_INVALID_RESPONSE;
		/**
		 * @brief No humidity. Value -503.
		 * The chip doesn't have humidity.
		 */
		static const int16_t ERROR_NO_HUMIDITY = MY_ERROR_MYBMx280_NO_HUMIDITY;
		/**
		 * @brief Invalid standby time. Value -504.
		 * An invalid standby time was provided.
		 */
		static const int16_t ERROR_INVALID_STANDBY = MY_ERROR_MYBMx280_INVALID_STANDBY;
		/**
		 * @brief Invalid filter. Value -505.
		 * An invalid filter value was passed.
		 */
		static const int16_t ERROR_INVALID_FILTER = MY_ERROR_MYBMx280_INVALID_FILTER;
		/**
		 * @brief Invalid Over Sample Rate. Value -506.
		 * An invalid over sample rate value was provided.
		 */
		static const int16_t ERROR_INVALID_OSR = MY_ERROR_MYBMx280_INVALID_OSR;
		/**
		 * @brief Invalid mode. Value -507.
		 * An invalid mode value was provided.
		 */
		static const int16_t ERROR_INVALID_MODE = MY_ERROR_MYBMx280_INVALID_MODE;
// Chipsets:
		/**
		 * @brief Chip model bmp280. No humidity
		 * 
		 */
		static const uint8_t CHIPMODEL_BMP280 = 0x58; // No humidity.
		/**
		 * @brief Chip mode BME 280. Has humidity.
		 * 
		 */
		static const uint8_t CHIPMODEL_BME280 = 0x60; // Has humidity.
// Oversampling rate, off turns reading off.:
		/**
		 * @brief Over sampling off.
		 * @note turns off reading.
		 */
		static const uint8_t OSR_OFF	= 0X00;
		/**
		 * @brief Oversampling x1
		 * 
		 */
		static const uint8_t OSR_X1		= 0X01;
		/**
		 * @brief Oversampling x2
		 * 
		 */
		static const uint8_t OSR_X2		= 0X02;
		/**
		 * @brief Oversampling x4
		 * 
		 */
		static const uint8_t OSR_X4		= 0X03;
		/**
		 * @brief Oversampling x8
		 * 
		 */
		static const uint8_t OSR_X8		= 0X04;
		/**
		 * @brief Oversampling x16
		 * 
		 */
		static const uint8_t OSR_X16	= 0X05;
// Chip Modes:
		/**
		 * @brief Chip mode Sleep
		 * 
		 */
		static const uint8_t MODE_SLEEP		= 0X00;
		/**
		 * @brief Chip mode forced.
		 * This is the default mode.
		 */
		static const uint8_t MODE_FORCED	= 0X01;
		/**
		 * @brief Chip mode normal.
		 * 
		 */
		static const uint8_t MODE_NORMAL	= 0X03;
// Standby Times:		
		/**
		 * @brief Standby time of 500 microseconds.
		 * 
		 */
		static const uint8_t STANDBY_TIME_500US		= 0x00;
		/**
		 * @brief Standby time of 6.25 milliseconds
		 * 
		 */
		static const uint8_t STANDBY_TIME_62500US	= 0x01;
		/**
		 * @brief Standby time of 125 milliseconds
		 * 
		 */
		static const uint8_t STANDBY_TIME_125MS		= 0x02;
		/**
		 * @brief Standby time of 250 milliseconds.
		 * 
		 */
		static const uint8_t STANDBY_TIME_250MS		= 0x03;
		/**
		 * @brief Standby time of 50 milliseconds
		 * 
		 */
		static const uint8_t STANDBY_TIME_50MS		= 0x04;
		/**
		 * @brief Standby time of 1000 milliseconds (1 second)
		 * 
		 */
		static const uint8_t STANDBY_TIME_1000MS	= 0x05;
		/**
		 * @brief Standby time of 10 milliseconds.
		 * @note BME280 only.
		 */
		static const uint8_t STANDBY_TIME_10MS		= 0x06;
		/**
		 * @brief Standby time of 20 milliseconds.
		 * @note BME280 only.
		 */
		static const uint8_t STANDBY_TIME_20MS		= 0x07;
		/**
		 * @brief Standby time of 2000 milliseconds (2 seconds.)
		 * @note BMP280 only.
		 */
		static const uint8_t STANDBY_TIME_2000MS	= 0x06; // For BMP280 only.
		/**
		 * @brief Standby time of 4000 milliseconds (4 seconds)
		 * @note BMP280 only.
		 */
		static const uint8_t STANDBY_TIME_4000MS	= 0x07; // For BMP280 only.
// Filter Modes:
		/**
		 * @brief Filter Off
		 * 
		 */
		static const uint8_t FILTER_OFF	= 0x00;
		/**
		 * @brief Filter at 2 samples.
		 * 
		 */
		static const uint8_t FILTER_2	= 0x01;
		/**
		 * @brief Filter at 4 samples.
		 * 
		 */
		static const uint8_t FILTER_4	= 0x02;
		/**
		 * @brief Filter at 8 samples
		 * 
		 */
		static const uint8_t FILTER_8	= 0x03;
		/**
		 * @brief Filter at 16 samples
		 * 
		 */
		static const uint8_t FILTER_16	= 0x04;
// Config Masks:
		/**
		 * @brief Standby time mask of the config byte
		 * 
		 */
		static const uint8_t CONFIG_STANDBY_MASK	= 0b11100000;
		/**
		 * @brief Filter mask of the config byte.
		 * 
		 */
		static const uint8_t CONFIG_FILTER_MASK		= 0b00011100;
		/**
		 * @brief 3 wire communications mode of the config byte.
		 * @note This library will fail in 3 wire mode.
		 */
		static const uint8_t CONFIG_3WIRE_MASK		= 0b00000001;
// Measure Control Masks:
		/**
		 * @brief Temperature OSR mask of the measCtrl byte
		 * 
		 */
		static const uint8_t MEAS_TEMP_OSR_MASK		= 0b11100000;
		/**
		 * @brief Pressure OSR mask of the measCtrl byte
		 * 
		 */
		static const uint8_t MEAS_PRES_OSR_MASK		= 0b00011100;
		/**
		 * @brief Measure mode mask of the measCtrl byte
		 * 
		 */
		static const uint8_t MEAS_MODE_MASK			= 0b00000011;
// Humidity Control Masks:
		/**
		 * @brief Humidity OSR mask of the humCtrl byte.
		 * 
		 */
		static const uint8_t HUM_OSR_MASK			= 0b00000111;
	/*
     * ########################## Public variables: ################################
     */
		/**
		 * @brief True if the sensor has humidity
		 * 
		 */
		bool have_humidity = false;
		/**
		 * @brief True if sensor is in forced mode.
		 * 
		 */
		bool forced_mode = false;
		/**
		 * @brief Temperature in DegC
		 * 
		 */
		float temperature = 0.0;
		/**
		 * @brief Pressure in Pa
		 * 
		 */
		float pressure = 0.0;
		/**
		 * @brief Humidity in RH%
		 * 
		 */
		float humidity = 0.0;
/*
 * ########################## Public functions: ############################
 */
		
        /**
         * @brief Construct a new my BMx280 object in I2C mode.
         * Create an new bmx280 object in I2C Mode.
         * @param i2cPort I2C port instance. 
         * @param sdaPin SDA pin.
         * @param sclPin SCL pin.
         * @param address Address of the BMx280.
         */
        myBMx280(i2c_inst_t *i2cPort, const uint8_t sdaPin, const uint8_t sclPin, const uint8_t address);
		/**
		 * @brief Get the chip id byte.
		 * Returns the Chip ID or error code on timeout or no response.
		 * @return uint8_t Positive is the chip id, one of MODEL_BME280 or MODEL_BMP280, or negative for error code.
		 */
		int16_t getID();								// Get the chip ID byte.
		/**
		 * @brief Get the Status byte
		 * Returns the status byte, unless an error during comms happens, at which
		 * point it returns an error code.
		 * @return int16_t Positive is the status byte, negative for error code.
		 */
		int16_t getStatus();							// Get the status byte.
		/**
		 * @brief Get the Config Byte.
		 * Returns the config byte, unless an error during comms happens, at which
		 * point it returns an error code.
		 * @return int16_t Positive is the config byte, negative for error code.
		 */
		int16_t getConfig();							// Get the configuration byte.
		/**
		 * @brief Set the Config byte.
		 * Sets the config byte, returns 0 (NO_ERROR) if successful, otherwise if an
		 * error during comms occurs, an error code is returned.
		 * @note When setting the config byte, the comms mode is ignored in I2C mode.
		 * @param value Config byte
		 * @return int16_t Returns 0 (NO_ERROR) if set okay, negative for error code.
		 */
		int16_t	setConfig(const uint8_t value);			// Set the configuration byte.
		/**
		 * @brief Get the Meas Ctrl Byte
		 * Gets the Meas Ctrl byte, returns positive for the byte, otherwise if an
		 * error occurs during comms, returns an error code.
		 * @return int16_t Positive: Meas ctrl byte, Negative: error code.
		 */
		int16_t getMeasCtrl();							// Get the measure control byte.
		/**
		 * @brief Set the Meas Ctrl byte
		 * Sets the Meas Ctrl byte, returns 0 (NO_ERROR) if set okay, otherwise if an error
		 * during comms occurs, an error code is returned.
		 * @param value measCtrl value.
		 * @return int16_t Returns 0 (NO_ERROR) for set okay, negative for error code.
		 */
		int16_t	setMeasCtrl(const uint8_t value);		// Set the Measure control byte.
		/**
		 * @brief Get the Hum Ctrl byte
		 * Gets the Humidity Control byte. Positive values (including zero) are the
		 * control byte, otherwise if an error during comms occurs, an error code is 
		 * returned.
		 * @return int16_t Positive is the Hum Ctrl byte, negative for error code.
		 */
		int16_t getHumCtrl();							// Get the humidity control byte.
		/**
		 * @brief Set the Hum Ctrl byte
		 * Sets the Humidity Control byte. Returns 0 (NO_ERROR) if set okay, returns an
		 * error code if there is no humidity, or an error during comms occurs.
		 * @param value humCtrl value to set.
		 * @return int16_t Returns 0 (NO_ERROR) for set okay, negative for error code.
		 */
		int16_t	setHumCtrl(const uint8_t value);		// Set the Humidity control byte.
		/**
		 * @brief Get the Standby Time
		 * Get the inactive duration in normal mode. (standby time). Returns positive, including
		 * zero for the standby times, otherwise if an error during comms occurs, an error code is
		 * returned.
		 * @return int16_t returns one of the STANDBY_TIME_ values.
		 */
		int16_t	getStandbyTime();						// Get the inactive duration in normal mode. (standby time)
		/**
		 * @brief Set the Standby Time
		 * Set the inactive duration in normal mode. (standby time) Returns 0
		 * (NO_ERROR) for set okay, otherwise if an invalid mode is supplied or
		 * an error in comms occurs an error code is returned.
		 * @param value Standby time value, one of the STANDBY_TIME_ values.
		 * @return Returns 0 (NO_ERROR) if set okay, negative for error code.
		 */
		int16_t	setStandbyTime(const uint8_t value);	// Set the inactive duration in normal mode. (standby time) Returns True if set, False if not set.
		/**
		 * @brief Get the Filter 
		 * Get the IIR filter setting. Returns positive for the filter value, and
		 * if an error occurs during comms returns an error code.
		 * @return int16_t Positive, returns the filter, one of the FILTER_ values, negative for error code.
		 */
		int16_t getFilter();							// Get the IIR filter setting.
		/**
		 * @brief Set the Filter
		 * Set the IIR filter setting, returns 0 (NO_ERROR) for set okay, otherwise if
		 * a comms error occurs, or an invlaid filter is provided, an error code is returned.
		 * @param value one of the FILTER_ values.
		 * @return int16_t Returns 0 (NO_ERROR) for set okay, negaitve for error code.
		 */
		int16_t	setFilter(const uint8_t value);			// Set the IIR filter setting. Return true if set, false if not set.
		/**
		 * @brief Get the Temperature OSR
		 * Get temperature oversampling rate. Returns positive (including zero) for the oversample
		 * rate, otherwise if a comms error occurs, an error code is returned.
		 * @return int16_t Returns positive,is one of the OSR_ values. Negative for error code.
		 */
		int16_t getTemperatureOSR();					// Get the temperature oversampling rate
		/**
		 * @brief Set the Temperature OSR 
		 * Set the temperature oversampling rate.Returns 0(NO_ERROR) if set okay, otherwise,
		 * if an invalid OSR, or a comms error occurs, an error code is returned.
		 * @param value one of the OSR_ values.
		 * @return int16_t Returns 0 for set okay, negative for error code.
		 */
		int16_t	setTemperatureOSR(const uint8_t value);	// Set the temperature oversampling rate. Returns true if set, false if not set.
		/**
		 * @brief Get the Pressure OSR
		 * Get the pressure oversampling rate. Returns postivie (including zero) for the OSR value, otherwise if
		 * an comms error occurs, an error code is returned.
		 * @return int16_t Postive: oversampling rate, one of OSR_ values, negative for error code.
		 */
		int16_t getPressureOSR();						// Get the pressure oversampling rate.
		/**
		 * @brief Set the Pressure OSR
		 * Set the pressure oversampling rate. Returns 0 (NO_ERROR) if set okay, otherwise
		 * if a comms error, or an invalid rate is supplied, an error code is returned.
		 * @param value The oversampling rate, one of the OSR_ values.
		 * @return int16_t Returns 0 (NO_ERROR) if set okay, negative for error code.
		 */
		int16_t	setPressureOSR(const uint8_t value);	// Set the pressure oversampling rate. Returns true if set, false if not set.
		/**
		 * @brief Get the Humidity OSR
		 * Get the humidity oversampling rate. Returns postive (including zero) for OSR value,
		 * otherwise if a comms error occurs, an error code is returned.
		 * @return int16_t Positive is the oversampling rate, one of the OSR_ values, negative for error code.
		 */
		int16_t getHumidityOSR();						// Get the humidity oversampling rate.
		/**
		 * @brief Set the Humidity OSR
		 * Set the humidity oversampling rate. Returns 0 (NO_ERROR) if set okay, otherwise if an invalid
		 * value is passed, or if a comms error occurs, an error code is returned.
		 * @param value The oversampling rate, one of the OSR_ values.
		 * @return int16_t Returns 0 (NO_ERROR) for set okay, negative for error code.
		 */
		int16_t	setHumidityOSR(const uint8_t value);	// Set the humidity oversampling rate. returns true if set, false if not set.
		/**
		 * @brief Get the Mode.
		 * Return the operating mode of the chip, Positive values (including zero) is the mode, 
		 * otherwise if a comms error occurs, an error code is returned.
		 * @return int16_t Positive is the mode one of the MODE_ values. Negative is an error code.
		 */
		int16_t getMode();								// get the operating mode of the chip.
		/**
		 * @brief Set the Mode.
		 * Set the operating mode of the chip. Returns 0 (NO_ERROR) if set oaky, otherwise if
		 * the mode is invalid, or a comms error occured an error code is returned.
		 * @param value The operating mode, one of the MODE_ values.
		 * @return Returns 0 (NO_ERROR) if set okay, negative for error code.
		 */
		int16_t 	setMode(const uint8_t value);			// Set the operating mode of the chip.
		/**
		 * @brief reset the chip.
		 * Resets the chip to default values. Retuns 0 (NO_ERROR) if reset okay, otherwise,
		 * if a comms error occurs, an error code is returned.
		 * @return int16_t Returns 0 (NO_ERROR) on reset okay, negative for error code.
		 */
		int16_t	reset();
		/**
		 * @brief Initialize the chip.
		 * Initialize the BMx280 chip.  Returns the achieved baud rate (target is 3.2MHz), otherwise if
         * and invalid address, the chip responds with invalid values, or invalid pins are provided, and
         * error code is returned.
		 * @param config The config byte to write to the chip, defaults to standby time of 500us, filter off, comms in 4 wire mode.
		 * @param measCtrl The measCtrl byte to write to the chip, defaults to temperature =OSR_X1, pressure=OSR_X1, mode = MODE_FORCED.
		 * @param humCtrl the humCtrl byte to write to the chip if available, defaults to humidty = OSR_X1
		 * @return int32_t Positive is the achieved baud rate, negaitive for error code.
		 */
		int32_t	initialize(const uint8_t config=DEFAULT_CONFIG, const uint8_t measCtrl=DEFAULT_MEAS_SETTINGS, const uint8_t humCtrl=DEFAULT_HUM_SETTINGS);
		/**
		 * @brief Update the temperature, humidity and pressure values.
		 * Read and update the pressure temperature and humidity values. Will force a reading if in forced mode. Returns
		 * 0 (NO_ERROR) if updated okay, otherwise if a comms error occurs, an error code is returned.
		 * @return int16_t Returns 0(NO_ERROR) if update okay, negative for error code.
		 */
		int16_t	update();
        /**
         * @brief Verify bmx280 address.
         * Returns true if the provided address is a valid BMx280 I2C address.
         * @param address Address to verify
         * @return true Address is valid.
         * @return false Address is invalid.
         */
        bool    verifyAddress(const uint8_t address);
    private:
	/* Constants: */
        //I2C timeout:
        static const uint32_t TIMEOUT_US            = 1000; // One microsecond timeout.
        // Addresses:
        static const uint8_t BME280_ADDR            = 0x76;
        static const uint8_t BMP280_ADDR            = 0x77;
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
		// SPI Modes:
		static const uint8_t SPI_3WIRE_FALSE	= 0x00;
		static const uint8_t SPI_3WIRE_TRUE		= 0x01;
		// Reset Value:
		static const uint8_t RESET_VALUE = 0xB6;
		// Default Sensor config:
		static const uint8_t DEFAULT_CONFIG 		= 0b10000000;// Default config, Standby 500us, filter off, spi3wire off.
		static const uint8_t DEFAULT_HUM_SETTINGS 	= 0b00000001;// Default humidity settings, hum OSR x1.
		static const uint8_t DEFAULT_MEAS_SETTINGS	= 0b00100101;// Default settings, temp OSR x1, pres OSR x1, forced mode.
        // Pin constants:
        const uint8_t _sdaPin = MY_NOT_A_PIN;
        const uint8_t _sclPin = MY_NOT_A_PIN;
        // Address constant:
        const uint8_t _address = 0x00;
	
    /* Private Variables: */
        // I2C variables:
		i2c_inst_t *_i2cPort;
        
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
		int16_t __readRegisters__(const uint8_t reg, uint8_t *buffer, const uint8_t length);
		int16_t __writeRegister__(const uint8_t reg, const uint8_t value);
		int16_t __readUint8__(const uint8_t reg, uint8_t value);
		int16_t __readInt8__(const uint8_t reg, int8_t value);
		int16_t __readUint16__(const uint8_t reg, uint16_t value);
		int16_t __readInt16__(const uint8_t reg, int16_t value);
		int16_t __readTemperatureCalibration__();
		int16_t __readPressureCalibration__();
		int16_t __readHumidityCalibration__();
		int32_t __calculateTFine__(const int32_t rawTemperature);
		float __calculateTemperature__();
		float __calculatePressure__(const int32_t rawPressure);
		float __calculateHumidity__(const int32_t rawHumidity);

};

/*
 * ############################## Constructor: ##################################
 */
myBMx280::myBMx280(i2c_inst_t *i2cPort, const uint8_t sdaPin, const uint8_t sclPin, const uint8_t address) : 
                        _sdaPin (sdaPin), _sclPin(sclPin), _address (address) {
    _i2cPort = i2cPort;
}
/*
 * ################################## Public functions: ##########################
 */
    int16_t myBMx280::getID() {
        uint8_t ID;
        int16_t returnValue;
        returnValue = __readRegisters__(REG_ID_ADDR, &ID, 1);
        if (returnValue < 0) { return returnValue; } // Error occured, return the error.
        return (int16_t)ID; // Otherwise return the ID.
    }

	int16_t myBMx280::getStatus() {
		uint8_t status;
		int16_t returnValue;
		returnValue = __readRegisters__(REG_STATUS_ADDR, &status, 1);
		if (returnValue < 0) { return returnValue; }
		return (int16_t)status;
	}

	int16_t myBMx280::getConfig() {
		uint8_t config;
		int16_t returnValue;
		returnValue = __readRegisters__(REG_CONFIG_ADDR, &config, 1);
		if (returnValue < 0) { return returnValue; }
		return (int16_t)config;
	}

	int16_t myBMx280::setConfig(const uint8_t value) {
		int16_t returnValue;
		returnValue = __writeRegister__(REG_CONFIG_ADDR, value);
		if (returnValue < 0) { return returnValue; }
		return NO_ERROR;
	}

	int16_t myBMx280::getMeasCtrl() {
		uint8_t measCtrl;
		int16_t returnValue;
		returnValue = __readRegisters__(REG_CTRL_MEAS_ADDR, &measCtrl, 1);
		if (returnValue < 0) { return returnValue; }
		return (int16_t)measCtrl;
	}

	int16_t myBMx280::setMeasCtrl(const uint8_t value) {
		uint8_t mode;
		int16_t returnValue;
		mode = value & MEAS_MODE_MASK;
		if (mode == 0x01 or mode == 0x02) {
			forced_mode = true;
		} else {
			forced_mode = false;
		}
		returnValue = __writeRegister__(REG_CTRL_MEAS_ADDR, value);
		return returnValue;
	}

	int16_t myBMx280::getHumCtrl() {
		uint8_t humCtrl = 0x00;
		int16_t returnValue;
		if (have_humidity == true) {
			returnValue = __readRegisters__(REG_CTRL_HUM_ADDR, &humCtrl, 1);
			if (returnValue < 0) { return returnValue; }
		}
		return (int16_t)humCtrl;
	}

	int16_t myBMx280::setHumCtrl(const uint8_t value) {
		int16_t returnValue;
		if (have_humidity == true) {
			uint8_t measCtrl = getMeasCtrl();
			returnValue = __writeRegister__(REG_CTRL_HUM_ADDR, value);
			if (returnValue < 0) { return returnValue; }
			returnValue = __writeRegister__(REG_CTRL_MEAS_ADDR, measCtrl);
			if (returnValue < 0) { return returnValue; }
		} else {
			return ERROR_NO_HUMIDITY;
		}
		return NO_ERROR;
	}

	int16_t myBMx280::getStandbyTime() {
		int16_t returnValue;
		returnValue = getConfig();
		if (returnValue < 0) { return returnValue; }
		return (((uint8_t)returnValue & CONFIG_STANDBY_MASK) >> 5);
	}

	int16_t myBMx280::setStandbyTime(const uint8_t value) {
		int8_t config;
		int16_t returnValue;
		if (value > STANDBY_TIME_20MS) { return ERROR_INVALID_STANDBY; }
		returnValue = getConfig();
		if (returnValue < 0) { return returnValue; }
		config = (uint8_t)returnValue;
		config &= ~CONFIG_STANDBY_MASK;
		config |= (value << 5);
		returnValue = setConfig(config);
		return returnValue;
	}

	int16_t myBMx280::getFilter() {
		int16_t returnValue;
		returnValue = getConfig();
		if (returnValue < 0) { return returnValue; }
		return (((uint8_t)returnValue & CONFIG_FILTER_MASK) >> 2);
	}

	int16_t myBMx280::setFilter(const uint8_t value) {
		int16_t returnValue;
		uint8_t config;
		if (value > FILTER_16) { return ERROR_INVALID_FILTER; }
		returnValue = getConfig();
		if (returnValue < 0) { return returnValue; }
		config = (uint8_t)returnValue;
		config &= ~CONFIG_FILTER_MASK;
		config |= (value << 2);
		returnValue = setConfig(config);
		return returnValue;
	}

	int16_t myBMx280::getTemperatureOSR() {
		uint8_t osr;
		int16_t returnValue;
		returnValue = getMeasCtrl();
		if (returnValue < 0) { return returnValue; }
		return ((uint8_t)returnValue >> 5);
	}

	int16_t myBMx280::setTemperatureOSR(const uint8_t value) {
		uint8_t measCtrl;
		int16_t returnValue;
		returnValue = getMeasCtrl();
		if (returnValue < 0) { return returnValue; }
		measCtrl = (uint8_t)returnValue;
		measCtrl &= ~MEAS_TEMP_OSR_MASK;
		measCtrl |= (value << 5);
		returnValue = setMeasCtrl(measCtrl);
		return returnValue;
	}

	int16_t myBMx280::getPressureOSR() {
		int16_t returnValue;
		returnValue = getMeasCtrl();
		if (returnValue < 0) { return returnValue; }
		return ((uint8_t)returnValue >> 2);
	}

	int16_t myBMx280::setPressureOSR(const uint8_t value) {
		uint8_t measCtrl;
		int16_t returnValue;
		if (value > OSR_X16) { return ERROR_INVALID_OSR; }
		returnValue = getMeasCtrl();
		if (returnValue < 0) { return returnValue; }
		measCtrl = (uint8_t)returnValue;
		measCtrl &= ~MEAS_PRES_OSR_MASK;
		measCtrl |= (value << 2);
		returnValue = setMeasCtrl(measCtrl);
		return returnValue;
	}

	int16_t myBMx280::getHumidityOSR() {
		int16_t returnValue;
		returnValue = getHumCtrl();
		if (returnValue < 0) { return returnValue; }
		return ((uint8_t)returnValue & HUM_OSR_MASK);
	}

	int16_t myBMx280::setHumidityOSR(const uint8_t value) {
		uint8_t humCtrl;
		int16_t returnValue;
		if (value > OSR_X16) { return ERROR_INVALID_OSR; }
		returnValue = getHumCtrl();
		if (returnValue < 0) { return returnValue; }
		humCtrl = (uint8_t)returnValue;
		humCtrl &= ~HUM_OSR_MASK;
		humCtrl |= value;
		returnValue = setHumCtrl(humCtrl);
		return returnValue;
	}

	int16_t myBMx280::getMode() {
		int16_t returnValue;
		returnValue = getMeasCtrl();
		if (returnValue < 0) { return returnValue; }
		return ((uint8_t)returnValue & MEAS_MODE_MASK);
	}

	int16_t myBMx280::setMode(const uint8_t value) {
		uint8_t measCtrl;
		int16_t returnValue;
		if (value > MODE_NORMAL) { return ERROR_INVALID_MODE; }
		returnValue = getMeasCtrl();
		if (returnValue < 0) { return returnValue; }
		measCtrl = (uint8_t)returnValue;
		measCtrl &= ~ MEAS_MODE_MASK;
		measCtrl |= value;
		returnValue = setMeasCtrl(measCtrl);
		return returnValue;
	}

	int16_t myBMx280::reset() {
		int16_t returnValue;
		returnValue = __writeRegister__(REG_RESET_ADDR, RESET_VALUE);
		return returnValue;
	}

	
    int32_t myBMx280::initialize(const uint8_t config, const uint8_t measCtrl, const uint8_t humCtrl){
        int32_t baudRate;
        int16_t returnValue;
        uint8_t chipID;
    // Init I2C:
        if (verifyAddress(_address) == false) { return ERROR_INVALID_ADDRESS; }
        baudRate = myI2C::initializeMaster(_i2cPort, _sdaPin, _sclPin, 3200*1000);
		printf("Got baud: %i\n", baudRate);
        if (baudRate < 0) { return baudRate; } // Error occured, return the error.
    // Check chip ID and set have humidity:
        returnValue = getID();
        if (returnValue < 0) { return returnValue; } // Error occured, return the error.
        chipID = (uint8_t)returnValue;
        switch (chipID) {
            case CHIPMODEL_BME280:
                have_humidity = true;
                break;
            case CHIPMODEL_BMP280:
                have_humidity = false;
                break;
            default:
                return ERROR_INVALD_RESPONSE;
        }
    // Read Calibrations:
        returnValue = __readTemperatureCalibration__();
        if (returnValue < 0) { return returnValue; }
        returnValue = __readPressureCalibration__();
        if (returnValue < 0) { return returnValue; }
        if (have_humidity == true) {
            returnValue = __readHumidityCalibration__();
            if (returnValue < 0) { return returnValue; }
        }
    // Verify forced mode:
        uint8_t mode = measCtrl & 0x03;
        if (mode == 0x01 or mode == 0x02) {
            forced_mode = true;
        } else {
            forced_mode = false;
        }
    // Write configuration:
        returnValue = __writeRegister__(REG_CONFIG_ADDR, config);
        if (returnValue < 0) { return returnValue; }
        if (have_humidity == true) {
            returnValue = __writeRegister__(REG_CTRL_HUM_ADDR, humCtrl);
            if (returnValue < 0) { return returnValue; }
        }
        returnValue = __writeRegister__(REG_CTRL_MEAS_ADDR, measCtrl);
        if (returnValue < 0) { return returnValue; }
        return baudRate;
    }

	int16_t myBMx280::update() {
		int16_t returnValue;
	// If in forced mode, force the reading.
		if (forced_mode == true) {
			returnValue = getMeasCtrl();
			if (returnValue < 0) { return returnValue; }
			setMeasCtrl((uint8_t)returnValue);
			sleep_us(1);
		}
	// Read the data from the chip:
		uint8_t dataBuffer[BME280_DATA_LEN] = {0x00};
		if (have_humidity == true) {
			returnValue = __readRegisters__(REG_DATA_START_ADDR, dataBuffer, BME280_DATA_LEN);
		} else {
			returnValue = __readRegisters__(REG_DATA_START_ADDR, dataBuffer, BMP280_DATA_LEN);
		}
		if (returnValue < 0) { return returnValue; }
	// Set raw data values:
		int32_t rawPressure, rawTemperature, rawHumidity;
		rawPressure = ((int32_t)dataBuffer[0] << 12) | ((int32_t)dataBuffer[1] << 4) | ((int32_t)dataBuffer[2] >> 4);
		rawTemperature = ((int32_t)dataBuffer[3] << 12) | ((int32_t)dataBuffer[4] << 4) | ((int32_t)dataBuffer[5] >> 4);
		if (have_humidity == true) {
			rawHumidity = ((int32_t)dataBuffer[6] << 8) | (int32_t)dataBuffer[7];
		}
	// Calculate tFine calibration value:
		_t_fine = __calculateTFine__(rawTemperature);
	// Calculate final values:
		temperature = __calculateTemperature__();
		pressure = __calculatePressure__(rawPressure);
		if (have_humidity == true) {
			humidity = __calculateHumidity__(rawHumidity);
		}
	}

    bool myBMx280::verifyAddress(const uint8_t address) {
        if (address == BME280_ADDR or address == BMP280_ADDR) { return true; }
        return false;
    }

/*
 * ############################# Private functions: ##############################
 */
int16_t myBMx280::__readRegisters__(const uint8_t reg, uint8_t *buffer, const uint8_t length) {
    int32_t returnValue;
    returnValue = i2c_write_timeout_us(_i2cPort, _address, &reg, 1, true, TIMEOUT_US);
    if (returnValue < 0) {
        if (returnValue == PICO_ERROR_GENERIC) { return ERROR_NO_RESPONSE; }
        if (returnValue == PICO_ERROR_TIMEOUT) { return ERROR_TIMEOUT; }
    } 
    returnValue = i2c_read_timeout_us(_i2cPort, _address, buffer, length, false, TIMEOUT_US);
    if (returnValue < 0) {
        if (returnValue == PICO_ERROR_GENERIC) { return ERROR_NO_RESPONSE; }
        if (returnValue == PICO_ERROR_TIMEOUT) { return ERROR_TIMEOUT; }
    } 
    return NO_ERROR;
}

int16_t myBMx280::__writeRegister__(const uint8_t reg, const uint8_t value) {
    int32_t returnValue;
    const uint8_t buffer[2] = {reg, value};
    returnValue = i2c_write_timeout_us(_i2cPort, _address, buffer, 2, false, TIMEOUT_US);
    if (returnValue < 0) {
        if (returnValue == PICO_ERROR_GENERIC) { return ERROR_NO_RESPONSE; }
        if (returnValue == PICO_ERROR_TIMEOUT) { return ERROR_TIMEOUT; }
    } 
    return NO_ERROR;
}

int16_t myBMx280::__readUint8__(const uint8_t reg, uint8_t value) {
    int16_t returnValue;
    returnValue = __readRegisters__(reg, &value, 1);
    if (returnValue < 0) { return returnValue; }
    return NO_ERROR;
}

int16_t myBMx280::__readInt8__(const uint8_t reg, int8_t value) {
    int16_t returnValue;
    uint8_t readValue;
    returnValue = __readRegisters__(reg, &readValue, 1);
    if (returnValue < 0) { return returnValue; }
    value = (int8_t)readValue;
    return NO_ERROR;
}

int16_t myBMx280::__readUint16__(const uint8_t reg, uint16_t value) {
    uint8_t buffer[2];
    int16_t returnValue;
    returnValue = __readRegisters__(reg, buffer, 2);
    if (returnValue < 0) { return returnValue; }
    value = (((uint16_t)buffer[1] << 8) | (uint16_t)buffer[0]);
    return NO_ERROR;
}

int16_t myBMx280::__readInt16__(const uint8_t reg, int16_t value) {
    uint8_t buffer[2];
    int16_t returnValue;
    returnValue = __readRegisters__(reg, buffer, 2);
    if (returnValue < 0) { return returnValue; }
    value = (((int16_t)buffer[1] << 8) | (int16_t)buffer[0]);
    return NO_ERROR;
}

int16_t myBMx280::__readTemperatureCalibration__() {
    int16_t returnValue;
    returnValue = __readUint16__(REG_TEMP_DIG_T1_ADDR, _dig_t1);
    if (returnValue < 0) { return returnValue; }
    returnValue = __readInt16__(REG_TEMP_DIG_T2_ADDR, _dig_t2);
    if (returnValue < 0) { return returnValue; }
    returnValue = __readInt16__(REG_TEMP_DIG_T3_ADDR, _dig_t3);
    return returnValue;    
}

int16_t myBMx280::__readPressureCalibration__() {
    int16_t returnValue;
    returnValue = __readUint16__(REG_PRES_DIG_P1_ADDR, _dig_p1);
    if (returnValue < 0) { return returnValue; }
    returnValue = __readInt16__(REG_PRES_DIG_P2_ADDR, _dig_p2);
    if (returnValue < 0) { return returnValue; }
    returnValue = __readInt16__(REG_PRES_DIG_P3_ADDR, _dig_p3);
    if (returnValue < 0) { return returnValue; }
    returnValue = __readInt16__(REG_PRES_DIG_P4_ADDR, _dig_p4);
    if (returnValue < 0) { return returnValue; }
    returnValue = __readInt16__(REG_PRES_DIG_P5_ADDR, _dig_p5);
    if (returnValue < 0) { return returnValue; }
    returnValue = __readInt16__(REG_PRES_DIG_P6_ADDR, _dig_p6);
    if (returnValue < 0) { return returnValue; }
    returnValue = __readInt16__(REG_PRES_DIG_P7_ADDR, _dig_p7);
    if (returnValue < 0) { return returnValue; }
    returnValue = __readInt16__(REG_PRES_DIG_P8_ADDR, _dig_p8);
    if (returnValue < 0) { return returnValue; }
    returnValue = __readInt16__(REG_PRES_DIG_P9_ADDR, _dig_p9);
    return returnValue;    
}

int16_t myBMx280::__readHumidityCalibration__() {
    int16_t returnValue;
    uint8_t buffer[3];
    returnValue = __readUint8__(REG_HUM_DIG_H1_ADDR, _dig_h1);
    if (returnValue < 0) { return returnValue; }
    returnValue = __readInt16__(REG_HUM_DIG_H2_ADDR, _dig_h2);
    if (returnValue < 0) { return returnValue; }
    returnValue = __readUint8__(REG_HUM_DIG_H3_ADDR, _dig_h3);
    if (returnValue < 0) { return returnValue; }
    returnValue = __readRegisters__(REG_HUM_DIG_H4_ADDR, buffer, 3);
    if (returnValue < 0) { return returnValue; }
	_dig_h4 = ((int16_t)buffer[0] <<4) | (0x000F & (int16_t)buffer[1]);
	_dig_h5 = ((int16_t)buffer[2] <<4) | (((int16_t)buffer[1] >> 4) & 0x000F);
    returnValue = __readInt8__(REG_HUM_DIG_H6_ADDR, _dig_h6);
    return returnValue;
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
	if (var1 == 0) { return 0.0f; } // Avoid div by 0.
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