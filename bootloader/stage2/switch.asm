[bits 16]
switch_to_pm:

	mov 	eax, cr0
	or 		eax, 0x1
	mov 	cr0, eax

	jmp 	CODE_SEG:pmode


[bits 32]
pmode:
    mov     ax, DATA_SEG
    mov     ds, ax
    mov     ss, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax

    mov 	ebp, 0x9000
    mov 	esp, ebp

    call    pbegin

