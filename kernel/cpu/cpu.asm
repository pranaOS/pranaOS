/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

[global gdt_flush]    
gdt_flush:
    mov eax, [esp+4]  
    lgdt [eax]        

    mov ax, 0x10      
    mov ds, ax        
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush   
.flush:
    ret


[global idt_flush]    
idt_flush:
    mov eax, [esp+4]  
    lidt [eax]        
    ret


[global tss_flush]   
tss_flush:
	mov ax, 0x2B      									
	ltr ax            
	ret