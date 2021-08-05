/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_TASKING_BITS_CPU_H
#define _KERNEL_TASKING_BITS_CPU_H

// includes
#include <drivers/generic/fpu.h>
#include <libkernel/types.h>
#include <mem/vmm/vmm.h>
#include <platform/generic/tasking/context.h>
#include <tasking/bits/sched.h>

#define CPU_CNT 4
#define THIS_CPU (&cpus[system_cpu_id()])
#define FPU_ENABLED

struct thread;
typedef int cpu_state_t;
enum CPU_STATE {
    CPU_IN_KERNEL,
    CPU_IN_USERLAND,
};

typedef struct {
    int id;
    int int_depth_counter;

    char* kstack;
    pdirectory_t* pdir;
    context_t* sched_context; 
    struct thread* running_thread;
    cpu_state_t current_state;
    struct thread* idle_thread;

    sched_data_t sched;

    time_t stat_ticks_since_boot;
    time_t stat_system_and_idle_ticks;
    time_t stat_user_ticks;

#ifdef FPU_ENABLED
    struct thread* fpu_for_thread;
    pid_t fpu_for_pid;
#endif 
} cpu_t;

extern cpu_t cpus[CPU_CNT];

#endif 