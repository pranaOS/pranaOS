/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef PRANAOS_SEMAPHORE_H
#define PRANAOS_SEMAPHORE_H

#include <libkernel/list.h>
#include <stdint.h>
#include "spinlock.h"

/**
 * @brief semaphore[lock, count, capacity, wait_list]
 * 
 */
struct semaphore {
    spinlock_t lock;
    uint32_t count;
    uint32_t capacity;
    struct list_head wait_list;
};

/**
 * @brief semaphore_initialize
 * 
 */
#define __SEMAPHORE_INITIALIZER(name, n)               \
	{                                                  \
		.lock = 0,                                     \
		.count = n,                                    \
		.capacity = n,                                 \
		.wait_list = LIST_HEAD_INIT((name).wait_list), \
	}


/**
 * @brief define semaphore name
 * 
 */
#define DEFINE_SEMAPHORE(name) \
	struct semaphore name = __SEMAPHORE_INITIALIZER(name, 1)


/***
 * @brief sema init
 *
 * @param sem
 * @param val
 */
static inline void sema_init(struct semaphore *sem, int val) {
    *sem = (struct semaphore)__SEMAPHORE_INITIALIZER(*sem, val);
}

/**
 * @brief acquire semaphore
 * @param sem
 */
void acquire_semaphore(struct semaphore *sem);

/**
 * @brief release semaphore
 * 
 * @param sem
 */
void release_semaphore(struct semaphore *sem);

#endif
