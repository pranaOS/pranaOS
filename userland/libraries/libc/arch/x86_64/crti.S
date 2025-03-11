/**
 * @file crti.S
 * @author Krisna Pranav
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

.section .init, "ax", @progbits
.align 4
.global _init
.type _init, @function
_init:
    pushq %rbp
    movq %rsp, %rbp
    andq $-16, %rsp

.section .fini, "ax", @progbits
.align 4
.global _fini
.type _fini, @function
_fini:
    pushq %rbp
    movq %rsp, %rbp
    andq $-16, %rsp
