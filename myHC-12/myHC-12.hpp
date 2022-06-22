#ifndef MY_HC12_H
    #define MY_HC12_H
    #include <hardware/uart.h>
    #include <hardware/gpio.h>
    #include <pico/time.h>
    #include "../myStandardDefines.hpp"
    #include "../myErrorCodes.hpp"

class myHC12 {
    public:
    /* Constants: */
        // Operating Modes:
        static const uint8_t MODE_FU1 = 0x00;  // Relative powersaving mode. All baud rates.
        static const uint8_t MODE_FU2 = 0x01;  // Powersaving mode. Supports baud: 1200, 2400, 4800
        static const uint8_t MODE_FU3 = 0x02;  // Normal mode. All baud rates.
        // Baud Rates
        static const uint8_t BAUD_1200 = 0x00; // Supported baud rates. default 9600.
        static const uint8_t BAUD_2400 = 0x01;
        static const uint8_t BAUD_4800 = 0x02;
        static const uint8_t BAUD_9600 = 0x03;
        static const uint8_t BUAD_19200 = 0x04;
        static const uint8_t BAUD_38400 = 0x05;
        static const uint8_t BAUD_57600 = 0x06;
        static const uint8_t BAUD_115200 = 0x07;
        // Channels:
        static const uint8_t MIN_CHANNEL = 1;      // Frequency the module operates on.
        static const uint8_t MAX_CHANNEL = 127;
        // Transmit Power levels:
        static const uint8_t POWER_NEG_1DBM = 0x01;    // Power level of the transmitter, default to 20 dBm
        static const uint8_t POWER_2DBM = 0x02;
        static const uint8_t POWER_5DBM = 0x03;
        static const uint8_t POWER_8DBM = 0x04;
        static const uint8_t POWER_11DBM = 0x05;
        static const uint8_t POWER_14DBM = 0x06;
        static const uint8_t POWER_17DBM = 0x07;
        static const uint8_t POWER_20DBM = 0x08;
        // Error codes:
        static const int16_t ERROR_INVALID_BAUD = MY_ERROR_MYHC12_INVALID_BAUD;         // -350 : Invalid baud provided.
        static const int16_t ERROR_INVALID_CHANNEL = MY_ERROR_MYHC12_INVALID_CHANNEL;   // -351 : Invalid channel provided.
        static const int16_t ERROR_INVALID_MODE = MY_ERROR_MYHC12_INVALID_MODE;         // -352 : Invalid mode provided.
        static const int16_t ERROR_INVALID_POWER = MY_ERROR_MYHC12_INVALID_POWER;       // -353 : Invalid power level provided.
    /* Constructors: */
        myHC12(uart_inst_t *uartPort, const uint8_t rxPin, const uint8_t txPin) : 
                    _rxPin (rxPin), _txPin (txPin) {
            _uartPort = uartPort;
        }

        myHC12(uart_inst_t *uartPort, const uint8_t rxPin, const uint8_t txPin, const uint8_t setPin) :
                    _rxPin (rxPin), _txPin(txPin), _setPin (setPin) {
            _uartPort = uartPort;
        }
    /* Functions: */
        // Validation funcitons:
        bool    validBaud(const uint8_t baud);
        bool    validChannel(const uint8_t channel);
        bool    validPower(const uint8_t power);
        bool    validMode(const uint8_t mode);
        // Set functions:
        int16_t setBaud(const uint8_t baud);
        int16_t setChannel(const uint8_t channel);
        int16_t setPower(const uint8_t power);
        int16_t setMode(const uint8_t mode);
        // Initialize function:
        int16_t initialize(const uint8_t baud=BAUD_9600, const uint8_t channel=MIN_CHANNEL, 
                                const uint8_t txPower=POWER_20DBM, const uint8_t mode=MODE_FU3);
    private:
    /* Variables: */
        uart_inst_t *_uartPort;
        const uint8_t _rxPin = MY_NOT_A_PIN;
        const uint8_t _txPin = MY_NOT_A_PIN;
        const uint8_t _setPin = MY_NOT_A_PIN;
        uint8_t _baudRate = BAUD_9600;
        uint8_t _txPower = POWER_20DBM;
        uint8_t _mode = MODE_FU3;
        uint8_t _channel = MIN_CHANNEL;
        uint32_t _txDelay = 80; // Default tx delay in milliseconds.
    /* Functions: */

};

/*
 *  Public Functions:
 */
bool myHC12::validBaud(const uint8_t baud) {
    if (_mode == MODE_FU2 and baud > BAUD_4800) {
        return false;
    }else if (baud > BAUD_115200) { 
        return false;
    }
    return true;
}

bool myHC12::validChannel(const uint8_t channel) {
    if (channel < MIN_CHANNEL or channel > MAX_CHANNEL) { return false; }
    return true;
}

bool myHC12::validMode(const uint8_t mode) {
    if (mode > MODE_FU3) { return false; }
    return true;
}

bool myHC12::validPower(const uint8_t power) {
    if (power > POWER_20DBM) { return false; }
    return true;
}

int16_t myHC12::initialize(const uint8_t baud, const uint8_t channel, const uint8_t txPower, 
                                const uint8_t mode) {
// Validate arguments:
    if (validBaud(baud) == false) { return ERROR_INVALID_BAUD; }
    if (validChannel(channel) == false) { return ERROR_INVALID_CHANNEL; }
    if (validMode(mode) == false) { return ERROR_INVALID_MODE; }
    if (validPower(txPower) == false) { return ERROR_INVALID_POWER; }
    uart_init(_uartPort, 9600);
    gpio_set_function(_rxPin, GPIO_FUNC_UART);
    gpio_set_function(_txPin, GPIO_FUNC_UART);
    if (_setPin != MY_NOT_A_PIN) {
    // Init the gpio:
        gpio_init(_setPin);
        gpio_set_dir(_setPin, GPIO_OUT);
        gpio_put(_setPin, true);
    // if we're given a mode, switch to mode.
        sleep_ms(500);
        gpio_put(_setPin, false);
        uart_puts(_uartPort, "AT");
        char buffer[10];
        // uart_read_blocking(_uartPort, buffer, 1);
        buffer[0] = uart_getc(_uartPort);
        // uart_getc(_)
        printf("Buffer: %s\n", buffer[0]);    
    // If we're given a baud switch:
    // If we're give a channel switch to channel:
    // if we're given a power level switch powerlevels.
    
    
    }
    return MY_NO_ERROR;
}

#endif