/**
 * @file myMAX1415.hpp
 * @author Peter Nearing (pnearing@protonmail.com)
 * @brief Class to use a MAX1415 16bit ADC.
 * @version 0.1
 * @date 2022-07-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MY_MAX1415_H
#define MY_MAX1415_H


#include "../myErrorCodes.hpp"
#include "../myHelpers.hpp"
#include "../mySPI/mySPI.hpp"


class myMAX1415 {
    public:
/* ############ Public Constants ############# */
    // Channel Select:                                      AIN +   AIN-    Offset/Gain Register Pair.
        static const uint8_t CHANNEL_A = 0x00; // 0b ---- --00     ain1+   ain1-   0
        static const uint8_t CHANNEL_B = 0x01; // 0b ---- --01     ain2+   ain2-   1
    // Operating modes:
        static const uint8_t MODE_NORMAL       = 0x00; // 0b 00-- ----
        static const uint8_t MODE_SELF_CAL     = 0x40; // 0b 01-- ----
        static const uint8_t MODE_ZERO_CAL     = 0x80; // 0b 10-- ----
        static const uint8_t MODE_FULL_CAL     = 0xC0; // 0b 11-- ----
    // Gain Selection:                          (Shifted value)
        static const uint8_t GAIN_1    = 0x00; // 0b --00 0---
        static const uint8_t GAIN_2    = 0x01; // 0b --00 1---
        static const uint8_t GAIN_4    = 0x02; // 0b --01 0---
        static const uint8_t GAIN_8    = 0x03; // 0b --01 1---
        static const uint8_t GAIN_16   = 0x04; // 0b --10 0---
        static const uint8_t GAIN_32   = 0x05; // 0b --10 1---
        static const uint8_t GAIN_64   = 0x06; // 0b --11 0---
        static const uint8_t GAIN_128  = 0x07; // 0b --11 1---
    // Clock speeds:
        static const uint8_t CLOCK_FREQ_1MHZ = 0x00; // 0b ---- -0-- 1MHz
        static const uint8_t CLOCK_FREQ_2MHZ = 0x01; // 0b ---- -1-- 2.4576 MHz
    // Output Data Rates: (Sets FILTER_SEL) Assumes clk_div=0.
        // CLOCK_FREQ = 1MHz
        static const uint8_t FREQ_20HZ     = 0x00; // 0b ---- --00     
        static const uint8_t FREQ_25HZ     = 0x01; // 0b ---- --01
        static const uint8_t FREQ_100HZ    = 0x02; // 0b ---- --10
        static const uint8_t FREQ_200HZ    = 0x03; // 0b ---- --11
        // CLOCK_FREQ = 2.4576 MHz
        static const uint8_t FREQ_50HZ     = 0x00; // 0b ---- --00
        static const uint8_t FREQ_60HZ     = 0x01; // 0b ---- --01
        static const uint8_t FREQ_250HZ    = 0x02; // 0b ---- --10
        static const uint8_t FREQ_500HZ    = 0x03; // 0b ---- --11
    // Errors:
        static const int16_t NO_ERROR = MY_NO_ERROR;
        /**
         * @brief Invalid Chip Select pin. Value -153.
         * Invalid Chip Select pin was passed during construction.
         */
        static const int16_t ERROR_INVALID_CS_PIN = MY_ERROR_SPI_INVALID_CS_PIN;
        /**
         * @brief Invalid update frequency. Value -225.
         * The provided update frequency is invalid. Valid values (0x00-0x03). Use FREQ_* values.
         */
        static const int16_t ERROR_INVALID_UPDATE_FREQ = MY_ERROR_MYMAX1415_INVALID_UPDATE_FREQUENCY;
        /**
         * @brief Invalid gain. Value -226.
         * The provided gain value is invalid.
         */
        static const int16_t ERROR_INVALID_GAIN = MY_ERROR_MYMAX1415_INVALID_GAIN;
