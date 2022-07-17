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

class mySpinLock {
    
    public:

/* ################ Constants: ################## */
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

/* ##################### Constructor ################### */
        mySpinLock(const int16_t lockNum) : _lockNum (lockNum) {};

/* ######################## Public Functions: ############### */
        
        /**
         * @brief Get the Free Lock number.
         * Static method to get and claim the next free lock.
         * @param required True = panic if no lock available, False, returns error if no lock available.
         * @return int16_t Positive = Lock number, Negative  = error code.
         */
        static int16_t getFreeLock(const bool required=false);
        /**
         * @brief Validate a lock number.
         * Validate a lock number. Returns true if valid, false if  not.
         * @param lockNum Lock number to verify.
         * @return true Lock number is valid.
         * @return false Lock number is invalid. 
         */
        bool isValidLockNum(const int16_t lockNum);
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
         * @brief Get if the spin lock has been initialized.
         * Returns true if the spin lock has been initialized, otherwise it returns false.
         * @return true Spin lock initialized.
         * @return false Spin lock not initialized or invalid lockNUM.
         */
        bool getInitialized();
        /**
         * @brief Initialize the spin lock.
         * Returns 0 (NO_ERROR) if initialized okay, otherwise if already initialized or an
         * invalid lockNum is passed during construction, an error code is returned.
         * @return int16_t Returns 0 (NO_ERROR) if initialized okay, negative for error code.
         */
        int16_t initialize();

    private:
/* ############### Private Constants: ################## */
        const int16_t _lockNum;
/* ############### Private Variables: #################### */

        static uint32_t _myLocksInit; // What locks have been initialized on a class wide basis.
        static uint32_t _myLocksClaimed; // What locks have been claimed on a class wide basis.
        spin_lock_t *_lock; // My spin lock instance.
        uint32_t _irq;      // Saved IRQ's
        uint32_t _isLocked=false; // If we've locked .
/* ############## Private functions ################### */
        static void __setInit__(const int16_t lockNum, const bool value);
        static bool __getInit__(const int16_t lockNum);
        static void __setClaimed__(const int16_t lockNum, const bool value);
        static bool __getClaimed__(const int16_t lockNum);
};

/* ################# Public Functions: ################## */
int16_t mySpinLock::getFreeLock(const bool required=false) {
    int lockNum;
    lockNum = spin_lock_claim_unused(required);
    if (lockNum < 0) { return ERROR_NO_LOCK_AVAILABLE; }
    __setClaimed__(lockNum, true);
    return (int16_t)lockNum;
}

bool mySpinLock::isValidLockNum(const int16_t lockNum) {
    if (lockNum < 0 or lockNum >= MY_NUM_SPIN_LOCKS) { return false; }
    return true;
}

int16_t mySpinLock::lockBlocking() {
    if (isValidLockNum(_lockNum) == false) { return ERROR_INVALID_LOCK_NUM; }
    if (__getClaimed__(_lockNum) == false) { return ERROR_NOT_CLAIMED; }
    if (_isLocked == true) { return ERROR_IS_LOCKED; }
    _irq = spin_lock_blocking(_lock);
    _isLocked = true;
    return NO_ERROR;
}

int16_t mySpinLock::unlock() {
    if (isValidLockNum(_lockNum) == false) { return ERROR_INVALID_LOCK_NUM; }
    if (__getClaimed__(_lockNum) == false) { return ERROR_NOT_CLAIMED; }
    if (_isLocked == false) { return ERROR_IS_UNLOCKED; }
    spin_unlock(_lock, _irq);
    _isLocked = false;
    return NO_ERROR;
}

bool mySpinLock::getInitialized() {
    if (isValidLockNum(_lockNum) == false) { return false; }
    return __getInit__(_lockNum);
}

int16_t mySpinLock::initialize() {
    if (isValidLockNum(_lockNum) == false) { return ERROR_INVALID_LOCK_NUM; }
    if (__getInit__(_lockNum) == true) {
        _lock = spin_lock_instance(_lockNum);
        return ERROR_IS_INITIALIZED;
    }
    spin_lock_claim(_lockNum);
    _lock = spin_lock_init(_lockNum);
    __setInit__(_lockNum, true);
}




// int16_t isLocked(const int16_t lockNum) {
//     if (isValidLockNum(lockNum) == false) {return ERROR_INVALID_LOCK_NUM; }
//     if (__getClaimed__(lockNum) == false) { return ERROR_NOT_CLAIMED; }
//     if (__getLocked__(lockNum) == true) { return STATUS_LOCKED_LOCALLY; }
//     return is_spin_locked(_locks[lockNum]);
// }

// }

// int16_t unlock(const int16_t lockNum) {
//     if (isValidLockNum(lockNum) == false) { return ERROR_INVALID_LOCK_NUM; }
//     if (__getClaimed__(lockNum) == false) { return ERROR_NOT_CLAIMED; }
//     if (__getLocked__(lockNum) == false) { return ERROR_IS_UNLOCKED; }
//     spin_unlock(_locks[lockNum], _irqs[lockNum]);
//     __setLocked__(lockNum, false);
//     return NO_ERROR;
// }


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