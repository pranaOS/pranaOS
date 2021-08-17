/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <platform/x86/system.h>
#include <tasking/tasking.h>

void system_disable_interrupts()
{
    THIS_CPU->int_depth_counter++;
    asm volatile("cli");
}

void system_enable_interrupts()
{
    THIS_CPU->int_depth_counter--;
    if (THIS_CPU->int_depth_counter == 0) {
        asm volatile("sti");
    }
}

void system_enable_interrupts_only_counter()
{
    THIS_CPU->int_depth_counter--;
}