/* ############# Constructors: ################# */
        myMAX1415(spi_inst_t *spiPort, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin,
                        const uint8_t csPin, const uint8_t resetPin, const uint8_t drdyPin) : _sckPin(sckPin),
                        _misoPin (misoPin), _mosiPin (mosiPin), _csPin (csPin), _resetPin (resetPin),
                        _drdyPin (drdyPin) {
            _spiPort = spiPort;
        }
/* ############ Public Functions: ################# */
        /**
         * @brief Read channel A.
         * Reads and returns the value of channel A.
         * @return uint16_t The value of channel A.
         */
        uint16_t readChannelA();
        /**
         * @brief Read channel B.
         * Reads and returns the value of channel B.
         * @return uint16_t The value of channel B.
         */
        uint16_t readChannelB();
        /**
         * @brief Read a channel.
         * Reads the given channel.
         * @param channel True = Channel B, False = Channel A.
         * @return uint16_t Channel reading.
         */
        uint16_t readChannel(const bool channel);
        /**
         * @brief Read both channels.
         * Reads both channels. Sets readingA to reading of Channel A, and readingB to reading of
         * Channel B. First channel is the first channel to sample, either a or b.
         * @param readingA Variable to set to reading of channelA.
         * @param readingB Valiable to set to reading of cahnnelB.
         * @param firstChannel True = Read Channel B then A, false = Read Channel A then B.
         */
        void readChannels(uint16_t readingA, uint16_t readingB, const bool firstChannel=CHANNEL_A);
        /**
         * @brief Read channel A voltage.
         * Reads and calculates voltage given VREF in volts.
         * @param vref VREF in volts. Default=3.3v.
         * @return float Voltage in volts.
         */
        float readChannelAVoltage(const float vref=3.3f);
        /**
         * @brief Read channel B voltage.
         * Reads and calculates voltage given VREF in volts.
         * @param vref VREF in volts. Default=3.3v.
         * @return float Voltage in volts.
         */
        float readChannelBVoltage(const float vref=3.3f);
        /**
         * @brief Reads channel voltage.
         * Reads and calculates a channels voltage, given VREF in volts.
         * @param channel True = Channel B, False = Channel A.
         * @param vref VREF in volts. Default 3.3v.
         * @return float Voltage in volts.
         */
        float readChannelVoltage(const bool channel, const float vref=3.3f);
        /**
         * @brief Set the Update frequency.
         * Sets the update frequency. Valid values are 0x00-0x03. Frequency is dependant on clock freqency.
         * If an invalid frequency is passed, an error code is returned and no action is taken.
         * @param value Value to set the update frequency to. Valid values 0x00->0x03.
         * @return int16_t Returns achieved freqency in Hz if set okay, negative for error code.
         */
        int16_t setUpdateFreq(const uint8_t value);
        /**
         * @brief Enable / disable the clock output.
         * Enables / disables the clock output on the clock output pin.
         * @param value True = enable clock, False = disable clock.
         */
        void enableClockOutput(const bool value);
        /**
         * @brief Enable / disable clock division.
         * Enables / disables clock division. Ignored when using internal clock.
         * @param value True = Enable clock division. False = Disable clock division.
         */
        void enableClockDivision(const bool value);
        /**
         * @brief Set the channel gain.
         * Sets the gain for the given channel.
         * @param channel True = Channel B, False = Channel A.
         * @param value Gain value to set. Valid Values 0x00->0x07 inclusive. Can use GAIN_* for
         * appropriate values. Returns the gain factor as int if successful. Otherwise an error
         * code is returned.
         * @return int16_t Positive = Gain factor. Negative for error code.
         */
        int16_t setGain(const bool channel, const uint8_t value);
        /**
         * @brief Set the buffer setting.
         * Enables / disables buffering. True = Buffer enabled, False = buffer disabled.
         * @param channel True = Channel B, False = Channel A
         * @param value True = Buffer enabled. False = Buffer disabled.
         */
        void setBuffer(const bool channel, const bool value);
        /**
         * @brief Set the unipolar / bipolar mode.
         * Sets the unipolar / bipolar mode for a given channel.
         * @param channel True = Channel B, False = Channel A.
         * @param value True = Unipolar mode, False = Bipolar mode.
         */
        void setPolarMode(const bool channel, const bool value);
        /**
         * @brief Preform hardware reset.
         * Preforms a hardware reset if the reset pin has been defined.
         */
        void reset();
        /**
         * @brief Sets the powerdown bit.
         * Sets / clears the power down bit, and puts ADC in low power mode.
         */
        void powerDown(const bool value);
        /**
         * @brief Set the Free Running mode.
         * Starts or stops the free running mode.
         * @param channel True = Channel B, False = Channel A.
         * @param value True = Enable Free running mode, False = Disable free running mode.
         */
        void setFreeRunning(const bool channel, const bool value);
        /**
         * @brief Initialize the chip.
         * Initializes the SPI bus, and calibrates the ADC channels. Returns 0 (NO_ERROR) if initialized
         * okay, otherwise if an invalid SPI pin was passed during construction an error code is returned.
         * if an invalid reset pin, or data ready pin is passed, hardware reset, and hardware polling is
         * disabled respectivly.
         * @param initSPI True = Initialize SPI bus, False = Don't initialize SPI bus.
         * @param intClock True (default) = Enable internal clock. False = External clock.
         * @param clkFreq True (default) = Frequency of 2.4576MHz, False = Frequency of 1MHz
         * @param updateFreq The update frequency, one of the FREQ_* values. Defaults to 0x01, (FREQ_25/
         * FREQ_60HZ) depending on the Clock frequency.
         * @param clkDiv True = Use clock division, False(default) = Do not use clock division.  Ignored if
         * using the internal clock.
         * @return int16_t Returns 0 (NO_ERROR) if initialized okay, negative for error code.
         */
        int16_t initialize(const bool initSPI=true, const bool intClock=true, const bool clkFreq=CLOCK_FREQ_2MHZ,
                                const uint8_t updateFreq=FREQ_60HZ, const bool clkDiv=false);

    private:
