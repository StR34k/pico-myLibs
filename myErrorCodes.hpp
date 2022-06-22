/*
 *  myErrorCodes
 *      All the error code defines for the myLibs
 */


#ifndef MY_ERROR_CODES
    #define MY_ERROR_CODES


//************* General Error codes: 0-99 *****************//
    #ifndef MY_NO_ERROR
        #define MY_NO_ERROR                                  0
    #endif
    #ifndef MY_GENERAL_ERROR
        #define MY_GENERAL_ERROR                            -1
    #endif
    #define MY_INVALID_PIN                                  -2  // An invalid pin number was passed.
    #define MY_INVALID_BIT_MASK                             -3  // An invalid bit mask was passed.
    #define MY_TIMEOUT                                      -4  // A time out has been reached.
    #define MY_INVALID_DIR                                  -5  // A pin is set to wront direction for operation requested.


//************* myADC error codes (200-249)  **************//
// *NOTE: Error codes are such a that the pico would stop operating
//          if the temperature were ever hit.
    #define MY_ERROR_ADC_INVALID_CHANNEL                    -200
    #define MY_ERROR_ADC_INVALID_PIN                        -201
    #define MY_ERROR_ADC_ADC_NOT_INIT                       -202
    #define MY_ERROR_ADC_CHANNEL_NOT_INIT                   -203

//************* my23LC1024 error codes (300-349) **********//
    #define MY_ERROR_MY23LC1024_SRAM_HELD                   -300
    #define MY_ERROR_MY23LC1024_INVALID_ADDRESS             -301
    #define MY_ERROR_MY23LC1024_SRAM_BUSY                   -302
    #define MY_ERROR_MY23LC1024_SRAM_IDLE                   -303
    #define MY_ERROR_MY23LC1024_HOLD_NOT_DEFINED            -304
    #define MY_ERROR_MY23LC1024_SIO2_NOT_DEFINED            -305
    #define MY_ERROR_MY23LC1024_SRAM_NOT_HELD               -306
    #define MY_ERROR_MY23LC1024_NOT_READING                 -307
    #define MY_ERROR_MY23LC1024_NOT_WRITING                 -308
    #define MY_ERROR_MY23LC1024_HOLD_NOT_AVAILABLE          -309
    #define MY_EROOR_MY23LC1024_COMM_CHECK_FAILED           -310

//***************** myHC-12 error codes (350-399) ****************//
    #define MY_ERROR_MYHC12_INVALID_BAUD                    -350
    #define MY_ERROR_MYHC12_INVALID_CHANNEL                 -351
    #define MY_ERROR_MYHC12_INVALID_MODE                    -352
    #define MY_ERROR_MYHC12_INVALID_POWER                   -353


// *************** mySNx4HC595 error codes (400-449) ****************//
    #define MY_ERROR_MY595_ENABLE_NOT_DEFINED               -400
    #define MY_ERROR_MY595_CLEAR_NOT_DEFINED                -401



#endif