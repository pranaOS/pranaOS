/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_PLATFORM_AARCH32_REGISTERS_H
#define _KERNEL_PLATFORM_AARCH32_REGISTERS_H

// includes
#include <libkernel/types.h>
#include <platform/aarch32/system.h>

static inline uint32_t read_r3()
{
    uint32_t val;
    asm volatile("mov %0, r3"
                 : "=r"(val)
                 :);
    return val;
}

static inline uint32_t read_far()
{
    uint32_t val;
    asm volatile("mrc p15, 0, %0, c6, c0, 0"
                 : "=r"(val)
                 :);
    return val;
}

static inline uint32_t read_cbar()
{
    uint32_t val;
    asm volatile("mrc p15, 4, %0, c15, c0, 0"
                 : "=r"(val)
                 :);
    return val;
}

static inline uint32_t read_dfsr()
{
    uint32_t val;
    asm volatile("mrc p15, 0, %0, c5, c0, 0"
                 : "=r"(val)
                 :);
    return val;
}

static inline uint32_t read_cpsr()
{
    uint32_t cpsr;
    asm volatile("mrs %0, cpsr"
                 : "=r"(cpsr)
                 :);
    return cpsr;
}

static inline uint32_t read_spsr()
{
    uint32_t spsr;
    asm volatile("mrs %0, spsr"
                 : "=r"(spsr)
                 :);
    return spsr;
}

static inline uint32_t read_cpacr()
{
    uint32_t cpacr;
    asm volatile("mrc p15, 0, %0, c1, c0, 2"
                 : "=r"(cpacr)
                 :);
    return cpacr;
}

static inline void write_cpacr(uint32_t val)
{
    asm volatile("mcr p15, 0, %0, c1, c0, 2"
                 :
                 : "r"(val)
                 : "memory");
    system_instruction_barrier();
}

static inline uint32_t read_nsacr()
{
    uint32_t cpacr;
    asm volatile("mrc p15, 0, %0, c1, c1, 2"
                 : "=r"(cpacr)
                 :);
    return cpacr;
}

static inline void write_nsacr(uint32_t val)
{
    asm volatile("mcr p15, 0, %0, c1, c1, 2"
                 :
                 : "r"(val)
                 : "memory");
    system_instruction_barrier();
}

static inline uint32_t read_hcptr()
{
    uint32_t cpacr;
    asm volatile("mrc p15, 4, %0, c1, c1, 2"
                 : "=r"(cpacr)
                 :);
    return cpacr;
}

static inline void write_hcptr(uint32_t val)
{
    asm volatile("mcr p15, 4, %0, c1, c1, 2"
                 :
                 : "r"(val)
                 : "memory");
    system_instruction_barrier();
}

#endif 