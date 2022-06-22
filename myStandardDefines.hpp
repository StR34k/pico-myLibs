#ifndef MY_STANDARD_DEFINES_H
    #define MY_STANDARD_DEFINES_H
/* Pin defines */
    #define MY_NOT_A_PIN 0xFF
    #define MY_NUM_PINS 30
    #define MY_MAX_PIN 29
/* error defines: Note see myErrorCodes.hpp for a full list of codes for all error codes.*/
    #ifndef MY_NO_ERROR
        #define MY_NO_ERROR                                     0
    #endif
    #ifndef MY_GENERAL_ERROR
        #define MY_GENERAL_ERROR                                -1
    #endif

/* Bit orders: */
    #ifndef MSB_FIRST
        #define MSB_FIRST 1
    #endif
    #ifndef LSB_FIRST
        #define LSB_FIRST 0
    #endif
/* Macros: */
    // BV
    #ifndef _BV
        #define _BV(bit) (1 << (bit))
    #endif
#endif