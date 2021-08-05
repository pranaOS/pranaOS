/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_PLATFORM_AARCH32_SYSTEM_H
#define _KERNEL_PLATFORM_AARCH32_SYSTEM_H

// includes
#include <libkernel/c_attrs.h>
#include <libkernel/types.h>


void system_disable_interrupts();
void system_enable_interrupts();
void system_enable_interrupts_only_counter();


inline static void system_instruction_barrier()
{
    asm volatile("isb");
}

inline static void system_data_synchronise_barrier()
{
    asm volatile("dsb ISH");
}

inline static void system_data_memory_barrier()
{
    asm volatile("dmb ISH");
}

inline static void system_flush_tlb_entry(uint32_t vaddr)
{
    asm volatile("mcr p15, 0, %0, c8, c7, 3"
                 :
                 : "r"(vaddr)
                 : "memory");
    system_data_synchronise_barrier();
}

inline static void system_flush_whole_tlb()
{
    asm volatile("mcr p15, 0, %0, c8, c7, 0"
                 :
                 : "r"(0)
                 : "memory");
    system_data_synchronise_barrier();
}

inline static void system_set_pdir(uint32_t pdir)
{
    system_data_synchronise_barrier();
    asm volatile("mcr p15, 0, %0, c2, c0, 0"
                 :
                 : "r"(pdir)
                 : "memory");
    system_flush_whole_tlb();
}

inline static void system_enable_write_protect()
{
}

inline static void system_disable_write_protect()
{
}

inline static void system_enable_paging()
{
    volatile uint32_t val;
    asm volatile("mrc p15, 0, %0, c1, c0, 0"
                 : "=r"(val));
    asm volatile("orr %0, %1, #0x1"
                 : "=r"(val)
                 : "r"(val));
    asm volatile("mcr p15, 0, %0, c1, c0, 0" ::"r"(val)
                 : "memory");
    system_instruction_barrier();
}

inline static void system_disable_paging()
{
}

inline static void system_stop_until_interrupt()
{
    asm volatile("wfi");
}

NORETURN inline static void system_stop()
{
    system_disable_interrupts();
    system_stop_until_interrupt();
    while (1) { }
}

#endif 