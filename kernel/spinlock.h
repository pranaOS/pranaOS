/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <ak/atomic.h>

/**
 * @brief spinlock acquire, spinlock release
 * 
 */
typedef void (*spinlockAcquire)(struct spinLock *spinlock);
typedef void (*spinlockRelease)(struct spinLock *spinlock);

/**
 * @brief spinlock operations[acquire, release]
 * 
 */
typedef struct spinlockOperations {
    spinlockAcquire acquire;
    spinlockRelease release;
} spinlockoperations_t;

/**
 * @brief spinlock main function
 * 
 */
typedef struct spinLock {
    atomic_t lock;
    spinlockoperations_t operations;
} spinlock_t;

/**
 * @brief spinlock default lock function
 * 
 * @param spinlock 
 */
void spinlock_default(spinlock_t *spinlock);

/**
 * @brief spinlock release function
 * 
 * @param spinlock 
 */
void spinlock_default_release(spinlock_t *spinlock);