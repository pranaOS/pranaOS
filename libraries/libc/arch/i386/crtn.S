/**
 * @file crtn.S
 * @author Krisna Pranav
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

.section .init, "ax", @progbits
    movl %ebp, %esp
    popl %ebp
    retl

.section .fini, "ax", @progbits
    movl %ebp, %esp
    popl %ebp
    retl