/* ############ Private Constants ############# */
    // Comms Register Bit Masks:
        const uint8_t MASK_DRDY     = 0x80; // 0b 1--- ----
        const uint8_t MASK_REG_SEL  = 0x70; // 0b -111 ----
        const uint8_t MASK_READ_BIT = 0x08; // 0b ---- 1---
        const uint8_t MASK_PWR_DWN  = 0x04; // 0b ---- -1--
        const uint8_t MASK_CHAN_SEL = 0x03; // 0b ---- --11
    // Registers:
        const uint8_t REG_COMMS  = 0x00; // 0b -000 ---- 8 bits.
        const uint8_t REG_SETUP  = 0x10; // 0b -001 ---- 8 bits.
        const uint8_t REG_CLOCK  = 0x20; // 0b -010 ---- 8 bits.
        const uint8_t REG_DATA   = 0x30; // 0b -011 ---- 16 bits.
        const uint8_t REG_NOP    = 0x50; // 0b -101 ---- n/a
        const uint8_t REG_OFFSET = 0x60; // 0b -110 ---- 24 bits.
        const uint8_t REG_GAIN   = 0x70; // 0b -111 ---- 24 bits.
    // Setup Register Bit Masks:
        const uint8_t MASK_OPER_MODE    = 0xC0; // 0b 11-- ----
        const uint8_t MASK_GAIN_SEL     = 0x38; // 0b --11 1---
        const uint8_t MASK_POLAR_MODE   = 0x04; // 0b ---- -1--
        const uint8_t MASK_BUFF_EN      = 0x02; // 0b ---- --1-
        const uint8_t MASK_FSYNC        = 0x01; // 0b ---- ---1
    // Clock register bit masks:
        const uint8_t MASK_RESV_MXID    = 0x80; // 0b 1--- ----
        const uint8_t MASK_RESV_ZERO    = 0x40; // 0b -1-- ----
        const uint8_t MASK_INT_CLK_EN   = 0x20; // 0b --1- ----
        const uint8_t MASK_CLK_DIS      = 0x10; // 0b ---1 ----
        const uint8_t MASK_CLK_DIV      = 0x08; // 0b ---- 1---
        const uint8_t MASK_CLK_SEL      = 0x04; // 0b ---- -1--
        const uint8_t MASK_FILTER_SEL   = 0x03; // 0b ---- --11
    // Pins:
        const uint8_t _sckPin = MY_NOT_A_PIN;
        const uint8_t _misoPin = MY_NOT_A_PIN;
        const uint8_t _mosiPin = MY_NOT_A_PIN;
        const uint8_t _csPin = MY_NOT_A_PIN;
        const uint8_t _resetPin = MY_NOT_A_PIN;
        const uint8_t _drdyPin = MY_NOT_A_PIN;
