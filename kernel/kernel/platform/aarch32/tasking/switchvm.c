/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <drivers/generic/fpu.h>
#include <mem/vmm/vmm.h>
#include <platform/aarch32/interrupts.h>
#include <platform/generic/system.h>
#include <platform/generic/tasking/trapframe.h>
#include <tasking/tasking.h>

/* switching the page dir and tss to the current proc */
void switchuvm(thread_t* thread)
{
    system_disable_interrupts();
    RUNNING_THREAD = thread;
    vmm_switch_pdir(thread->process->pdir);
    fpu_make_unavail();
    system_enable_interrupts();
}