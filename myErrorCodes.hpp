/**
 * @file myErrorCodes.hpp
 * @author Peter Nearing (pnearing@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */
/**
 *  Define all error codes.
 *  All the error code defines for the pico-myLibs. General Error codes are from 0 -> -99, so they
 *      can be cast to any int type. All other error codes must be cast to int16_t at minimum.
 */


#ifndef MY_ERROR_CODES
    #define MY_ERROR_CODES
/************* General Error codes: 0-99 *****************/
    #ifndef MY_NO_ERROR
        /**
         * @brief No error value.
         * Value to use for no error.
         */
        #define MY_NO_ERROR 0
    #endif
    #ifndef MY_GENERAL_ERROR
        /**
         * @brief General error value.
         * Value to use if no other error applies.
         */
        #define MY_GENERAL_ERROR -1
    #endif
    /**
     * @brief invalid pin number.
     * An invalid pin number was passed.
     */
    #define MY_INVALID_PIN                                  -2
    /**
     * @brief invalid bit mask.
     * An invalid bit mask was passed.
     */
    #define MY_INVALID_BIT_MASK                             -3
    /**
     * @brief timeout reached.
     * A timeout has been reached.
     */
    #define MY_TIMEOUT                                      -4
    /**
     * @brief invalid direction
     * A pin is set to wrong direction for operation requested.
     */
    #define MY_INVALID_DIR                                  -5


/************* myADC error codes (200-249)  **************/
// *NOTE: Error codes are such a that the pico would stop operating
//          if the temperature were ever hit.
    /**
     * @brief Invalid ADC channel.
     * An invalid ADC channel was passed.
     */
    #define MY_ERROR_ADC_INVALID_CHANNEL                    -200
    /**
     * @brief Invalid pin number.
     * An invalid pin number was passed.
     */
    #define MY_ERROR_ADC_INVALID_PIN                        -201
    /**
     * @brief ADC not initialized.
     * Initialize hasn't been called prior to operation.
     */
    #define MY_ERROR_ADC_ADC_NOT_INIT                       -202
    /**
     * @brief Channel not initialized.
     * The channel has not been initialized prior to operation.
     */
    #define MY_ERROR_ADC_CHANNEL_NOT_INIT                   -203

/************* my23LC1024 error codes (300-349) **********/
    /**
     * @brief Sram held.
     * Sram is set to HOLD when operation was requested.
     */
    #define MY_ERROR_MY23LC1024_SRAM_HELD                   -300
    /**
     * @brief Invalid Address.
     * An invalid address was passed.
     */
    #define MY_ERROR_MY23LC1024_INVALID_ADDRESS             -301
    /**
     * @brief Sram busy
     * Sram was busy when operation was requested.
     */
    #define MY_ERROR_MY23LC1024_SRAM_BUSY                   -302
    /**
     * @brief Sram idle.
     * Sram was idle when operation was requested.
     */
    #define MY_ERROR_MY23LC1024_SRAM_IDLE                   -303
    /**
     * @brief Hold pin not defined.
     * The hold pin is not defined for operation or comms mode.
     */
    #define MY_ERROR_MY23LC1024_HOLD_NOT_DEFINED            -304
    /**
     * @brief Sio2 pin not defined.
     * The sio2 pin is not defined for comms mode.
     */
    #define MY_ERROR_MY23LC1024_SIO2_NOT_DEFINED            -305
    /**
     * @brief Sram not held.
     * Sram was not in hold state when operation requested.
     */
    #define MY_ERROR_MY23LC1024_SRAM_NOT_HELD               -306
    /**
     * @brief Sram not reading.
     * Sram was not in reading state when operation was requested.
     */
    #define MY_ERROR_MY23LC1024_NOT_READING                 -307
    /**
     * @brief Sram not writing.
     * Sram was not in writing state when operation was requested.
     */
    #define MY_ERROR_MY23LC1024_NOT_WRITING                 -308
    /**
     * @brief Hold not available.
     * Hold function not available in comms mode.
     */
    #define MY_ERROR_MY23LC1024_HOLD_NOT_AVAILABLE          -309
    /**
     * @brief Communication check failed.
     * Communications couldn't be verified.
     */
    #define MY_EROOR_MY23LC1024_COMM_CHECK_FAILED           -310

/*************** mySNx4HC595 error codes (400-449) ****************/
    /**
     * @brief Enable pin not defined.
     * The enable pin was not defined when operation was requested.
     */
    #define MY_ERROR_MY595_ENABLE_NOT_DEFINED               -400
    /**
     * @brief Clear pin not defined.
     * The clear pin was not defined when operation was requested.
     */
    #define MY_ERROR_MY595_CLEAR_NOT_DEFINED                -401



#endif