/**
 *  Define all error codes.
 *  All the error code defines for the pico-myLibs. General Error codes are from 0 -> -99, so they
 *      can be cast to any int type. All other error codes must be cast to int16_t at minimum.
 * @file myErrorCodes.hpp
 * @author Peter Nearing (pnearing@protonmail.com)
 * @brief Define all error codes used in pico-myLibs
 * @version 0.1
 * @date 2022-06-23
 * 
 * @copyright Copyright (c) 2022
 * 
 *
 */

#ifndef MY_ERROR_CODES_H
    #define MY_ERROR_CODES_H
/************* General Error codes: 4-49 NOTE: error codes -1->-4 are used in <pico/error.h>*****************/
    #ifndef MY_NO_ERROR
        /**
         * @brief No error value.
         * Value to use for no error.
         */
        #define MY_NO_ERROR                                      0
    #endif
    #ifndef MY_GENERAL_ERROR
        /**
         * @brief General error value.
         * Value to use if no other error applies.
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
/************** myHelpers error codes (50-99) **********************/

    /**
     * @brief Invalid second.
     * A second value is invalid.
     */
    #define MY_HELPERS_INVALID_SECOND                       -50
    /**
     * @brief Invalid minute.
     * A minute valud is invalid.
     */
    #define MY_HELPERS_INVALID_MINUTE                       -51
    /**
     * @brief Invalid hour.
     * A hour value is invalid.
     */
    #define MY_HELPERS_INVALID_HOUR                         -52
    /**
     * @brief Invalid DOTW.
     * Invalid Day of the week value.
     */
    #define MY_HELPERS_INVALID_DOTW                         -53
    /**
     * @brief Invalid date.
     * Invalid day of the month value.
     */
    #define MY_HELPERS_INVALID_DATE                         -54
    /**
     * @brief Invalid month.
     * A month value is invalid.
     */
    #define MY_HELPERS_INVALID_MONTH                        -55
    /**
     * @brief Invalid year.
     * A year value is invalid.
     */
    #define MY_HELPERS_INVALID_YEAR                         -56

    
/************* myPWM error codes (100-149) ******************/
    /**
     * @brief Pin not initialized.
     * Initialize has not been called for the pin.
     */
    #define MY_PWM_PIN_NOT_INIT                             -100
    /**
     * @brief Pin already initialized.
     * The pin is already initialized as a PWM pin.
     */
    #define MY_PWM_PIN_ALREADY_INIT                         -101
    /**
     * @brief Invalid slice number.
     * An invalid slice number was passed.
     */
    #define MY_PWM_INVALID_SLICE                            -102
    /**
     * @brief Slice already enabled.
     * The slice has already been enabled.
     */
    #define MY_PWM_SLICE_ALREADY_ENABLED                    -103
    /**
     * @brief Slice already disabled.
     * The slice has already been disabled.
     */
    #define MY_PWM_SLICE_ALREADY_DISABLED                   -104
    /**
     * @brief Phase correct enabled.
     * Phase correct has already been enabled.
     */
    #define MY_PWM_PHASE_CORRECT_ENABLED                    -105
    /**
     * @brief Phase correct disabled.
     * Phase correct has already been disabled.
     */
    #define MY_PWM_PHASE_CORRECT_DISABLED                   -106
    /**
     * @brief Invalid frequency.
     * An invalid frequency was passed.
     */
    #define MY_PWM_INVALID_FREQ                             -107
    /**
     * @brief Invalid channel.
     * An invalid channel was passed.
     */
    #define MY_PWM_INVALID_CHANNEL                          -108
    /**
     * @brief Invalid duty.
     * An invalid duty value was passed (must be between 0-100 inclusive).
     */
    #define MY_PWM_INVALID_DUTY                             -109
    /**
     * @brief Invalid divisor integer.
     * The provided divisor integer is invalid (must be greater than 0).
     */
    #define MY_PWM_INVALID_DIVISOR_INT                      -110
    /**
     * @brief Invalid divisor fraction.
     * The provided divisor fraction is invalid (must be 0-15 inclusive).
     */
    #define MY_PWM_INVALID_DIVISOR_FRAC                     -111
    /**
     * @brief Invalid level.
     * The provided level is greater than wrap value.
     */
    #define MY_PWM_INVALID_LEVEL                            -112
/************** mySPI error codes (150-199) ******************/
    /**
     * @brief Invalid clock pin.
     * The provided clock pin is not a valid clock pin for the spi port.
     */
    #define MY_SPI_INVALID_CLOCK_PIN                        -150
    /**
     * @brief Invalid MISO pin.
     * The provided MISO pin is not a valid MISO pin for the spi port.
     */
    #define MY_SPI_INVALID_MISO_PIN                         -151
    /**
     * @brief Invalid MOSI pin.
     * The provided MOSI pin is not a valid MOSI pin for the spi port.
     */
    #define MY_SPI_INVALID_MOSI_PIN                         -152
    /**
     * @brief Invalid chip select pin
     * The provided chip select pin is not a valid chip select pin for the spi port.
     */
    #define MY_SPI_INVALID_CS_PIN                           -153
    /**
     * @brief Port already initialized.
     * The spi port is already initialized.
     */
    #define MY_SPI_INITIALIZED                              -154
    /**
     * @brief Port already deinitialized.
     * The spi port is already deinialized.
     */
    #define MY_SPI_NOT_INITIALIZED                          -155
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
    /**
     * @brief Channel already init.
     * The channel was already initialized when operation requested.
     */
    #define MY_ERROR_ADC_CHANNEL_ALREADY_INIT               -204

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
    #define MY_ERROR_MY23LC1024_COMM_CHECK_FAILED           -310

