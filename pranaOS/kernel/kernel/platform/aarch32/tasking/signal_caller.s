/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

.global signal_caller_start
.global signal_caller_end

signal_caller_start:
    blx     r1 
    mov     r1, r0
    mov     r7, #14 
    swi     0x0
signal_caller_end: