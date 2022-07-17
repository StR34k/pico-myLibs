/**
 * @file myMCP49x2.hpp
 * @author Peter Nearing (pnearing@protonmail.com)
 * @brief High level class for controling MCP4902/4912/4922 DAC.
 * @version 0.1
 * @date 2022-07-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MY_MCP49x2_H
#define MY_MCP49x2_H
#include <hardware/gpio.h>
#include "../myStandardDefines.hpp"
#include "../myErrorCodes.hpp"
#include "../myHelpers.hpp"
#include "../mySPI/mySPI.hpp"
// #include "../myBitBangSPI/myBitBangSPI.hpp"
/**
 * @brief High level class for controling MCP4902/4912/4922.
 * Class to control a MCP49x2 DAC
 */
class myMCP49x2 {
    public:
/* ################### Enums: ######################## */
        /**
         * @brief Models This class works with.
         * Chip model numbers.
         */
        enum models { MODEL_MCP4902, MODEL_MCP4912, MODEL_MCP4922 };
/* ##################### Public Constants: ################### */
    // Channels:
        /**
         * @brief Channel A value.
         */
        static const bool CHANNEL_A = false;
        /**
         * @brief Channel B value.
         */
        static const bool CHANNEL_B = true;
    // Values:
        /**
         * @brief Minimum value.
         */
        static const uint16_t MIN_VALUE     = 0x0000;   // 0b ---- 0000 0000 0000
        /**
         * @brief Maximum value for MCP4902.
         */
        static const uint16_t MAX_VALUE_02  = 0x00FF;   // 0b ---- ---- 1111 1111
        /**
         * @brief Maximum value for MCP4912.
         */
        static const uint16_t MAX_VALUE_12  = 0x03FF;   // 0b ---- --11 1111 1111 
        /**
         * @brief Maximum value for MCP4922.
         */
        static const uint16_t MAX_VALUE_22  = 0x0FFF;   // 0b ---- 1111 1111 1111
    // Errors:
        /**
         * @brief No Error. Value 0.
         * No error occured.
         */
        static const int16_t NO_ERROR = MY_NO_ERROR;
        /**
         * @brief Invalid pin. Value -5.
         * Invalid pin number was passed.
         */
        static const int16_t ERROR_INVALID_PIN = MY_INVALID_PIN;
        /**
         * @brief Value error. Value -650.
         * Value out of range.
         */
        static const int16_t ERROR_VALUE_ERROR = MY_ERROR_MYMCP49X2_VALUE_ERROR;
/* #################### Constructors: ###################### */
        
