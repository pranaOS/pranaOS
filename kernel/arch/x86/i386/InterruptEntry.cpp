/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/arch/x86/DescriptorTable.h>
#include <kernel/arch/x86/TrapFrame.h>

asm(
    ".globl interrupt_common_asm_entry\n"
    "interrupt_common_asm_entry: \n"
    "    pusha\n"
    "    pushl %ds\n"
    "    pushl %es\n"
    "    pushl %fs\n"
    "    pushl %gs\n"
    "    pushl %ss\n"
    "    mov $" __STRINGIFY(GDT_SELECTOR_DATA0) ", %ax\n"
    "    mov %ax, %ds\n"
    "    mov %ax, %es\n"
    "    mov $" __STRINGIFY(GDT_SELECTOR_PROC) ", %ax\n"
    "    mov %ax, %gs\n"
    "    pushl %esp \n" 
    "    subl $" __STRINGIFY(TRAP_FRAME_SIZE - 4) ", %esp \n"
    "    movl %esp, %ebx \n" 
    "    pushl %ebx \n"
    "    cld\n"
    "    call enter_trap \n"
    "    movl %ebx, 0(%esp) \n" 
    "    call handle_interrupt\n"
    "    movl %ebx, 0(%esp) \n" 
    ".globl common_trap_exit \n"
    "common_trap_exit: \n"

    "    call exit_trap \n"
    "    addl $" __STRINGIFY(TRAP_FRAME_SIZE + 4) ", %esp\n" 
    ".globl interrupt_common_asm_exit \n"
    "interrupt_common_asm_exit: \n"
    "    addl $4, %esp\n" 
    "    popl %gs\n"
    "    popl %fs\n"
    "    popl %es\n"
    "    popl %ds\n"
    "    popa\n"
    "    addl $0x4, %esp\n" 
    "    iret\n"
);
