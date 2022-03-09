//
// Created by KrisnaPranav on 09/03/22.
//

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

#endif
