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

    "    pushq %r15\n"
    "    pushq %r14\n"
    "    pushq %r13\n"
    "    pushq %r12\n"
    "    pushq %r11\n"
    "    pushq %r10\n"
    "    pushq %r9\n"
    "    pushq %r8\n"
    "    pushq %rax\n"
    "    pushq %rcx\n"
    "    pushq %rdx\n"
    "    pushq %rbx\n"
    "    pushq %rsp\n"
    "    pushq %rbp\n"
    "    pushq %rsi\n"
    "    pushq %rdi\n"
    "    pushq %rsp \n" /* set TrapFrame::regs */
    "    subq $" __STRINGIFY(TRAP_FRAME_SIZE - 8) ", %rsp \n"
    "    movq %rsp, %rdi \n"
    "    cld\n"
    "    call enter_trap \n"
    "    movq %rsp, %rdi \n"
    "    call handle_interrupt \n"
    ".globl common_trap_exit \n"
    "common_trap_exit: \n"
    
    "    movq %rsp, %rdi \n"
    "    call exit_trap \n"
    "    addq $" __STRINGIFY(TRAP_FRAME_SIZE) ", %rsp\n" 
    ".globl interrupt_common_asm_exit \n"
    "interrupt_common_asm_exit: \n"
    "    popq %rdi\n"
    "    popq %rsi\n"
    "    popq %rbp\n"
    "    addq $8, %rsp\n" 
    "    popq %rbx\n"
    "    popq %rdx\n"
    "    popq %rcx\n"
    "    popq %rax\n"
    "    popq %r8\n"
    "    popq %r9\n"
    "    popq %r10\n"
    "    popq %r11\n"
    "    popq %r12\n"
    "    popq %r13\n"
    "    popq %r14\n"
    "    popq %r15\n"
    "    addq $0x8, %rsp\n" 
    "    iretq\n"
);
