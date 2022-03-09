/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "semaphore.h"
#include <memory/vmm.h>
#include <cpu/halt.h>

struct semaphore_waiter {
    struct list_head sibling;
    struct thread *task;
};

/**
 * @brief acquire semaphore
 * 
 * @param sem
 */
void acquire_semaphore(struct semaphore *sem) {
    s_lock(&sem->lock);
    if (sem->count > 0) {
        sem->count--;
        s_unlock(&sem->lock);
        enable_interrupts();
    }

    else {
        struct semaphore_waiter *waiter = kcalloc(1, sizeof(struct semaphore_waiter));
        waiter->task = current_thread;

        s_unlock(&sem->lock);
    }
}