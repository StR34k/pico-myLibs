#ifndef MY23LC1024_H
#define MY23LC1024_H
#include "../myStandardDefines.hpp"
#include <hardware/spi.h>

class my23LC1024 {

    public:
        my23LC1024(spi_inst_t *spiPort, const uint8_t csPin, const uint8_t sckPin, const uint8_t misoPin, 
                        const uint8_t mosiPin, const uint8_t holdPin=MY_NOT_A_PIN, const uint8_t sio2Pin=MY_NOT_A_PIN);
    private:
        spi_inst_t *_spiPort;
        uint8_t _csPin;
        uint8_t _sckPin;
        uint8_t _misoPin;
        uint8_t _mosiPin;
        uint8_t _holdPin;
        uint8_t _sio2Pin;
};

my23LC1024::my23LC1024(spi_inst_t *spiPort, const uint8_t csPin, const uint8_t sckPin, const uint8_t misoPin, 
                            const uint8_t mosiPin, const uint8_t holdPin=MY_NOT_A_PIN, const uint8_t sio2Pin=MY_NOT_A_PIN) {
    _spiPort = spiPort;
    _sckPin = sckPin;
    _misoPin = misoPin;
    _mosiPin = mosiPin;
    _holdPin = holdPin;
    _sio2Pin = sio2Pin;
}




#endif