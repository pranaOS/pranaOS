/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

.section .init, "ax", @progbits
    movl %ebp, %esp
    popl %ebp
    retl

.section .fini, "ax", @progbits
    movl %ebp, %esp
    popl %ebp
    retl
