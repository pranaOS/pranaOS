global _start
extern multiboot2_helper_init
extern multiboot2_helper_com_write_char
extern multiboot2_helper_com_write

extern multiboot2_helper_vga_write
extern multiboot2_helper_vga_write_char

section .text32
%macro COM_WRITE_CHAR 1
    push ebp
    mov ebp, esp
    push %1

    call multiboot2_helper_com_write_char

    add esp, 12
    mov esp, ebp
    pop ebp
%endmacro

%macro COM_WRITE 1
    push ebp
    mov ebp, esp
    push %1

    call multiboot2_helper_com_write

    add esp, 12
    mov esp, ebp
    pop ebp
%endmacro

%macro VGA_WRITE_CHAR 1
    push ebp
    mov ebp, esp
    push %1

    call multiboot2_helper_vga_write_char

    add esp, 12
    mov esp, ebp
    pop ebp
%endmacro

%macro VGA_WRITE 1
    push ebp
    mov ebp, esp
    push %1

    call multiboot2_helper_vga_write

    add esp, 12
    mov esp, ebp
    pop ebp
%endmacro

bits 32
_start:
    cli
    cld

    mov dword[multiboot2_header], ebx
    mov dword[multiboot2_magic], eax


    mov esp, _kernel_stack_top

    call multiboot2_helper_init

    call cpuid_check_long_mode

    call set_up_page_tables
    call enable_paging

    lgdt [_gdt_descriptor]
    jmp _gdt.code:long_mode_start

cpuid_check_long_mode:
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb .no_long_mode

    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz .no_long_mode
    ret
.no_long_mode:
    mov ebx, error_no_long_mode
    jmp loader_print_error

global set_up_page_tables
set_up_page_tables:
    mov eax, p3_table
    or eax, 0b11
    mov [p4_table], eax

    mov eax, p2_table
    or eax, 0b00000011
    mov [p3_table], eax

    mov ecx, 0

.p2_table_map:
    mov eax, 0x200000  
    mul ecx

    or eax, 0b10000011 
    mov [p2_table + ecx * 8], eax

    inc ecx
    cmp ecx, 512
    jne .p2_table_map

    ret

global enable_paging
enable_paging:
    mov eax, p4_table
    mov cr3, eax

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret

print:
	mov ecx, 0xb8000
.loop:
    mov ah, byte[ebx]
    cmp ah, byte 0
    je .exit
    mov byte[ecx], ah
    add ecx, 2
    add ebx, 1

    jmp .loop
.exit:
    ret

loader_print_error:
    mov ecx, error_prompt
    VGA_WRITE ecx
    VGA_WRITE ebx
    VGA_WRITE_CHAR ` `

    mov ecx, error_prompt
    COM_WRITE ecx
    COM_WRITE ebx
    COM_WRITE_CHAR `\n`

    hlt
    ret

loader_print_unknown_err:
    mov ebx, error_unknown
    call loader_print_error

error_prompt: db "boot error: ", 0
error_unknown: db "An unknown error occured", 0
error_no_long_mode: db "No long mode detected. Please use an x86_64 compatible machine.", 0

section .bss
align 4096

p4_table:
    resb 4096

p3_table:
    resb 4096

p2_table:
    resb 4096

section .data
_gdt:
    .zero: equ $ - _gdt
        dq 0 

    .code: equ $ - _gdt
        dw 0x0000       
        dw 0x0000       
        db 0x00         
        db 10011010b    
        db 00100000b    
        db 0x00         

    .data: equ $ - _gdt
        dw 0            
        dw 0            
        db 0            
        db 10010010b    
        db 00000000b    
        db 0            

    .task: equ $ - _gdt
        dq 0
        dq 0

        dw 0

_gdt_descriptor:
    dw $ - _gdt - 1
    dq _gdt

extern _kernel_stack_top
extern _kernel_stack_bottom

multiboot2_header: dq 0
multiboot2_magic: dq 0

section .text
bits 64
long_mode_start:
    cli

    mov ax, _gdt.data
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    extern x86_64_init

    mov edi, dword[multiboot2_header]
    mov esi, dword[multiboot2_magic]

    call x86_64_init

    hlt