/* ############# Private Variables: ################# */
        spi_inst_t *_spiPort;
        bool        _haveReset = false;
        bool        _haveDrdy = false;
        uint8_t     _commsRegData = 0x00;
        uint8_t     _clockRegData = 0x00;
        uint8_t     _setupRegData = 0x00;
        uint8_t     _freq = FREQ_60HZ;  // Update frequency in free running mode.
        bool        _extClockIn = false;   // Use external clock. True = external clock, False = internal clock.
        bool        _extClockOut = false; // Output a clock signal on the clock out pin.
        bool        _clockFreq = true; // Clock frequency, False = 1 MHz, True = 2.4576 MHz.
        bool        _clockDiv = false;  // Use clock division, ignored in internal clock mode.
        uint8_t     _channel = CHANNEL_A;
        uint8_t     _mode = MODE_NORMAL;
        uint8_t     _gain[2] = {GAIN_1, GAIN_1};
        bool        _buffered[2] = {false, false};
        bool        _unipolar[2] = {true, true};
        bool        _powerDown = false;

/* ############## Private functions: ############### */
        uint8_t __buildCommsRegData__(const uint8_t reg, const bool read);
        uint8_t __buildSetupRegData__(const bool fsync);
        uint8_t __buildClockRegData__();
        void __writeReg__(const uint8_t reg, const uint8_t value);
        uint8_t __readReg__(const uint8_t reg);
        uint16_t __readData__();
        bool __poll_sw__();
        bool __poll_hw__();
        bool __checkFreeRunning__();
};

/* ########################## Public Functions: #################### */
uint16_t myMAX1415::readChannelA() {
    uint16_t returnValue;
    _channel = CHANNEL_A;
    _mode = MODE_NORMAL;
    bool isFreeRunning = __checkFreeRunning__();
    if ( isFreeRunning == false) {
        _setupRegData = __buildSetupRegData__(false);
        __writeReg__(REG_SETUP, _setupRegData);
        if (_haveDrdy == true) {
            while (__poll_hw__() == false) {
                ; // Pass
            }
        } else {
            while (__poll_sw__() == false) {
                ; // Pass
            }
        }
    }
    returnValue = __readData__();
    if (isFreeRunning == false) {
        _setupRegData = __buildSetupRegData__(true); // Stop sampling.
        __writeReg__(REG_SETUP, _setupRegData);
    }
    return returnValue;
}

uint16_t myMAX1415::readChannelB() {
    uint16_t returnValue;
    _channel = CHANNEL_B;
    _mode = MODE_NORMAL;
    bool isFreeRunning = __checkFreeRunning__();
    if (isFreeRunning == false) {
        _setupRegData = __buildSetupRegData__(false);
        __writeReg__(REG_SETUP, _setupRegData);
        if (_haveDrdy == true) {
            while (__poll_hw__() == false) {
                ; // Pass
            }
        } else {
            while (__poll_sw__() == false) {
                ; // Pass
            }
        }
    }
    returnValue = __readData__();
    if (isFreeRunning == false) {
        _setupRegData = __buildSetupRegData__(true); // Stop sampling.
        __writeReg__(REG_SETUP, _setupRegData);
    }
    return returnValue;
}

