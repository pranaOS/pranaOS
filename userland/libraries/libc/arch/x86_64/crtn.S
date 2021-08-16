/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

.section .init, "ax", @progbits
    movq %rbp, %rsp
    popq %rbp
    retq

.section .fini, "ax", @progbits
    movq %rbp, %rsp
    popq %rbp
    retq
