/**
 * @file mySpinLock.hpp
 * @author Peter Nearing (pnearing@protonmail.com)
 * @brief Spin lock helper functions.
 * @version 0.1
 * @date 2022-07-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MY_SPINLOCK_H
#define MY_SPINLOCK_H

#include <hardware/sync.h>
#include "../myStandardDefines.hpp"
#include "../myErrorCodes.hpp"

volatile uint32_t _myLocksInit; // What locks have been initialized on a class wide basis.
volatile uint32_t _myLocksClaimed; // What locks have been claimed on a class wide basis.




class mySpinLock {
    
    public:

/* ################ Constants: ################## */
        /**
         * @brief Generate lock number value
         */
        static const int16_t GENERATE_LOCK_NUM = -1;
    // Status Codes:
        /**
         * @brief Lock is unlocked.
         */
        static const int16_t STATUS_UNLOCKED = 0;
        /**
         * @brief Lock is locked remotely.
         */
        static const int16_t STATUS_LOCKED_REMOTELY = 1;
        /**
         * @brief Lock is locked by this instance.
         */
        static const int16_t STATUS_LOCKED_LOCALLY = 2;

    // Error Codes:
        /**
         * @brief No Error. Value 0.
         * No error occured.
         */
        static const int16_t NO_ERROR = MY_NO_ERROR;
        /**
         * @brief No lock available. Value -125.
         * No Spin lock unclaimed.
         */
        static const int16_t ERROR_NO_LOCK_AVAILABLE = MY_ERROR_SPIN_LOCK_NO_LOCK_AVAILABLE;
        /**
         * @brief Spin lock not claimed. Value -126.
         * Spin lock is not claimed.
         */
        static const int16_t ERROR_NOT_CLAIMED = MY_ERROR_SPIN_LOCK_NOT_CLAIMED;
        /**
         * @brief Spin lock number invalid. Value -127.
         * Spin lock number provided is invalid.
         */
        static const int16_t ERROR_INVALID_LOCK_NUM = MY_ERROR_SPIN_LOCK_INVALID_LOCK_NUM;
        /**
         * @brief Spin lock is unlocked. Value -128.
         * Spin lock is already unlocked.
         */
        static const int16_t ERROR_IS_UNLOCKED = MY_ERROR_SPIN_LOCK_IS_UNLOCKED;
        /**
         * @brief Spin lock is locked. Value -129.
         * Spin lock is already locked.
         */
        static const int16_t ERROR_IS_LOCKED = MY_ERROR_SPIN_LOCK_IS_LOCKED;
        /**
         * @brief Spin lock is initialized. Value -130.
         * Spin lock is already initialized.
         */
        static const int16_t ERROR_IS_INITIALIZED = MY_ERROR_SPIN_LOCK_IS_INITIALIZED;

/* ##################### Constructors: ################### */
        mySpinLock(const int16_t lockNum);

/* ######################## Public Functions: ############### */
        /**
         * @brief Lock the spin lock, blocking
         * Returns 0 (NO_ERROR) if locked okay, otherwise returns an error code.
         * @return int16_t Return 0 (NO_ERROR) if locked okay, negative for error code.
         */
        int16_t lockBlocking();
        /**
         * @brief Unlock the spin lock.
         * Returns 0 (NO_ERROR) if unlocked okay, otherwise returns an error code.
         * @return int16_t Return 0 (NO_ERROR) if unlocked okay, negative for error code.
         */
        int16_t unlock();
        /**
         * @brief Check if the spin lock is locked.
         * Checks if the spin lock is locked. Returns 0 (STATUS_UNLOCKED) if unlocked,
         * 1 (STATUS_LOCKED_REMOTELY) if lock is held some where other than this instance, or
         * 2 (STATUS_LOCKED_LOCALLY) if lock is held by this instance.
         * @return int16_t Returns 0(STATUS_UNLOCKED), 1 (STATUS_LOCKED_REMOTELY), or 2 
         * (STATUS_LOCKED_LOCALLY).
         */
        int16_t isLocked();
        /**
         * @brief Get the Lock number.
         * Returns the lock number.
         * @return int16_t Lock number.
         */
        int16_t getLockNum();

    private:
/* ############### Private Variables: #################### */

        int _lockNum; // The lock number.
        spin_lock_t *_lock; // My spin lock instance.
        uint32_t _irq;      // Saved IRQ's
        uint32_t _isLocked=false; // If we've locked .
/* ############## Private functions ################### */
        static void __setInit__(const int16_t lockNum, const bool value);
        static bool __getInit__(const int16_t lockNum);
        static void __setClaimed__(const int16_t lockNum, const bool value);
        static bool __getClaimed__(const int16_t lockNum);
};


/* ################# Constructors: ##################### */

mySpinLock::mySpinLock(const int16_t lockNum) {

    if (lockNum < 0) {
        _lockNum = spin_lock_claim_unused(true);
        __setClaimed__(_lockNum, true);
        _lock = spin_lock_init(_lockNum);
        __setInit__(_lockNum, true);
    } else {
        _lockNum = lockNum;
        _lock = spin_lock_instance(_lockNum);
        if (__getClaimed__(_lockNum) == false) {
            spin_lock_claim(_lockNum);
            __setClaimed__(_lockNum, true);
        }
        if (__getInit__(_lockNum) == false) {
            spin_lock_init(_lockNum);
            __setInit__(_lockNum, true);
        }
    }
}

/* ################# Public Functions: ################## */

int16_t mySpinLock::lockBlocking() {
    if (_isLocked == true) { return ERROR_IS_LOCKED; }
    _irq = spin_lock_blocking(_lock);
    _isLocked = true;
    return NO_ERROR;
}

int16_t mySpinLock::unlock() {
    if (_isLocked == false) { return ERROR_IS_UNLOCKED; }
    spin_unlock(_lock, _irq);
    _isLocked = false;
    return NO_ERROR;
}

int16_t mySpinLock::isLocked() {
    if (_isLocked == true) { return STATUS_LOCKED_LOCALLY; }
    return (int16_t)is_spin_locked(_lock);
}

int16_t mySpinLock::getLockNum() {
    return _lockNum;
}
/* ################# Private Functions: ##################### */

void mySpinLock::__setInit__(const int16_t lockNum, const bool value) {
    bitWrite(_myLocksInit, lockNum, value);
}

bool mySpinLock::__getInit__(const int16_t lockNum) {
    return bitRead(_myLocksInit, lockNum);
}

void mySpinLock::__setClaimed__(const int16_t lockNum, const bool value) {
    bitWrite(_myLocksClaimed, lockNum, value);
}

bool mySpinLock::__getClaimed__(const int16_t lockNum) {
    return bitRead(_myLocksClaimed, lockNum);
}


#endif