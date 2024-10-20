#include <mods/platform.h>
#include <kernel/prekernel/prekernel.h>

.code32

.section .stack, "aw", @nobits
stack_bottom:
.skip 32768
stack_top:

.global kernel_cmdline
kernel_cmdline:
.skip 4096

.section .page_tables, "aw", @nobits
.align 4096
#if ARCH(X86_64)
.global boot_pml4t
boot_pml4t:
.skip 4096
#endif
.global boot_pdpt
boot_pdpt:
.skip 4096
.global boot_pd0
boot_pd0:
.skip 4096
.global boot_pd0_pts
boot_pd0_pts:
.skip 4096 * (MAX_KERNEL_SIZE >> 21)
.global boot_pd_kernel
boot_pd_kernel:
.skip 4096
.global boot_pd_kernel_pt0
boot_pd_kernel_pt0:
.skip 4096
.global boot_pd_kernel_image_pts
boot_pd_kernel_image_pts:
.skip 4096 * (MAX_KERNEL_SIZE >> 21)
.global boot_pd_kernel_pt1023
boot_pd_kernel_pt1023:
.skip 4096

.section .text

.global start
.type start, @function

.extern init
.type init, @function

.global reload_cr3
.type reload_cr3, @function

.extern multiboot_info_ptr
.type multiboot_info_ptr, @object

#if ARCH(X86_64)
gdt64:
    .quad 0
gdt64code:
    .quad (1<<43) | (1<<44) | (1<<47) | (1<<53) 
.global gdt64ptr
gdt64ptr:
    .short . - gdt64 - 1
    .quad gdt64

.set code64_sel_value, gdt64code - gdt64

.global code64_sel
code64_sel:
.short code64_sel_value
#endif

start:
    jmp real_start

print_no_halt:
    pushl %ebp
    movl %esp, %ebp

    pushl %esi
    pushl %ecx

    movl 8(%ebp), %esi

    mov $0xb8000, %ecx    
    mov $0x07, %ah        

.Lprint_str_loop:
    lodsb                 

    test %al, %al
    jz .Lprint_str_end

    movw %ax, (%ecx)
    add $2, %ecx

    jmp .Lprint_str_loop
.Lprint_str_end:

    mov %esi, %eax
    sub 8(%ebp), %eax

    popl %ecx
    popl %esi

    movl %ebp, %esp
    popl %ebp
    ret



print_and_halt:


.equ COPIED_STRING_LOCATION, 0x400
.equ GDT_REAL_MODE_LOCATION, 0x45000
.equ EXITING_PROTECTED_MODE_CODE_LOCATION, 0x10000
.equ REAL_MODE_CODE, 0x500
.equ PROTECTED_MODE_16_BIT_CODE, 0x600
    movl %esp, %ebp
    movl 4(%ebp), %esi

    pushl %esi
    call print_no_halt
    addl $4, %esp

    mov %eax, %ecx
    movw %cx, (COPIED_STRING_LOCATION)    

    mov 4(%ebp), %esi
    mov $COPIED_STRING_LOCATION + 2, %edi
    rep movsb

    movl $gdt_table_real_mode, %eax
    movl $gdt_table_real_mode_end, %ebx

    movl %ebx, %ecx
    sub %eax, %ecx
    mov %eax, %esi          
    mov $GDT_REAL_MODE_LOCATION, %edi     
    rep movsb

    movl $protected_mode_16_bit, %eax
    movl $real_mode, %ebx

    movl %ebx, %ecx
    sub %eax, %ecx
    mov %eax, %esi          
    mov $PROTECTED_MODE_16_BIT_CODE, %edi     
    rep movsb

    movl $real_mode, %eax
    movl $end_of_print_and_halt_function, %ebx

    movl %ebx, %ecx
    sub %eax, %ecx
    mov %eax, %esi          
    mov $REAL_MODE_CODE, %edi     
    rep movsb


    movl $exiting_real_mode, %eax
    movl $protected_mode_16_bit, %ebx

    movl %ebx, %ecx
    sub %eax, %ecx
    mov %eax, %esi          
    mov $EXITING_PROTECTED_MODE_CODE_LOCATION, %edi     
    pushl %edi
    rep movsb
    popl %edi
    pushl %edi
    ret