/*************** mySNx4HC165 error codes (350-399) *****************/
    /**
     * @brief Enable pin not defined.
     * The enable pin was not defined when operation was requested.
     */
    #define MY_ERROR_MY165_ENABLE_NOT_DEFINED               -350
    /**
     * @brief Clock already enabled.
     * The clock inhibit is already in enabled state when operation was requested.
     */
    #define MY_ERROR_MY165_ALREADY_ENABLED                  -351
    /**
     * @brief Clock already disabled.
     * The clock inhibit is alread in disabled state when opertation was requested.
     */
    #define MY_ERROR_MY165_ADREADY_DISABLED                 -352
    
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
    /**
     * @brief Operation not available.
     * The bit write operation is not available in hardware SPI mode.
     */
    #define MY_ERROR_MY595_OPERATION_NOT_AVAILABLE          -402

/******************** myI2C error codes: (450-499) *******************/
    
    /**
     * @brief Invalid SDA pin.
     * The SDA pin provided is invalid.
     */
    #define MY_ERROR_MYI2C_INVALID_SDA_PIN                  -450
    /**
     * @brief Invalid SCL pin.
     * The SCL pin provided is invalid.
     */
    #define MY_ERROR_MYI2C_INVALID_SCL_PIN                  -451
    /**
     * @brief I2C port alreay initialized.
     * The I2C port was already initialized when initialize was called.
     */
    #define MY_ERROR_MYI2C_INITIALIZED                      -452
    /**
     * @brief I2C port not initialized.
     * The I2C port was already deinitialized when deinitialize was called.
     */
    #define MY_ERROR_MYI2C_NOT_INITIALIZED                  -453
    /**
     * @brief The I2C address is not responding.
     * The device at the I2C address is not responding.
     */
    #define MY_ERROR_MYI2C_ADDRESS_NOT_RESPONDING           -454
/******************** myBMx280 Error codes: (500-549) ***********************/
    /**
     * @brief Invalid I2C address.
     * The I2C address provided is invalid.
     */
    #define MY_ERROR_MYBMx280_INVALID_ADDRESS               -500
    /**
     * @brief No response.
     * The I2C address didn't respond.
     */
    #define MY_ERROR_MYBMx280_NO_RESPONSE                   -501
    /**
     * @brief Invalid response.
     * The chip provided an invalid response.
     */
    #define MY_ERROR_MYBMx280_INVALID_RESPONSE              -502
    /**
     * @brief No humidity.
     * The chip doesn't have humidity.
     */
    #define MY_ERROR_MYBMx280_NO_HUMIDITY                   -503
    /**
     * @brief Invalid standby time.
     * An invalid standby time was provided.
     */
    #define MY_ERROR_MYBMx280_INVALID_STANDBY               -504
    /**
     * @brief Invaid filter.
     * An invalid filter value was provided.
     */
    #define MY_ERROR_MYBMx280_INVALID_FILTER                -505
    /**
     * @brief Invalid over sample rate.
     * An invalid Over Sample Rate was provided.
     */
    #define MY_ERROR_MYBMx280_INVALID_OSR                   -506
    /**
     * @brief Invalid mode.
     * An invalid mode was passed.
     */
    #define MY_ERROR_MYBMx280_INVALID_MODE                  -507

/******************** myds1307 Error codes: (550-599) ***********************/

    /**
     * @brief Invalid seconds.
     * Invalid seconds was passed. Valid values (0-59).
     */
    #define MY_ERROR_MYDS1307_INVALID_SECONDS               -550
    /**
     * @brief Invalid minutes.
     * Invalid minutes value was passed. Valid values (0-59).
     */
    #define MY_ERROR_MYDS1307_INVALID_MINUTES               -551
    /**
     * @brief Invalid hours.
     * Invalid hours value was passed. Valid values (0-23 in 24h mode, 1-12 in 12h mode)
     */
    #define MY_ERROR_MYDS1307_INVALID_HOURS                 -552
    /**
     * @brief Invalid day of the week.
     * Invalid day value was passed. Valid values (1-7).
     */
    #define MY_ERROR_MYDS1307_INVALID_DAY                   -553
    /**
     * @brief Invalid date.
     * Invalid date value was passed. Valid values (1-31).
     */
    #define MY_ERROR_MYDS1307_INVALID_DATE                  -554
    /**
     * @brief Invalid month.
     * Invalid month value was passed. Valid values (1-12).
     */
    #define MY_ERROR_MYDS1307_INVALID_MONTH                 -555
    /**
     * @brief Invalid year.
     * Invalid year value was passed. Valid values (2000-2099)
     */
    #define MY_ERROR_MYDS1307_INVALID_YEAR                  -556
    /**
     * @brief Invalid rate.
     * Invalid rate value was passed. Valid values (RATE_*) (0-3).
     */
    #define MY_ERROR_MYDS1307_INVALID_RATE                  -557
    /**
     * @brief Comms check failed.
     * The comms check during initialization failed.
     */
    #define MY_ERROR_MYDS1307_COMMS_CHECK_FAILED            -558
    /**
     * @brief Boundary crossed.
     * The read / write action would cross a boundary.
     */
    #define MY_ERROR_MYDS1307_BOUNDARY_CROSSED              -559
/******************** myAT24C32 Error codes: (600-649) ***********************/

    /**
     * @brief Invalid address.
     * The address passed is invalid.
     */
    #define MY_ERROR_MYAT24C32_INVALID_ADDRESS              -600
    /**
     * @brief Invalid length.
     * The length provided is too long.
     */
    #define MY_ERROR_MYAT24C32_INVALID_LENGTH               -601
#endif