uint16_t myMAX1415::readChannel(const bool channel) {
    if (channel == CHANNEL_A) {
        return readChannelA();
    } else {
        return readChannelB();
    }
}

void myMAX1415::readChannels(uint16_t readingA, uint16_t readingB, const bool firstChannel) {
    if (firstChannel == CHANNEL_A) {
        readingA = readChannelA();
        readingB = readChannelB();
    } else {
        readingB = readChannelB();
        readingA = readChannelA();
    }
}

float myMAX1415::readChannelAVoltage(const float vref) {
    const float conversion_factor = vref / (1 << 16); // Calculate conversion factor.
    uint16_t reading = readChannelA();
    return ((float)reading * conversion_factor);
}

float myMAX1415::readChannelBVoltage(const float vref) {
    const float conversion_factor = vref / (1 << 16); // Calculate conversion factor.
    uint16_t reading = readChannelB();
    return ((float)reading * conversion_factor);
}

float myMAX1415::readChannelVoltage(const bool channel, const float vref) {
    if (channel == CHANNEL_A) {
        return readChannelAVoltage(vref);
    } else {
        return readChannelBVoltage(vref);
    }
}

int16_t myMAX1415::setUpdateFreq(const uint8_t value) {
    if (value > 0x03) { return ERROR_INVALID_UPDATE_FREQ; }
    _freq = value;
    _clockRegData = __buildClockRegData__();
    __writeReg__(REG_CLOCK, _clockRegData);
    if (_clockFreq == true) {
        switch (_freq) {
            case FREQ_20HZ: return 20;
            case FREQ_25HZ: return 25;
            case FREQ_100HZ: return 100;
            case FREQ_200HZ: return 200;
        }
    } else {
        switch (_freq) {
            case FREQ_50HZ: return 50;
            case FREQ_60HZ: return 60;
            case FREQ_250HZ: return 250;
            case FREQ_500HZ: return 500;
        }
    }
    return NO_ERROR;
}

void myMAX1415::enableClockOutput(const bool value) {
    _extClockOut = value;
    _clockRegData = __buildClockRegData__();
    __writeReg__(REG_CLOCK, _clockRegData);
}

void myMAX1415::enableClockDivision(const bool value) {
    _clockDiv = value;
    _clockRegData = __buildClockRegData__();
    __writeReg__(REG_CLOCK, _clockRegData);
}

int16_t myMAX1415::setGain(const bool channel, const uint8_t value) {
    if (value > 0x07) { return ERROR_INVALID_GAIN; }
    _gain[channel] = value;
    if (_channel == channel) { // If selected channel is updated channel.
        if (__checkFreeRunning__() == true) {
           _setupRegData = __buildSetupRegData__(false); // Stay in free running mode.
            __writeReg__(REG_SETUP,_setupRegData);
        }
    }
    switch (_gain[channel]) {
        case GAIN_1: return 1;
        case GAIN_2: return 2;
        case GAIN_4: return 4;
        case GAIN_8: return 8;
        case GAIN_16: return 16;
        case GAIN_32: return 32;
        case GAIN_64: return 64;
        case GAIN_128: return 128;
    }
    return NO_ERROR; // Should never get here.
}

void myMAX1415::setBuffer(const bool channel, const bool value) {
    _buffered[channel] = value;
    if (_channel == channel) {
        if (__checkFreeRunning__() == true) {
            _setupRegData = __buildSetupRegData__(false); // Stay in freerunning mode.
            __writeReg__(REG_SETUP, _setupRegData);
        }
    }
}

void myMAX1415::setPolarMode(const bool channel, const bool value) {
    _unipolar[channel] = value;
    if (_channel == channel) {
        if (__checkFreeRunning__() == true) {
            _setupRegData = __buildSetupRegData__(false);
            __writeReg__(REG_SETUP, _setupRegData);
        }
    }
}

