/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

    .align 4
    .globl _invoke_entry
    .hidden _invoke_entry
    .type _invoke_entry,@function
_invoke_entry: 
    
    andq $~15, %rsp

    jmp *%rcx
