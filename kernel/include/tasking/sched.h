/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _KERNEL_TASKING_SCHED_H
#define _KERNEL_TASKING_SCHED_H

#include <libkernel/types.h>
#include <mem/vmm/vmm.h>
#include <tasking/bits/sched.h>
#include <tasking/tasking.h>

void scheduler_init();
void schedule_activate_cpu();
void resched_dont_save_context();
void resched();
void sched();
void sched_enqueue(thread_t* thread);
void sched_dequeue(thread_t* thread);
uint32_t active_cpu_count();

static inline void sched_tick()
{
    if (RUNNING_THREAD) {
        RUNNING_THREAD->ticks_until_preemption--;
        if (!RUNNING_THREAD->ticks_until_preemption) {
            resched();
        }
    }
}

#endif 