        /**
         * @brief Construct a new my MCP49x2 object. Bit banged SPI.
         * Construct the object, uses bit banged spi. Load and shutdown pins are undefined.
         * @param sckPin Clock pin.
         * @param mosiPin Mosi pin.
         * @param csPin Chip select pin.
         */
        myMCP49x2(const uint8_t sckPin, const uint8_t mosiPin, const uint8_t csPin) : _sckPin (sckPin),
                        _mosiPin (mosiPin), _csPin (csPin) {
            _useHWSPI = false;
        }
        /**
         * @brief Construct a new my MCP49x2 object, bit banged SPI, with load pin.
         * Construct the object. Uses bit banged SPI, Load pin is defined, while shutdown is not.
         * @param sckPin Clock pin.
         * @param mosiPin Mosi pin.
         * @param csPin Chip select pin.
         * @param loadPin Load pin.
         */
        myMCP49x2(const uint8_t sckPin, const uint8_t mosiPin, const uint8_t csPin, const uint8_t loadPin) :
                        _sckPin (sckPin), _mosiPin (mosiPin), _csPin (csPin), _loadPin (loadPin) {
            _useHWSPI = false;
        }
        /**
         * @brief Construct a new my MCP49x2 object, bit banged SPI, with load and shutdown pins.
         * Construct the object. Uses bitbanged SPI. Load and shutdown pins are defined.  If the load
         * functionality is not desired, the load pin can be set to an invalid value, IE: MY_NOT_A_PIN,
         * to disable.
         * @param sckPin Clock pin.
         * @param mosiPin Mosi pin.
         * @param csPin Chip select pin.
         * @param loadPin Load pin.
         * @param shdnPin Shutdown pin.
         */
        myMCP49x2(const uint8_t sckPin, const uint8_t mosiPin, const uint8_t csPin, const uint8_t loadPin,
                        const uint8_t shdnPin) : _sckPin (sckPin), _mosiPin (mosiPin), _csPin (csPin),
                        _loadPin (loadPin), _shdnPin (shdnPin) {
            _useHWSPI = false;
        }
        /**
         * @brief Construct a new my MCP49x2 object. HW SPI, no load or shutdown.
         * Construct the object. Uses hardware SPI, load and shutdown are undefined.
         * @param spiPort HW SPI port instance.
         * @param sckPin Clock pin.
         * @param misoPin Miso pin.
         * @param mosiPin Mosi pin.
         * @param csPin Chip select pin.
         */
        myMCP49x2(spi_inst_t *spiPort, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin,
                        const uint8_t csPin) : _sckPin (sckPin), _misoPin (misoPin), _mosiPin (mosiPin),
                        _csPin (csPin) {
            _spiPort = spiPort;
            _useHWSPI = true;
        }
        /**
         * @brief Construct a new my MCP49x2 object, HW SPI with load pin.
         * Construct the object. Uses hardware SPI, with a load pin.
         * @param spiPort HW SPI port instance.
         * @param sckPin Clock pin.
         * @param misoPin Miso pin.
         * @param mosiPin Mosi pin.
         * @param csPin Chip select pin.
         * @param loadPin Load pin.
         */
        myMCP49x2(spi_inst_t *spiPort, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin,
                        const uint8_t csPin, const uint8_t loadPin) : _sckPin (sckPin), _misoPin (misoPin),
                        _mosiPin (mosiPin), _csPin (csPin), _loadPin (loadPin) {
            _spiPort = spiPort;
            _useHWSPI = true;
        }
        /**
         * @brief Construct a new my MCP49x2 object, HW SPI with load and shutdown pins.
         * Construct the object. Uses hardware SPI, with load and shutdown pins, if the load pin functionality
         * isn't desired, then it can be set to an invalid value, IE: MY_NOT_A_PIN, to disable it
         * @param spiPort HW spi port instance.
         * @param sckPin Clock pin.
         * @param misoPin Miso pin.
         * @param mosiPin Mosi pin.
         * @param csPin Chip select pin.
         * @param loadPin Load pin.
         * @param shdnPin Shutdown pin.
         */
        myMCP49x2(spi_inst_t *spiPort, const uint8_t sckPin, const uint8_t misoPin, const uint8_t mosiPin,
                        const uint8_t csPin, const uint8_t loadPin, const uint8_t shdnPin) : 
                        _sckPin (sckPin), _misoPin(misoPin), _mosiPin(mosiPin), _csPin (csPin),
                        _loadPin (loadPin), _shdnPin (shdnPin) {
            _spiPort = spiPort;
            _useHWSPI = true;
        }
/* ################## Public Functions: #################### */
        
