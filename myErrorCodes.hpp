#ifndef MY_ERROR_CODES
#define MY_ERROR_CODES


//************* General Error codes: 1-99 *****************//
#define MY_OKAY                  0
#define MY_NO_ERROR              0
#define MY_GENERAL_ERROR        -1


//************* myADC error codes (100-199) **************//
#define MY_ERROR_ADC_INVALID_CHANNEL                    -100
#define MY_ERROR_ADC_INVALID_PIN                        -101

//************* my23LC1024 error codes (300-399) **********//
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






#endif