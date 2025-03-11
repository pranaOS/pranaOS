/**
 * @file setjmp.S
 * @author Krisna Pranav
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <bits/sighow.h>

.Lget_pc:
    mov (%esp), %ebx
    ret

.global _setjmp
.global setjmp
_setjmp:
setjmp:
    xor %eax, %eax          
    jmp .Lsigset_common

.global sigsetjmp
sigsetjmp:
    mov 8(%esp), %eax       

.Lsigset_common:
    mov 4(%esp), %ecx       
    mov %eax, 24(%ecx)      
    movl $0, 28(%ecx)       
    test %eax, %eax
    jz .Lsaveregs

    push %ebp               
    mov %esp, %ebp
    push %ebx
    call .Lget_pc           
    addl $_GLOBAL_OFFSET_TABLE_, %ebx

    lea 28(%ecx), %eax      
    push %eax
    push $0                 
    push $0                 
    call sigprocmask@plt
    add $12, %esp
    pop %ebx
    pop %ebp

.Lsaveregs:
    mov 4(%esp), %ecx       
    mov (%esp), %edx        
    mov %ebx, (0 * 4)(%ecx) 
    mov %esi, (1 * 4)(%ecx)
    mov %edi, (2 * 4)(%ecx)
    mov %ebp, (3 * 4)(%ecx)
    mov %esp, (4 * 4)(%ecx)
    mov %edx, (5 * 4)(%ecx)
    xor %eax, %eax
    ret

.global _longjmp
.global longjmp
_longjmp:
longjmp:
    mov 4(%esp), %ecx       
    mov 8(%esp), %eax       
    test %eax, %eax
    jnz  .Lnonzero
    mov $1, %eax

.Lnonzero:
    mov (0 * 4)(%ecx), %ebx 
    mov (1 * 4)(%ecx), %esi
    mov (2 * 4)(%ecx), %edi
    mov (3 * 4)(%ecx), %ebp
    
    mov (4 * 4)(%ecx), %esp
    mov (5 * 4)(%ecx), %edx
    mov %edx, (%esp)        

    ret
