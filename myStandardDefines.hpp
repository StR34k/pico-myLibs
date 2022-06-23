/**
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
 * Standard defines and helper macros.
 * Set some defines used by pico-myLibs. Some error values overlap with myErrorCodes.hpp, values here take
 * precidence over myErrorCodes.hpp
 */
#ifndef MY_STANDARD_DEFINES_H
    #define MY_STANDARD_DEFINES_H
/* Pin defines */
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
/* error defines: Note see myErrorCodes.hpp for a full list of codes for all error codes.*/
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
        #define MY_GENERAL_ERROR -1
    #endif

/* Bit orders: */
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
/* Macros: */
    #ifndef _BV
        /**
         * @brief Create a bit mask given a bit number.
         */
        #define _BV(bit) (1 << (bit))
    #endif
#endif