/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/* FIXME: Only for one cpu now */

#include <libkernel/libkern.h>
#include <platform/aarch32/system.h>
#include <tasking/tasking.h>

void system_disable_interrupts()
{
    THIS_CPU->int_depth_counter++;
    asm volatile("cpsid i");
}

void system_enable_interrupts()
{
    THIS_CPU->int_depth_counter--;
    ASSERT(THIS_CPU->int_depth_counter >= 0);
    if (THIS_CPU->int_depth_counter == 0) {
        asm volatile("cpsie i");
    }
}

void system_enable_interrupts_only_counter()
{
    THIS_CPU->int_depth_counter--;
    ASSERT(THIS_CPU->int_depth_counter >= 0);
}
