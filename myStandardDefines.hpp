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
    #ifdef RASPBERRYPI_PICO
        #define MY_MAX_PIN 28
    #else
        #define MY_MAX_PIN 29
    #endif
    /**
     * @brief Direction OUTPUT.
     * The value to set, or the value read for output.
     */
    #define MY_DIR_OUTPUT 1
    /**
     * @brief Direction INPUT.
     * The value to set, or the value read for input.
     */
    #define MY_DIR_INPUT 0
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
    #ifndef MY_VALUE_ERROR
        /**
         * @brief Value error.
         * A value is out of range.
         */
        #define MY_VALUE_ERROR                                  -9
    #endif
    #ifndef MY_INVALID_FUNC
        /**
         * @brief Invalid function.
         * A pin function is not set properly.
         */
        #define MY_INVALID_FUNC                                 -10
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
 * ############################# Date / Time defines: ###########################
 */

    /**
     * @brief Number of days in a common year.
     */
    #define MY_DAYS_PER_YEAR 365
    /**
     * @brief Number of days in a leap year.
     */
    #define MY_DAYS_PER_LEAP_YEAR 366
    /**
     * @brief Minimum second value.
     */
    #define MY_MIN_SECOND 0
    /**
     * @brief Maximum second value.
     */
    #define MY_MAX_SECOND 59
    /**
     * @brief Minimim minute value.
     */
    #define MY_MIN_MINUTE 0
    /**
     * @brief Maximum minute value.
     */
    #define MY_MAX_MINUTE 59
    /**
     * @brief Minimum hour in 12h format.
     */
    #define MY_MIN_HOUR_12H 1
    /**
     * @brief Maximum hour in 12h format.
     */
    #define MY_MAX_HOUR_12H 12
    /**
     * @brief AM value. (false)
     */
    #define MY_IS_AM 0
    /**
     * @brief PM value. (true)
     */
    #define MY_IS_PM 1
    /**
     * @brief Minimum hour value in 24h format.
     */
    #define MY_MIN_HOUR_24H 0
    /**
     * @brief Maximum hour value in 24h format.
     */
    #define MY_MAX_HOUR_24H 23
    /**
     * @brief Minimum day of the week value.
     */
    #define MY_MIN_DOTW 1
    /**
     * @brief Maximum day of the week value.
     */
    #define MY_MAX_DOTW 7
    /**
     * @brief Minimum day of the month value.
     */
    #define MY_MIN_DATE 1
    /**
     * @brief Maximum day of the month value.
     */
    #define MY_MAX_DATE 31
    /**
     * @brief Minimum month value.
     */
    #define MY_MIN_MONTH 1
    /**
     * @brief Maximum month value.
     */
    #define MY_MAX_MONTH 12
    /**
     * @brief Months per year.
     */
    #define MY_MONTHS_PER_YEAR 12
    /**
     * @brief Minimum year value with Centuries, IE: 2020.
     */
    #define MY_MIN_YEAR 100
    /**
     * @brief Maximum year value with centuries, IE: 2202. NOTE: Centuries and year can be tracked
     * using 2 int8_t variables, one for centuries, and one for years. Valid Centuries would be 0-127,
     * and valid years are 0-99 giving us a max year of 127*100+99 = 12799. You can also track this value
     * with a single int16_t allowing negative values.
     */
    #define MY_MAX_YEAR 12799
    /**
     * @brief Is 24 hour format (false).
     */
    #define MY_IS_24H 0
    /**
     * @brief Is 12 hour format (true).
     */
    #define MY_IS_12H 1
    /**
     * @brief Years per century.
     */
    #define MY_YEARS_PER_CENTURY 100
    /**
     * @brief Min century value.
     */
    #define MY_MIN_CENTURY 0
    /**
     * @brief Max century value.
     */
    #define MY_MAX_CENTURY 127
    /**
     * @brief Default century if year in range of 00-99.
     */
    #define MY_DEFAULT_CENTURY 20
    /**
     * @brief Month of January.
     */
    #define MY_MONTH_JAN 1
    /**
     * @brief Month of Febuary.
     */
    #define MY_MONTH_FEB 2
    /**
     * @brief Month of March.
     */
    #define MY_MONTH_MAR 3
    /**
     * @brief Month of April.
     */
    #define MY_MONTH_APR 4
    /**
     * @brief Month of May.
     */
    #define MY_MONTH_MAY 5
    /**
     * @brief Month of June.
     */
    #define MY_MONTH_JUN 6
    /**
     * @brief Month of July.
     */
    #define MY_MONTH_JUL 7
    /**
     * @brief Month of August.
     */
    #define MY_MONTH_AUG 8
    /**
     * @brief Month of September. 
     */
    #define MY_MONTH_SEP 9
    /**
     * @brief Month of October.
     */
    #define MY_MONTH_OCT 10
    /**
     * @brief Month of November.
     */
    #define MY_MONTH_NOV 11
    /**
     * @brief Month of December.
     */
    #define MY_MONTH_DEC 12

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