void myMAX1415::reset() {
    if (myHelpers::isPin(_resetPin) == true) {
        gpio_put(_resetPin, false);
        gpio_put(_resetPin, true);
    }
}

void myMAX1415::powerDown(const bool value) {
    _powerDown = value;
    _commsRegData = __buildCommsRegData__(REG_COMMS, false);
    __writeReg__(REG_COMMS, _commsRegData);
}

void myMAX1415::setFreeRunning(const bool channel, const bool value) {
    _channel = (uint8_t)channel;
    if (value == true) {
        _setupRegData = __buildSetupRegData__(false);
    } else {
        _setupRegData = __buildSetupRegData__(true);
    }
    __writeReg__(REG_SETUP, _setupRegData);
}

int16_t myMAX1415::initialize(const bool initSPI, const bool intClock, const bool clkFreq,
                                    const uint8_t updateFreq, const bool clkDiv) {
    int32_t returnValue;
// Init SPI:
    if (initSPI == true) {
        returnValue = mySPI::initializeMaster(_spiPort, _sckPin, _misoPin, _mosiPin, 5000*1000); // 5Mhz.
        if (returnValue < 0) { return (int16_t)returnValue; }
    }
// Init CS Pin.
    if (myHelpers::isPin(_csPin) == false) { return ERROR_INVALID_CS_PIN; }
    gpio_init(_csPin);
    gpio_set_dir(_csPin, GPIO_OUT);
    gpio_put(_csPin, true);
// Init Reset pin:
    if (myHelpers::isPin(_resetPin) == true) {
        gpio_init(_resetPin);
        gpio_set_dir(_resetPin, GPIO_OUT);
        gpio_put(_resetPin, true);
        _haveReset = true;
    }
// Init DRDY pin:
    if (myHelpers::isPin(_drdyPin) == true) {
        gpio_init(_drdyPin);
        gpio_set_dir(_drdyPin, GPIO_IN);
        _haveDrdy = true;
    }

// Initialize Clock register:
    _extClockIn = ! intClock;
    _clockFreq = clkFreq;
    _freq = updateFreq;
    _clockDiv = clkDiv;
    _clockRegData = __buildClockRegData__();
    __writeReg__(REG_CLOCK, _clockRegData);

// Calibrate channel A (1).
    _channel = CHANNEL_A; // Set channel to Channel A
    _mode = MODE_SELF_CAL;
    _setupRegData = __buildSetupRegData__(false); // Fsync false to start Calibration.
    __writeReg__(REG_SETUP, _setupRegData);
// Wait for calibration to finish:
    if (_haveDrdy == true) {
        while (__poll_hw__() == false) {
            tight_loop_contents(); // Pass.
        } 
    } else {
        while (__poll_sw__() == false) {
            tight_loop_contents(); // pass.
        }
    }
// Read Value to clear drdy.
    __readData__();
// Calibrate channel B (2).
    _channel = CHANNEL_B;
    _mode = MODE_SELF_CAL;
    _setupRegData = __buildSetupRegData__(false); // Fsync false to start calibration.
    __writeReg__(REG_SETUP, _setupRegData);
// Wait for calibration to finish:
    if (_haveDrdy == true) {
        while (__poll_hw__() == false) {
            tight_loop_contents(); // Pass.
        } 
    } else {
        while (__poll_sw__() == false) {
            tight_loop_contents(); // pass.
        }
    }
// Read Value to clear drdy.
    __readData__();
// Stop sampling:
    _mode = MODE_NORMAL;
    _setupRegData = __buildSetupRegData__(true);
    __writeReg__(REG_SETUP, _setupRegData);
    return NO_ERROR;
}


/* ###################### Private functions: ############################### */

