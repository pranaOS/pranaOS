%include "defines.asm"
%define SEG_INC_ON_READ ((MAX_SECTORS * BLOCK_SIZE) / 0x10)

[org 0x7c00]

section .text

[bits 16]
_start:
	mov 	[BOOT_DRIVE], dl	

	mov 	bp, 0x9000	
	mov 	sp, bp

	mov 	ah, 0x41
	mov 	bx, 0x55aa
	mov 	dl, [BOOT_DRIVE] 
	int 	0x13
	jc 		.errorNoBios

	call 	load_second_stage

	mov 	bx, init_sec
	call 	print
	call 	print_newline

	push 	word [BOOT_DRIVE]
	jmp 	SECOND_STAGE_ENTRY

	.errorNoBios:
		mov 	bx, biosNoExtended
		call 	print
		jmp 	$

load_second_stage:
	
	mov 	word [high_tloc], (SECOND_STAGE_ENTRY / 0x10)
	mov 	dword [low_lba], 1

	mov  	cx, (SECOND_STAGE_SIZE / SEG_INC_ON_READ + 1)

	.reading_loop:

		cmp 	cx, 0
		jz 		.end

		dec 	cx

		cmp 	cx, 5
		jnz 	.cont
		jmp 	$

	.cont:
		
		mov 	di, 0
		mov 	si, DISK_ADDRESS_PKT
		mov 	ah, 0x42
		mov 	dl, [BOOT_DRIVE]

		mov 	byte [DISK_ADDRESS_PKT], 	0x10
		mov 	word [blkcnt], 				MAX_SECTORS
		mov 	word [low_tloc], 			0
		mov 	word [high_lba], 			0

		int 	0x13
		jc 		.readerror

		mov 	bx, [low_lba]
		add 	bx, MAX_SECTORS
		mov 	[low_lba], bx

		mov 	bx, [high_tloc]
		add 	bx, SEG_INC_ON_READ
		mov 	[high_tloc], bx

		mov 	word dx, [high_tloc]
		call 	print_hex
		call 	print_newline

		jmp 	.reading_loop

	.readerror:
		xor 	dx, dx
		mov 	dh, ah
		call 	print_hex
		mov 	bx, readerror
		call 	print
		jmp 	$

	.end:
		ret


%include "./stage1/print.asm"


DISK_ADDRESS_PKT:
			db	0x10	
			db 	0
blkcnt:		dw  MAX_SECTORS
low_tloc:	dw 	0
high_tloc: 	dw  (SECOND_STAGE_ENTRY / 0x10)
low_lba: 	dd 	1
high_lba: 	dd 	0

biosNoExtended: db "Bios does not support int13 extensions", 0x0
init_sec: 		db "Initializing second stage", 0x0
readerror: 		db " <- readerror", 0x0
ran: 			db " ran ", 0x0
BOOT_DRIVE: 	db 0x80
