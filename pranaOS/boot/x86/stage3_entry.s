/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

section .pranaos_kernel_boot

[bits 32]
extern stage3
global _start
_start:
    push dword [esp+4]
    call stage3
    jmp $