uint8_t myMAX1415::__buildCommsRegData__(const uint8_t reg, const bool read) {
    uint8_t returnValue = 0x00;
    returnValue |= reg;
    if (read == true) {
        returnValue |= MASK_READ_BIT;
    }
    if (_powerDown == true) {
        returnValue |= MASK_PWR_DWN;
    }
    returnValue |= _channel;
    return returnValue;
}

uint8_t myMAX1415::__buildSetupRegData__(const bool fsync) {
    uint8_t returnValue = 0x00;
    returnValue |= _mode;
    returnValue |= (_gain[_channel] << 3);
    if (_buffered[_channel] == true) {
        returnValue |= MASK_BUFF_EN;
    }
    if (_unipolar[_channel] == true) {
        returnValue |= MASK_POLAR_MODE;
    }
    if (fsync == true) {
        returnValue |= MASK_FSYNC;
    }
    return returnValue;
}

uint8_t myMAX1415::__buildClockRegData__() {
    uint8_t returnValue = 0x80; // Set Reserved bits 7,6 to 1,0.
    // Clock IN:
    if (_extClockIn == false) {
        returnValue |= MASK_INT_CLK_EN;
    }
    // Clock OUT:
    if (_extClockOut == false) {
        returnValue |= MASK_CLK_DIS;
    }
    // Clock Division:
    if (_clockDiv == true) {
        returnValue |= MASK_CLK_DIV;
    }
    // Clock frequency: True = 2.4576 MHz, False = 1 MHz.
    if (_clockFreq == true) {
        returnValue |= MASK_CLK_SEL;
    }
    // Update frequency:
    returnValue |= _freq;
    return returnValue;
}

void myMAX1415::__writeReg__(const uint8_t reg, const uint8_t value) {
    uint8_t commsRegData = __buildCommsRegData__(reg, false);
    gpio_put(_csPin, false);
    spi_write_blocking(_spiPort, &commsRegData, 1);
    gpio_put(_csPin, true);
    gpio_put(_csPin, false);
    spi_write_blocking(_spiPort, &value, 1);
    gpio_put(_csPin, true);
}

uint8_t myMAX1415::__readReg__(const uint8_t reg) {
    uint8_t returnValue;
    uint8_t commsRegData = __buildCommsRegData__(reg, true);
    gpio_put(_csPin, false);
    spi_write_blocking(_spiPort, &commsRegData, 1);
    gpio_put(_csPin, true);
    gpio_put(_csPin, false);
    spi_read_blocking(_spiPort, 0x00, &returnValue, 1);
    gpio_put(_csPin, true);
    return returnValue;
}

uint16_t myMAX1415::__readData__() {
    uint16_t returnValue;
    uint8_t dataBuffer[2];
    uint8_t commsRegData = __buildCommsRegData__(REG_DATA, true);
    gpio_put(_csPin, false);
    spi_write_blocking(_spiPort, &commsRegData, 1);
    gpio_put(_csPin, true);
    gpio_put(_csPin, false);
    spi_read_blocking(_spiPort, 0x00, dataBuffer, 2);
    gpio_put(_csPin, true);
    returnValue = (uint16_t)(dataBuffer[0] << 8);
    returnValue |= (uint16_t)dataBuffer[1];
    return returnValue;
}

bool myMAX1415::__poll_hw__() {
    return ! gpio_get(_drdyPin);
}

bool myMAX1415::__poll_sw__() {
    uint8_t commsRegData = __buildCommsRegData__(REG_COMMS, true);
// Set comms reg data
    gpio_put(_csPin, false);
    spi_write_blocking(_spiPort, &commsRegData, 1);
    gpio_put(_csPin, true);
    gpio_put(_csPin,false);
    spi_read_blocking(_spiPort, 0x00, &commsRegData, 1);
    gpio_put(_csPin, true);
    return ! (bool)(commsRegData & MASK_DRDY);
}

bool myMAX1415::__checkFreeRunning__() {
    uint8_t setupData = __readReg__(REG_SETUP);
    return ! (bool)(setupData & MASK_FSYNC);
}
#endif