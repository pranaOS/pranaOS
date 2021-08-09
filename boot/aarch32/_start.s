/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

.extern arm_mem_desc
.extern vm_setup
.extern vm_setup_secondary_cpu
.extern stage3
.extern boot_secondary_cpu
.global _start
_start:
    ldr     r0, =vm_setup
    ldr     lr, =_start_return
    mov     pc, r0
_start_return:

    ldr     sp, =STACK_TOP
    ldr     r1, =.arm_mem_desc_addr
    ldr     r0, [r1]
    bl      stage3
1:
    b       1b
.size _start, . - _start

.global _start_secondary_cpu
_start_secondary_cpu:
    ldr     r0, =vm_setup_secondary_cpu
    ldr     lr, =_start_secondary_cpu_return
    mov     pc, r0
_start_secondary_cpu_return:

    ldr     sp, =STACK_SECONDARY_TOP
    mov     r9, #512
    mrc     p15, #0, r8, c0, c0, #5 
2:
    add     r8, r8, #1
    cmp     r8, #0
    beq     _start_secondary_cpu_exit
    add     r9, r9, #512
    b       2b
_start_secondary_cpu_exit:
    subs    sp, r9
    bl      boot_secondary_cpu
3:
    b       3b


.arm_mem_desc_addr:
    .word arm_mem_desc