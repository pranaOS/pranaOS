[bits 16]
[global _load_block]
_load_block:
	push 	ebp
	movzx 	ebp, sp

	mov 	dl, [bp + 8]

	mov 	eax, [bp + 16]
	mov 	word [blkcnt], ax

	mov 	eax, [bp + 20]
	mov 	word [low_tloc], ax
	
	mov 	eax, [bp + 24]
	mov 	word [high_tloc], ax
	
	mov 	eax, [bp + 12]
	mov 	dword [low_lba], eax
	mov 	dword [high_lba], 0

	mov 	si, DISK_ADDRESS_PKT
	mov 	ah, 0x42

	int 	0x13
	jc 		.error

	pop 	ebp
	ret

.error:
	mov 	bx, errorLoad
	call 	print
	pop 	ebp
	ret

errorLoad: db "Error loading", 0

DISK_ADDRESS_PKT:
			db 0x10					
			db 0					
blkcnt: 	dw 0					
low_tloc: 	dw 0			 		
high_tloc:	dw 0
low_lba: 	dd 0
high_lba:	dd 0
