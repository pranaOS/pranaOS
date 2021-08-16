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
    addl $4, %esp 
    popl %edi 
    popl %esi 
    popl %edx 
    popl %ecx 

    andl $~15, %esp

    subl $4, %esp

    pushl %edx
    pushl %esi
    pushl %edi
    jmp *%ecx
