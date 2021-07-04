/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <string.h>
#include "archs/x86/FPU.h"

char fpu_initial_context[512] ALIGNED(16);
char fpu_registers[512] ALIGNED(16);

void fpu_initialize()
{
    asm volatile("clts");
    size_t t;
    asm volatile("mov %%cr0, %0"
                 : "=r"(t));
    t &= ~(1 << 2);
    t |= (1 << 1);
    t |= (1 << 5);
    asm volatile("mov %0, %%cr0" ::"r"(t));

    asm volatile("mov %%cr4, %0"
                 : "=r"(t));
    t |= 3 << 9;
    asm volatile("mov %0, %%cr4" ::"r"(t));
 
    asm volatile("fninit");
    asm volatile("fxsave (%0)" ::"r"(fpu_initial_context));
}