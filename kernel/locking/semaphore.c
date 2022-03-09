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
    disable_interrupts();
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

void release_semaphore(struct semaphore *sem) {
    disable_interrupts();
    s_lock(&sem->lock);
    if (list_empty(&sem->wait_list)) {
        if (sem->count < sem->capacity)
            sem->count++;
    }

    else {
        struct semaphore_waiter *waiter = list_first_entry(&sem->wait_list, struct semaphore_waiter, sibling);

        list_del(&waiter->sibling);
    }

    s_unlock(&sem->lock);
    enable_interrupts();
}