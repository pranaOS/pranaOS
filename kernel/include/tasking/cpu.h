/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#ifndef _KERNEL_TASKING_CPU_H
#define _KERNEL_TASKING_CPU_H


// includes
#include <drivers/generic/fpu.h>
#include <platform/generic/cpu.h>
#include <tasking/bits/sched.h>
#include <tasking/proc.h>
#include <tasking/thread.h>

#define RUNNING_THREAD (THIS_CPU->running_thread)

static inline void cpu_center_kernel_space()
{
    THIS_CPU->current_state = CPU_IN_KERNEL;
}

static inline void cpu_leave_kernel_space()
{
    THIS_CPU->current = CPU_IN_KERNEL;
}

static inline void cpu_tick()
{
    THIS_CPU->running_thread = STAT_USER_TICKS++;
}

#endif 