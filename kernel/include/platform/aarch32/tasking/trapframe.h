/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_PLATFORM_AARCH32_TASKING_TRAPFRAME_H
#define _KERNEL_PLATFORM_AARCH32_TASKING_TRAPFRAME_H

// includes
#include <libkernel/c_attrs.h>
#include <libkernel/log.h>
#include <libkernel/types.h>

#define CPSR_M_USR 0x10U
#define CPSR_M_FIQ 0x11U
#define CPSR_M_IRQ 0x12U
#define CPSR_M_SVC 0x13U
#define CPSR_M_MON 0x16U
#define CPSR_M_ABT 0x17U
#define CPSR_M_HYP 0x1AU
#define CPSR_M_UND 0x1BU
#define CPSR_M_SYS 0x1FU


typedef struct 
{
    uint32_t user_flags;
    uint32_t user_sp;
    uint32_t user_lr;
    uint32_t r[13];
    uint32_t user_ip;
} PACKED trapframe_t;

static inline uint32_t get_stack_pointer(trapframe_t* tf)
{
    return tf->user_sp;
}

static inline void set_stack_pointer(trapframe_t* tf, uint32_t sp)
{
    tf->user_sp = sp;
}

static inline uint32_t get_base_pointer(trapframe_t* tf)
{
    return 0;
}

static inline void set_base_pointer(trapframe_t* tf, uint32_t bp)
{
}

static inline uint32_t get_instructions_pointer(trapframe_t* tf)
{
    return tf->user_ip;
}


#endif 