gdt_table_real_mode:
    .quad 0             

	.short 0xffff
	.short 0
    .byte 0
    .byte 0b10011010
    .byte 0b00001111
    .byte 0x0

    .short 0xffff
	.short 0
    .byte 0
    .byte 0b10010010
    .byte 0b00001111
    .byte 0x0
gdt_table_real_mode_end:

no_long_mode_string:
    .asciz "Your computer does not support long mode (64-bit mode). Halting!"

no_pae_string:
    .asciz "Your computer does not support PAE. Halting!"

pentium_m_forcepae_string:
    .asciz "Intel Pentium M detected. Assuming present but unadvertised PAE support."

kernel_image_too_big_string:
    .asciz "Error: Kernel Image too big for memory slot. Halting!"

.code32
exiting_real_mode:

    mov $0x50000, %eax
    pushl %eax
    movl $0x3ff, 0(%eax)
    add $2, %eax
    movl $0, 0(%eax)
    popl %eax
    lidt (%eax)

    mov $0x40000, %eax
    pushl %eax
    movl $32, 0(%eax)
    add $2, %eax
    movl $GDT_REAL_MODE_LOCATION, 0(%eax)
    popl %eax
    lgdt (%eax)

    pushw $8
    push $PROTECTED_MODE_16_BIT_CODE
    lret
    hlt

.code16
protected_mode_16_bit:
    xor %eax, %eax
    movl $0x10, %eax
    movw %ax, %ds
    and $0xFE, %al       
    mov %eax, %cr0
    mov $0x10, %eax
    movl %eax, %cr0

    pushw $0
    push $REAL_MODE_CODE
    lret
    hlt

real_mode:
    movw $0x7000, %ax
    movl $0x0000, %esp
    movw %ax, %ss

    xor %ax, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    mov $0x3, %ax
    int $0x10

    movb $0x13, %ah
    movb $0x0, %bh
    movb $0xf, %bl
    movw (COPIED_STRING_LOCATION), %cx
    movw $0, %dx
    movw $COPIED_STRING_LOCATION + 2, %bp
    int $0x10

    movl $0xdeadcafe, %ebx
    cli
    hlt
end_of_print_and_halt_function:

.code32
real_start:
    cli
    cld
    mov $end_of_prekernel_image, %esi
    cmp $MAX_KERNEL_SIZE, %esi
    jbe kernel_not_too_large
    
    movl $kernel_image_too_big_string, %esi
    pushl %esi
    call print_and_halt

    hlt

kernel_not_too_large:
    pushl %eax
    pushl %edx
    pushl %ebx

    movl $0x1, %eax       
    cpuid
    testl $(1 << 6), %edx 
    jnz pae_supported     

    call is_pentium_m_with_hidden_pae
    test %eax, %eax
    jz .Lskip_forcepae

    pushl $pentium_m_forcepae_string
    call print_no_halt
    subl $4, %esp
    jmp pae_supported
.Lskip_forcepae:

    movl $no_pae_string, %esi
    pushl %esi
    call print_and_halt
    hlt


#if ARCH(X86_64)
pae_supported:
    movl $0x80000001, %eax
    cpuid
    testl $(1 << 29), %edx    
    jnz long_mode_supported             

    movl $no_long_mode_string, %esi
    pushl %esi
    call print_and_halt

    hlt



long_mode_supported:
    popl %ebx
    popl %edx
    popl %eax
#else

pae_supported:
    popl %ebx
    popl %edx
    popl %eax
#endif

    movl %ebx, %esi
    addl $16, %esi
    movl (%esi), %esi
    movl $1024, %ecx
    movl $kernel_cmdline, %edi
    rep movsl

#if ARCH(X86_64)
    movl $boot_pml4t, %edi
    movl $1024, %ecx
    xorl %eax, %eax
    rep stosl

    movl $boot_pml4t, %edi
    movl $boot_pdpt, 0(%edi)

    orl $0x3, 0(%edi)
#endif

    movl $boot_pdpt, %edi
    movl $1024, %ecx
    xorl %eax, %eax
    rep stosl

    movl $boot_pdpt, %edi
#if ARCH(X86_64)
    movl $(boot_pd0 + 3), 0(%edi)
#else
    movl $(boot_pd0 + 1), 0(%edi)
