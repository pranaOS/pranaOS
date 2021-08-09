/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

.global _tasking_jumper
.global switch_to_context
.extern system_enable_interrupts

_tasking_jumper:
    bl      system_enable_interrupts_only_counter
    mov     r0, sp 
    cps	    #0x13
    mov     sp, r0
    pop     {pc}