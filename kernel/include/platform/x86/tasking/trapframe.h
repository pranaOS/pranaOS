/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_PLATFORM_X86_TASKING_TRAPFRAME_H
#define _KERNEL_PLATFORM_X86_TASKING_TRAPFRAME_H

// incldues
#include <libkernel/c_attrs.h>
#include <libkernel/types.h>
#include <platform/x86/gdt.h>
#include <platform/x86/tasking/tss.h>

struct PACKED trapframe {

    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t oesp; 
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    uint16_t gs;
    uint16_t padding1;
    uint16_t fs;
    uint16_t padding2;
    uint16_t es;
    uint16_t padding3;
    uint16_t ds;
    uint16_t padding4;
    uint32_t int_no;

    uint32_t err;
    uint32_t eip;
    uint16_t cs;
    uint16_t padding5;
    uint32_t eflags;

    uint32_t esp;
    uint16_t ss;
    uint16_t padding6;
};

typedef struct trapframe trapframe_t;

static inline uint32_t get_stack_pointer(trapframe_t* tf)
{
    return tf->esp;
}

static inline void set_stack_pointer(trapframe_t* tf, uint32_t sp)
{
    tf->esp = sp;
}

static inline void tf_setup_as_kernel_thread(trapframe_t* tf)
{
    tf->cs      = (SEG_KCODE << 3);
    tf->ds      = (SEG_KDATA << 3);
    tf->es      = tf->ds;
    tf->ss      = tf->ds;
    tf->eflags  = FL_IF;
}

#endif