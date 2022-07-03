/**
 * Standard defines and helper macros.
 * Set some defines used by pico-myLibs. Some error values overlap with myErrorCodes.hpp, values here take
 * precidence over myErrorCodes.hpp
 *  
 * @file myStandardDefines.hpp
 * @author Peter Nearing (pnearing@protonmail.com)
 * @brief Standard defines for pico-myLibs
 * @version 0.1
 * @date 2022-06-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/**
 * @brief Standard defines:
 * Set some defines used by pico-myLibs. Some error values overlap with myErrorCodes.hpp, values here take
 * precidence over myErrorCodes.hpp
  */
#ifndef MY_STANDARD_DEFINES_H
    #define MY_STANDARD_DEFINES_H
/*
 * ############################# General Pin defines  ############################
 */
    /**
     * @brief Not a pin value.
     * Value to use for an undefined pin, fails myHelpers::isPin()
     */
    #define MY_NOT_A_PIN 0xFF
    /**
     * @brief Number of gpio pins.
     * Number of GPIO pins on the rp2040
     */
    #define MY_NUM_PINS 30
    /**
     * @brief Max valid pin number.
     * The highest pin number on the rp2040.
     */
    #define MY_MAX_PIN 29
/*
 * ################################ ADC Pin Defines:##################################
 */
    /**
     * @brief Channel 0 pin.
     * The pin number for ADC channel 0.
     */
    #define MY_ADC_CHANNEL_0_PIN    26
    /**
     * @brief Channel 1 pin.
     * The pin number for ADC channel 1.
     */
    #define MY_ADC_CHANNEL_1_PIN    27
    /**
     * @brief Channel 2 pin.
     * The pin number for ADC channel 2.
     */
    #define MY_ADC_CHANNEL_2_PIN    28
    /**
     * @brief Channel 3 pin.
     * The pin number for ADC channel 3.
     */
    #define MY_ADC_CHANNEL_3_PIN    29
/*
 * ############################ ADC Channel Defines: ########################
 */
    /**
     * @brief ADC Channel 0
     * The channel number for adc user channel 0
     */
    #define MY_ADC_CHANNEL_0        0
    /**
     * @brief ADC Channel 1
     * The channel number for adc user channel 1
     */
    #define MY_ADC_CHANNEL_1        1
    /**
     * @brief ADC Channel 2
     * The channel number for adc user channel 2
     */
    #define MY_ADC_CHANNEL_2        2
    /**
     * @brief ADC Channel 3
     * The channel number for adc user channel 3
     */
    #define MY_ADC_CHANNEL_3        3
    /**
     * @brief Temperature channel.
     * The channel number for the internal temerature sensor.
     */
    #define MY_ADC_TEMP_CHANNEL     4
    /**
     * @brief Number of user channels.
     * The number for user channels (ie channels mapped to pins) on the rp2040.
     */
    #define MY_ADC_NUM_USER_CHANNELS    4
    /**
     * @brief Number of ADC channels.
     * The total number of ADC channels, including the internal temperature sensor for the rp2040.
     */
    #define MY_ADC_NUM_CHANNELS     5
/*
 * ############################ PWM Defines #####################################
 */
    /**
     * @brief Number of slices.
     * Number of PWM slices on the rp2040. There are 8 slices, with 2 channels each.
     */
    #define MY_PWM_NUM_SLICE    8
    /**
     * @brief Number of channels per slice.
     * Number of PWM channels per slice on the rp2040. There are 8 slices with 2 channels each.
     */
    #define MY_PWM_NUM_CHANNEL  2
    /**
     * @brief Default PWM frequency.
     * The defalt PWM frequency in hz.
     */
    #define MY_PWM_DEFAULT_FREQ 125000000

/*
 * ############################# SPI Defines: ###############################
 */
    /**
     * @brief Number of spi ports.
     * Number of spi ports on the rp2040. There are 2 ports.
     */
    #define MY_SPI_NUM_PORTS 2
    /**
     * @brief Number of possible spi pins.
     * Number of possible options for each type of spi pin. IE: SPI0 RX pin can be set to 1 of 4 pins.
     * @note Some pins on SPI port 1 only have 3 options.
     */
    #define MY_SPI_NUM_PINS 4
/*
 * ################################ I2C Defines: ###################################
 */

    /**
     * @brief Number of I2C ports on the rp2040
     * Number of I2C ports on the rp2040, there are 2.
     */
    #define MY_I2C_NUM_PORTS 2
    /**
     * @brief Number of possible I2C pins.
     * Number of possible options for each type of I2C pin. IE: I2C0 SDA can be set to 1 of 8 pins.
     * @note I2C port 1 only has 5 possible options.
     */
    #define MY_I2C_NUM_PINS 8
/* 
 * ############################## Error Defines: ####################################
 * Note see myErrorCodes.hpp for a full list of codes for all error codes.
 */
    #ifndef MY_NO_ERROR
        /**
         * @brief No error value.
         * Value to use to represent no error.
         */
        #define MY_NO_ERROR 0
    #endif
    #ifndef MY_GENERAL_ERROR
        /**
         * @brief General error value.
         * General error for when no other error code applies.
         */
        #define MY_GENERAL_ERROR                                -4
    #endif
    #ifndef MY_INVALID_PIN
        /**
         * @brief invalid pin number.
         * An invalid pin number was passed.
         */
        #define MY_INVALID_PIN                                  -5
    #endif
    #ifndef MY_INVALID_BIT_MASK
        /**
         * @brief invalid bit mask.
         * An invalid bit mask was passed.
         */
        #define MY_INVALID_BIT_MASK                             -6
    #endif
    #ifndef MY_TIMEOUT
        /**
         * @brief Timeout reached.
         * A timeout has been reached.
         */
        #define MY_TIMEOUT                                      -7
    #endif
    #ifndef MY_INVALID_DIR
        /**
         * @brief Invalid direction
         * A pin is set to wrong direction for operation requested.
         */
        #define MY_INVALID_DIR                                  -8
    #endif

/* 
 * ################################ Bit orders: ##############################
 */
    #ifndef MSB_FIRST
        /**
         * @brief bit order of MSB first.
         */
        #define MSB_FIRST 1
    #endif
    #ifndef LSB_FIRST
        /**
         * @brief bit order of LSB first.
         */
        #define LSB_FIRST 0
    #endif
/* 
 * ################################### Macros: #################################
 */
    #ifndef _BV
        /**
         * @brief Create a bit mask given a bit number.
         */
        #define _BV(bit) (1 << (bit))
    #endif
#endif