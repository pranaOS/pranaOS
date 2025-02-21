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
.p2align 2
.global _init
.type _init, @function
_init:
    ret

.section .fini, "ax", @progbits
.p2align 4
.global _fini
.type _fini, @function
_fini:
    ret
