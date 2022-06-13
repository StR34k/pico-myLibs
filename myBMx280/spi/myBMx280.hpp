#ifndef MY_BMx280_H
#define MY_BMx280_H

#include <hardware/spi.h>

class myBMx280 {
	public:
		// static const uint8_t BITBANG_MODE = 0X00;
		// static const uint8_t HWSPI_MODE = 0x01;
		myBMx280(spi_inst_t *spiObj, uint8_t csPin, uint8_t clkPin, uint8_t misoPin, uint8_t mosiPin);
		bool initialize();
		// void 	begin		(const uint8_t mode=BITBANG_MODE);
		// uint8_t	readByte	(const bool bitOrder=MSBFIRST);
		// void 	startRead	();
		// uint8_t	read		(const bool bitOrder=MSBFIRST);
		// void 	stopRead	();
		
	private:
		spi_inst_t *_spiObj;
		uint8_t _csPin;
		uint8_t _sckPin;
		uint8_t _misoPin;
		uint8_t _mosiPin;
		// uint8_t _mode = BITBANG_MODE;
};
#endif