#endif

    movl $boot_pd0, %edi
    movl $1024, %ecx
    xorl %eax, %eax
    rep stosl

    movl $boot_pd0_pts, %edi
    movl $(1024 * (MAX_KERNEL_SIZE >> 21)), %ecx
    xorl %eax, %eax
    rep stosl

    movl $(MAX_KERNEL_SIZE >> 21), %ecx
    movl $boot_pd0, %edi
    movl $boot_pd0_pts, %eax

1:
    movl %eax, 0(%edi)

    orl $0x3, 0(%edi)
    addl $8, %edi
    addl $4096, %eax
    loop 1b

    movl $(512 * (MAX_KERNEL_SIZE >> 21)), %ecx
    movl $boot_pd0_pts, %edi
    xorl %eax, %eax

1:
    movl %eax, 0(%edi)

    orl $0x3, 0(%edi)
    addl $8, %edi
    addl $4096, %eax
    loop 1b

#if ARCH(X86_64)
    movl $boot_pml4t, %eax
#else
    movl $boot_pdpt, %eax
#endif

    movl %eax, %cr3

    movl %cr4, %eax
    orl $0x60, %eax
    movl %eax, %cr4

#if ARCH(X86_64)
1:

    mov $0xC0000080, %ecx            
    rdmsr                            
    or $(1 << 8), %eax               
    wrmsr                            
#endif

    movl %cr0, %eax
    orl $0x80000000, %eax
    movl %eax, %cr0

    mov $stack_top, %esp
    and $-16, %esp

#if ARCH(X86_64)
    mov $gdt64ptr, %eax
    lgdt (%eax)
    ljmpl $code64_sel_value, $1f

.code64
1:
    movl %ebx, %ebx
    movq %rbx, multiboot_info_ptr

    mov $0, %ax
    mov %ax, %ss
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
#else
    movl %ebx, multiboot_info_ptr
#endif

    call reload_cr3
    call init

    cli
loop:
    hlt
    jmp loop

reload_cr3:
#if ARCH(X86_64)
    pushq %rax
    mov %cr3, %rax
    mov %rax, %cr3
    popq %rax
#else
    pushl %eax
    movl %cr3, %eax
    movl %eax, %cr3
    popl %eax
#endif
    ret


.code32

is_intel_cpu:
    pushl %ebp
    movl %esp, %ebp

    pushl %ebx

    xorl %eax, %eax
    cpuid

    xorl %eax, %eax
    cmpl $0x756e6547, %ebx  
    jnz .Lis_intel_cpu_end
    cmpl $0x49656e69, %edx  
    jnz .Lis_intel_cpu_end
    cmpl $0x6c65746e, %ecx  
    jnz .Lis_intel_cpu_end

    movl $1, %eax

.Lis_intel_cpu_end:
    popl %ebx

    movl %ebp, %esp
    popl %ebp

    ret

get_cpu_model_family:
    pushl %ebp
    movl %esp, %ebp

    pushl %ebx

    movl $0x1, %eax
    cpuid
    movl %eax, %ebx
    movl %eax, %ecx
    movl %eax, %edx

    shrl $8, %eax
    andl $0xf, %eax

    shrl $4, %edx
    andl $0xf, %edx

    cmpl $6, %eax
    jz .Ldo_ext_model
    cmpl $15, %eax
    jz .Ldo_ext_model
    jmp .Lskip_ext_model
.Ldo_ext_model:
    shrl $16, %ebx
    andl $0xf, %ebx
    shll $4, %ebx
    addl %ebx, %edx
.Lskip_ext_model:

    cmpl $15, %eax
    jnz .Lskip_ext_family
    shrl $20, %ecx
    andl $0xff, %ecx
    addl %ecx, %eax
.Lskip_ext_family:

    popl %ebx

    movl %ebp, %esp
    popl %ebp

    ret

is_pentium_m_with_hidden_pae:
    pushl %ebp
    movl %esp, %ebp

    call is_intel_cpu
    testl %eax, %eax
    jz .Lis_pentium_m_end

    call get_cpu_model_family
    movl %eax, %ecx        
    xorl %eax, %eax

    cmpl $6, %ecx          
    jnz .Lis_pentium_m_end

    cmpl $9, %edx          
    jz .Lpass_model_check
    cmpl $13, %edx         
    jz .Lpass_model_check
    jmp .Lis_pentium_m_end
.Lpass_model_check:

    movl $1, %eax

.Lis_pentium_m_end:
    movl %ebp, %esp
    popl %ebp

    ret
