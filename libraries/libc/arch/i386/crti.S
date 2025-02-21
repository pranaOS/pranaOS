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
    pushl %ebp
    movl %esp, %ebp
    andl $-16, %esp

.section .fini, "ax", @progbits
.align 4
.global _fini
.type _fini, @function
_fini:
    pushl %ebp
    movl %esp, %ebp
    andl $-16, %esp
