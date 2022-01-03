%include "../defines.asm"


[bits 16]
section 	.text

[global _start]
_start:
	pop 	dx
	and 	dx, 0xFF
	mov 	word [BOOT_DRIVE], dx

	push 	ds

	cli
 
	lgdt 	[gdt_descriptor]

	mov 	eax, cr0
	or 		eax, 1		
	mov 	cr0, eax

	jmp 	$ + 2

	mov 	bx, DATA_SEG
	mov 	ds, bx

	and 	al, 0xFE
	mov 	cr0, eax

	jmp 	$+2

	pop 	ds
	
	mov 	bx, hello_test
	call 	print
	call 	print_newline

	call 	load_kernel


[global _jump_kernel]
[extern _findkernel]

load_kernel:

	call 	check_a20

	mov 	bx, A20_enabled
	call 	print


 	push 	dword BLOCK_SIZE
 	push 	dword [BOOT_DRIVE]
 	
 	push 	word 0
 	call 	_findkernel

_jump_kernel:
	mov 	bx, loadedKernel
	call 	print

	mov 	ax, 0x4f02
	mov 	bx, 0x0118
	int 	0x10
	
	call 	switch_to_pm

[bits 32]
pbegin:
	jmp 	KERNEL_ENTRY_POINT
	jmp 	$

[bits 16]
check_a20:

	mov 	ax, 0x2403
	int 	0x15
	jb 		.bios_unsupported
	cmp 	ah, 0
	jnz 	.bios_unsupported

	mov 	ax, 0x2402
	int 	0x15
	jb 		.bios_unsupported 	
	cmp 	ah, 0
	jnz 	.bios_unsupported	

	cmp 	al, 1
	jnz 	.bios_enable_a20	

	ret

.bios_enable_a20:
	mov 	ax, 0x2401
	int 	0x15
	jb 		.bios_unsupported 	
	cmp 	ah, 0
	jnz 	.bios_unsupported 	

	ret

.bios_unsupported:

	ret

%include "gdt.asm"
%include "print.asm"
%include "load_block.asm"
%include "switch.asm"

section 	.rodata

hello_test: 	db "Unreal mode entered", 0x0
loadedKernel:	db "Kernel loaded, jumping...", 0x0
A20_enabled:	db "A20 line enabled", 0x0
testline: 		db "Test", 0x0
BOOT_DRIVE 		dd 0x0