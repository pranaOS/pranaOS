/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

[bits 32]
[extern stage2]
global _start
_start:
    push dword [esp+4]
    call stage2
    jmp $