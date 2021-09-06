/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <drivers/generic/fpu.h>
#include <platform/generic/cpu.h>
#include <tasking/bits/sched.h>
#include <tasking/proc.h>
#include <tasking/thread.h>

#define RUNNING_THREAD (THIS_CPU->running_thread)

static inline void cpu_enter_kernel_space()
{
    THIS_CPU->current_state = CPU_IN_KERNEL;
}

static inline void cpu_leave_kernel_space()
{
    THIS_CPU->current_state = CPU_IN_USERLAND;
}

static inline void cpu_tick()
{
    if (THIS_CPU->running_thread->process->is_kthread) {
        THIS_CPU->stat_system_and_idle_ticks++;
    } else {
        THIS_CPU->stat_user_ticks++;
    }
}