/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/* FIXME: Only for one cpu now */

#include <platform/x86/registers.h>

uint32_t read_cr2()
{
    uint32_t val;
    asm volatile("movl %%cr2,%0"
                 : "=r"(val));
    return val;
}

uint32_t read_cr3()
{
    uint32_t val;
    asm volatile("movl %%cr3,%0"
                 : "=r"(val));
    return val;
}

uint32_t read_esp()
{
    uint32_t val;
    asm volatile("movl %%esp,%0"
                 : "=r"(val));
    return val;
}
