#pragma once

#include <ak/atomic.h>

typedef void (*spinlockAcquire)(struct spinLock *spinlock);
typedef void (*spinlockRelease)(struct spinLock *spinlock);

typedef struct spinlockOperations {
    spinlockAcquire acquire;
    spinlockRelease release;
} spinlockoperations_t;

typedef struct spinLock {
    atomic_t lock;
}