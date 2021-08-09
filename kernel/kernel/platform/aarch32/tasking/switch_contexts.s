/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

.global switch_contexts
.global switch_to_context

switch_contexts:
    stmfd   sp!, {r4-r12,lr}
    str     sp, [r0]
    mov     sp, r1
    ldmfd   sp!, {r4-r12,lr}
    bx lr

switch_to_context:
    mov     sp, r0
    ldmfd   sp!, {r4-r12,lr}
    bx lr