        /**
         * @brief Check value.
         * Check that a given value is valid for the model initialized.
         * @param value Value to check.
         * @return true Value is valid.
         * @return false Value is invalid
         */
        bool isValidValue(const int16_t value);
        /**
         * @brief Get the Channel A value.
         * Returns the value set for channel A.
         * @return uint16_t Value of channel A.
         */
        int16_t getChannelA();
        /**
         * @brief Set the Channel A Value.
         * Sets the value for channel A. Valid values are dependant on model number initialized. Returns
         * 0 (NO_ERROR) if set okay, otherwise if an invalid value is passed an error code is returned.
         * @param value Value to set DAC channel A to.
         * @return int16_t Returns 0 (NO_ERROR) if set okay, negative for error code.
         */
        int16_t setChannelA(const int16_t value);
        /**
         * @brief Get the Channel B value.
         * Returns the value of channel B.
         * @return uint16_t Value of channel B.
         */
        int16_t getChannelB();
        /**
         * @brief Set the Channel B Value.
         * Sets the value for channel B. Valid values are dependant on model number initialized. Returns
         * 0 (NO_ERROR) if set okay, otherwise if an invalid value is passed, an error code is returned.
         * @param value Value to set DAC channel B to.
         * @return int16_t Returns 0 (NO_ERROR) if set okay, negative for error code.
         */
        int16_t setChannelB(const int16_t value);
        /**
         * @brief Set both channels.
         * Sets both channels to the given values. The order in which they are set is determined by the 
         * firstChannel parameter. Returns 0 (NO_ERROR) if set okay, otherwise if an invalid value is passed
         * an error code is returned.
         * @param valueA Value for channel A.
         * @param valueB Value for channel B.
         * @param firstChannel False = Channel A first. True = Channel B first.
         * @return int16_t Returns 0 (NO_ERROR) if set okay, negative for error code.
         */
        int16_t setChannels(const int16_t valueA, const int16_t valueB, const bool firstChannel=CHANNEL_A);
        /**
         * @brief Load the DAC
         * Loads values into the DAC if the load / LDAC pin is defined. Returns 0 (NO_ERROR) if loaded okay
         * otherwise if the load pin is not defined, an error code is returned.
         * @return int16_t Retuns 0(NO_ERROR) for load okay, negative for error code.
         */
        int16_t load();
        /**
         * @brief Set the VREF input buffer.
         * Sets the Buffer bit for the Vref input buffer. Defaults to unbuffered mode. If the load
         * functionality is enabled, then load must be called for this to take effect.
         * @param channel Channel to set the buffer for.
         * @param value True = Buffered, False = Unbuffered.
         */
        void setBuffer(const bool channel, const bool value);
        /**
         * @brief Set the gain.
         * Sets the gain for a given channel. False = gain disabled(x1), True = gain enabled(x2).
         * Defaults to disabled. If the load functionality is enabled, then load must be called for this
         * to take effect.
         * @param channel Channel to set gain for.
         * @param value True = enabled, False = disabled.
         */
        void setGain(const bool channel, const bool value);
        /**
         * @brief Enables / disables a channel.
         * Sets the shdn bit. True = channel enabled, False = channel disabled. Defaults to true. If the
         * load functionality is enabled, then load must be called for this to take effect.
         * @param channel Channel to enable/disable.
         * @param value True = channel enabled, False = channel disabled.
         */
        void enableChannel(const bool channel, const bool value);
        /**
         * @brief Enable / disable the DAC.
         * Uses the shutdown pin to enable / disable the DAC. Returns 0 (NO_ERROR) if set okay, otherwise
         * if the shutdown pin hasn't been defined, an error code is returned. Defaults to enabled.  When
         * the shutdown pin is used, both channels are reset to 0. Use resetValue, and firstChannel to 
         * set the value back to last value set. If the load functionality is enabled, then load must be 
         * called for the channels to be reset.
         * @param value True = DAC enabled, False = DAC disabled.
         * @param resetValue True = Set the value to last value set, False = do not set value.
         * @param firstChannel First channel to reset to last state.
         * @return int16_t Returns 0 (NO_ERROR) if set okay, negative for error code.
         */
        int16_t enableDAC(const bool value, const bool resetValue=true, const bool firstChannel=CHANNEL_A);
        /**
         * @brief Initialze chip.
         * Initialize the chip. Returns 0 (NO_ERROR) for initialized okay, otherwise if an invalid
         * pin is passed during construction an error code is returned.
         * @param model Model number, one of MODEL_* values.
         * @param initSPI True = init spi bus, False assumes it's already initialized at max 20MHz.
         * @return int16_t Returns 0 for initialized okay. Negative for error code.
         */
        int16_t initialize(const models model, const bool initSPI=true);
    private:
/* ############### Private Constants: ########################## */
    // Masks:
        static const uint16_t AB_MASK       = 0x8000;   // 0b 1000 ---- ---- ----
        static const uint16_t BUFF_MASK      = 0x4000;   // 0b 0100 ---- ---- ----
        static const uint16_t GAIN_MASK     = 0x2000;   // 0b 0010 ---- ---- ----
        static const uint16_t SHDN_MASK     = 0x1000;   // 0b 0001 ---- ---- ----
    // Pins:
        const uint8_t _sckPin = MY_NOT_A_PIN;      // Clock
        const uint8_t _misoPin = MY_NOT_A_PIN;     // Miso
        const uint8_t _mosiPin = MY_NOT_A_PIN;     // Mosi
        const uint8_t _csPin = MY_NOT_A_PIN;       // Chip select. acitve low
        const uint8_t _loadPin = MY_NOT_A_PIN;     // LDAC / Load DAC pin. active low.
        const uint8_t _shdnPin = MY_NOT_A_PIN;     // Shutdown pin. active low.
/* ################# Private Variables ####################### */
    // SPI 
        spi_inst_t *_spiPort;
        bool _useHWSPI = true;
    // Model:
        models _model;
    // Settings:
        bool _buff[2] = {false, false}; // Buffered. True = buffered, False = unbuffered.
        bool _gain[2] = {false, false}; // If gain is enabled. True = enabled.
        bool _shdn[2] = {true, true}; // Shutdown active low. (true = enabled.)
    // Values:
        uint16_t _values[2];
/* ##################### Private functions: ############################ */

