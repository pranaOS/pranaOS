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

.global setjmp
setjmp:
    mov $0, %esi            

.global sigsetjmp
sigsetjmp:
    mov %esi, 64(%rdi)      
    movl $0, 68(%rdi)       
    test %esi, %esi
    jz .Lsaveregs

    push %rbp               
    mov %rsp, %rbp
    push %rdi
    lea 68(%rdi), %rdx      
    mov $0, %rsi            
    mov $0, %rdi            
    call sigprocmask@plt
    pop %rdi
    pop %rbp

.Lsaveregs:
    mov %rbx, (0 * 8)(%rdi) 
    mov %r12, (1 * 8)(%rdi)
    mov %r13, (2 * 8)(%rdi)
    mov %r14, (3 * 8)(%rdi)
    mov %r15, (4 * 8)(%rdi)
    mov %rbp, (5 * 8)(%rdi)
    mov %rsp, (6 * 8)(%rdi)
    mov (%rsp), %rax        
    mov %rax, (7 * 8)(%rdi)    
    xor %eax, %eax
    ret

.global longjmp
longjmp:
    mov %esi, %eax
    test %eax, %eax
    jnz  .Lnonzero
    mov $1, %eax

.Lnonzero:
    mov (0 * 8)(%rdi), %rbx 
    mov (1 * 8)(%rdi), %r12
    mov (2 * 8)(%rdi), %r13
    mov (3 * 8)(%rdi), %r14
    mov (4 * 8)(%rdi), %r15
    mov (5 * 8)(%rdi), %rbp
    
    mov (6 * 8)(%rdi), %rsp
    mov (7 * 8)(%rdi), %rcx
    mov %rcx, (%rsp)        
    
    ret
