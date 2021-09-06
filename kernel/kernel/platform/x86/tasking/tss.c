/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <mem/kmalloc.h>
#include <mem/vmm/vmm.h>
#include <platform/x86/gdt.h>
#include <platform/x86/tasking/tss.h>

tss_t tss;

void ltr(uint16_t seg)
{
    asm volatile("ltr %0"
                 :
                 : "r"(seg));
}