        uint16_t __getSettings__(const bool channel);
        uint16_t __shiftValue__(const uint16_t value);
        void __writeDAC__(const bool channel);
};




/* ################################## Public Functions: ##################################### */

bool myMCP49x2::isValidValue(const int16_t value) {
    if (value < 0) { return false; }
    switch (_model) {
        case MODEL_MCP4902:
            if (value > MAX_VALUE_02) { return false; }
            break;
        case MODEL_MCP4912:
            if (value > MAX_VALUE_12) { return false; }
            break;
        case MODEL_MCP4922:
            if (value > MAX_VALUE_22) { return false; }
            break;
    }
    return true;
}

int16_t myMCP49x2::getChannelA() {
    return (int16_t)_values[0];
}

int16_t myMCP49x2::setChannelA(const int16_t value) {
    if (isValidValue(value) == false) { return ERROR_VALUE_ERROR; }
    _values[0] = value;
    __writeDAC__(CHANNEL_A);
    return NO_ERROR;
}

int16_t myMCP49x2::getChannelB() {
    return (int16_t)_values[1];
}

int16_t myMCP49x2::setChannelB(const int16_t value) {
    if (isValidValue(value) == false) { return ERROR_VALUE_ERROR; }
    _values[1] = value;
    __writeDAC__(CHANNEL_B);
    return NO_ERROR;
}

int16_t myMCP49x2::setChannels(const int16_t valueA, const int16_t valueB, const bool firstChannel) {
    if (isValidValue(valueA) == false) { return ERROR_VALUE_ERROR; }
    if (isValidValue(valueB) == false) { return ERROR_VALUE_ERROR; }
    _values[0] = valueA;
    _values[1] = valueB;
    if (firstChannel == CHANNEL_A) {
        __writeDAC__(CHANNEL_A);
        __writeDAC__(CHANNEL_B);
    } else {
        __writeDAC__(CHANNEL_B);
        __writeDAC__(CHANNEL_A);
    }
    return NO_ERROR;
}

int16_t myMCP49x2::load() {
    if (myHelpers::isPin(_loadPin) == false) { return ERROR_INVALID_PIN; }
    gpio_put(_loadPin, false);
    gpio_put(_loadPin, true);
    return NO_ERROR;
}

void myMCP49x2::setBuffer(const bool channel, const bool value) {
    _buff[channel] = value;
    __writeDAC__(channel);
}

void myMCP49x2::setGain(const bool channel, const bool value) {
    _gain[channel] = value;
    __writeDAC__(channel);
}

