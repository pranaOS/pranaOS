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
    movq %rbp, %rsp
    popq %rbp
    retq

.section .fini, "ax", @progbits
    movq %rbp, %rsp
    popq %rbp
    retq
