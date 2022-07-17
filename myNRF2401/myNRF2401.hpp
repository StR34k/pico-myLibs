/**
 * @file myNRF2401.hpp
 * @author Peter Nearing (pnearing@protonmail.com)
 * @brief Class to use a nRF24L01 radio module.
 * @version 0.1
 * @date 2022-07-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MY_NRF2401_H
#define MY_NRF2401_H

#include <hardware/gpio.h>
#include <hardware/spi.h>

#include "../myStandardDefines.hpp"
#include "../myErrorCodes.hpp"
#include "../myHelpers.hpp"
#include "../mySPI/mySPI.hpp"

/**
 * @brief Class to use a nRF2401 radio module.
 * 
 */
class myNRF2401 {
    public:
/* ##################### Constructors ############################ */
    myNRF2401(spi_inst_t *spiPort, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin,
                    const uint8_t csPin, const uint8_t cePin, const uint8_t irqPin) : _sckPin (sckPin),
                    _misoPin (misoPin), _mosiPin (mosiPin), _csPin (csPin), _cePin (cePin), _irqPin (irqPin)
    {
        _spiPort = spiPort;                
    }


    private:
/* #################### Private constants: ######################## */
    // SPI Commands:
        static const uint8_t READ_REGISTER              = 0x00; // 0b 000a aaaa Read register. Where a are address bits.
        static const uint8_t WRITE_REGISTER             = 0x20; // 0b 001a aaaa Write register. Where a are address bits.
        static const uint8_t READ_RX_PAYLOAD            = 0x61; // 0b 0110 0001 Read RX Payload.
        static const uint8_t WRITE_TX_PAYLOAD           = 0xA0; // 0b 1010 0000 Write TX Payload.
        static const uint8_t FLUSH_TX                   = 0xE1; // 0b 1110 0001 Flush TX FIFO.
        static const uint8_t FLUSH_RX                   = 0xE2; // 0b 1110 0010 Flush RX FIFO.
        static const uint8_t ACTIVATE                   = 0x50; // 0b 0101 0000 Acivate features call with 0x73 to acivate.
        static const uint8_t READ_RX_PL_WIDTH           = 0x60; // 0b 0110 0000 Read rx Payload width.
        static const uint8_t WRITE_ACK_PAYLOAD          = 0xA8; // 0b 1010 1ppp  Where p is Pipe bits.
        static const uint8_t WRITE_TX_PAYLOAD_NO_ACK    = 0xB0; // 0b 1011 0000 Disables AUTO ACK on packet.
        static const uint8_t NOP                        = 0xFF; // 0b 1111 1111 No operation.
    // Register map:
        static const uint8_t REG_CONFIG         = 0x00; // Config register.
        static const uint8_t REG_ENABLE_AA      = 0x01; // Enable auto acknowledgement.
        static const uint8_t REG_ENABLE_RX_ADDR = 0x02; // Enabled RX Addresses.
        static const uint8_t REG_SETUP_AW       = 0x03; // Setup address widths.
        static const uint8_t REG_SETUP_RETR     = 0x04; // Setup automatic retransmission.
        static const uint8_t REG_RF_CHANNEL     = 0x05; // RF Channel.
        static const uint8_t REG_RF_SETUP       = 0x06; // RF Setup register.
        static const uint8_t REG_STATUS         = 0x07; // Status register.
        static const uint8_t REG_OBSERVE_TX     = 0x08; // Transmit observe register.
        static const uint8_t REG_CD             = 0x09; // Carrier detect.
        static const uint8_t REG_RX_ADDR_P0     = 0x0A; // Recieve address, Pipe 0.
        static const uint8_t REG_RX_ADDR_P1     = 0x0B; // Recieve address, Pipe 1.
        static const uint8_t REG_RX_ADDR_P2     = 0x0C; // Recieve address, Pipe 2.
        static const uint8_t REG_RX_ADDR_P3     = 0x0D; // Recieve address, Pipe 3.
        static const uint8_t REG_RX_ADDR_P4     = 0x0E; // Recieve address, Pipe 4.
        static const uint8_t REG_RX_ADDR_P5     = 0x0F; // Recieve address, Pipe 5.
        static const uint8_t REG_TX_ADDR        = 0x10; // Transmit address.
        static const uint8_t REG_RX_PW_P0       = 0x11; // Recieve payload width, Pipe 0.
        static const uint8_t REG_RX_PW_P1       = 0x12; // Recieve payload width, Pipe 1.
        static const uint8_t REG_RX_PW_P2       = 0x13; // Recieve payload width, Pipe 2.
        static const uint8_t REG_RX_PW_P3       = 0x14; // Recieve payload width, Pipe 3.
        static const uint8_t REG_RX_PW_P4       = 0x15; // Recieve payload width, Pipe 4.
        static const uint8_t REG_RX_PW_P5       = 0x16; // Recieve payload width, Pipe 5.
        static const uint8_t REG_FIFO_STATUS    = 0x17; // FIFO Status register.
        static const uint8_t REG_DYNPD          = 0x1C; // Enable dynamic payload length.
        static const uint8_t REG_FEATURE        = 0x1D; // Feature register.
    // Pins:
        const uint8_t _sckPin = MY_NOT_A_PIN;
        const uint8_t _misoPin = MY_NOT_A_PIN;
        const uint8_t _mosiPin = MY_NOT_A_PIN;
        const uint8_t _csPin = MY_NOT_A_PIN;
        const uint8_t _cePin = MY_NOT_A_PIN;
        const uint8_t _irqPin = MY_NOT_A_PIN;
/* ##################### Private variables ################### */
        spi_inst_t *_spiPort;
};


#endif