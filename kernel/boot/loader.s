.set ALIGN,    1<<0             
.set MEMINFO,  1<<1             
.set FLAGS,    ALIGN | MEMINFO  
.set MAGIC,    0x1BADB002       
.set CHECKSUM, -(MAGIC + FLAGS) 

.section .multiboot
	.align 4
	.long MAGIC
	.long FLAGS
	.long CHECKSUM

.set KERNEL_VIRTUAL_BASE, 0xC0000000
.set KERNEL_PAGE_NUMBER, (KERNEL_VIRTUAL_BASE >> 22)

.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 
.global stack_top
stack_top:

.section .data
.align 0x1000
.global bootpagedirectory
bootpagedirectory:
	.long 0x00000083
	
	.rept (KERNEL_PAGE_NUMBER - 1)
	.long 0
	.endr
	
	.long 0x00000083

	.rept (1024 - KERNEL_PAGE_NUMBER - 1)
	.long 0
	.endr

.global _kernel_virtual_base
_kernel_virtual_base:
	.long KERNEL_VIRTUAL_BASE

.section .text
.align 4
.global _entrypoint
.type _entrypoint, @function

_entrypoint:
	mov $(bootpagedirectory - KERNEL_VIRTUAL_BASE), %ecx
	mov %ecx, %cr3

	mov %cr4, %ecx
	or $0x00000010, %ecx
	mov %ecx, %cr4

	mov %cr0, %ecx
	or $0x80000001, %ecx
	mov %ecx, %cr0

	lea 4f, %ecx
	jmp *%ecx

4:
	movl $0, bootpagedirectory
	invlpg 0

	movl $stack_top, %esp
	movl $0, %ebp

	add $KERNEL_VIRTUAL_BASE, %ebx

    call callConstructors

	push $_kernel_base
	push $_kernel_end
    push %eax
    push %ebx
    call kernelMain

_stop:
    cli
    hlt
    jmp _stop