void myMCP49x2::enableChannel(const bool channel, const bool value) {
    _shdn[channel] = value;
    __writeDAC__(channel);
}

int16_t myMCP49x2::enableDAC(const bool value, const bool resetValue, const bool firstChannel) {
    if (myHelpers::isPin(_shdnPin) == false) { return ERROR_INVALID_PIN; }
    gpio_put(_shdnPin, value);
    if (value == true and resetValue == true) {
        if (firstChannel == CHANNEL_A) {
            __writeDAC__(CHANNEL_A);
            __writeDAC__(CHANNEL_B);
        } else {
            __writeDAC__(CHANNEL_B);
            __writeDAC__(CHANNEL_A);
        }
    }
    return NO_ERROR;
}

int16_t myMCP49x2::initialize(const models model, const bool initSPI) {
    int16_t returnValue;
    uint32_t pinMask;
    _model = model; // Store model #
// Init SPI:
    if (initSPI == true) {
        if (_useHWSPI == true) {
            returnValue = mySPI::initializeMaster(_spiPort, _sckPin, _misoPin, _mosiPin, 20000*1000);
            if (returnValue < 0) { return returnValue; }
        } else {
        // Check pins:
            if (myHelpers::isPin(_sckPin) == false) { return ERROR_INVALID_PIN; }
            if (myHelpers::isPin(_mosiPin) == false) { return ERROR_INVALID_PIN; }
        // Add pins to bitMask:
            pinMask |= (1 << _sckPin) | ( 1 << _mosiPin);
        }
    }
// Check pins:
    if (myHelpers::isPin(_csPin) == false) { return ERROR_INVALID_PIN; }
    pinMask |= (1 << _csPin);
    if (myHelpers::isPin(_loadPin) == true) { pinMask |= (1 << _loadPin); }
    if (myHelpers::isPin(_shdnPin) == true) { pinMask |= (1 << _shdnPin); }
// Init GPIO:
    gpio_init_mask(pinMask);
    gpio_set_dir_out_masked(pinMask);
    if (_useHWSPI == false) {
        pinMask &= ~(1 << _sckPin); // Leave pin low.
    }
    gpio_put_masked(pinMask, pinMask); // Set pins high.
    return NO_ERROR;
}

/* ########################## Private functions: ################################### */


uint16_t myMCP49x2::__getSettings__(const bool channel) {
    uint16_t settings;
    uint8_t index = (uint8_t)channel;
    if (channel == true) { settings |= AB_MASK; }
    if (_buff[index] == true) { settings |= BUFF_MASK; }
    if (_gain[index] == true) { settings |= GAIN_MASK; }
    if (_shdn[index] == true) { settings |= SHDN_MASK; }
    return settings;
}

uint16_t myMCP49x2::__shiftValue__(const uint16_t value) {
    uint16_t shfitedValue;
    switch (_model) {
        case MODEL_MCP4902:
            shfitedValue = value << 4;
            break;
        case MODEL_MCP4912:
            shfitedValue = value << 2;
            break;
        case MODEL_MCP4922:
            shfitedValue = value;    
            break;
    }
    return shfitedValue;
}

void myMCP49x2::__writeDAC__(const bool channel) {
    int16_t returnValue;
    uint8_t dataBuffer[2];
    uint8_t index = (uint8_t)channel;
    uint16_t outValue = __getSettings__(channel);
    outValue |= __shiftValue__(_values[index]);
    dataBuffer[0] = (uint8_t)(outValue >> 8);
    dataBuffer[1] = (uint8_t)outValue;
    gpio_put(_csPin, false);
    if (_useHWSPI == true) {
        spi_write_blocking(_spiPort, dataBuffer, 2);
    } else {
        returnValue = myHelpers::shiftOut(_sckPin, _mosiPin, dataBuffer, 2);
        if (returnValue < 0) {
            printf("Shift out error code: %i\n", returnValue);
        }
    }
    gpio_put(_csPin, true);
}





#endif