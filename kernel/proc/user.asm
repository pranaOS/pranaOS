[global enter_usermode]
enter_usermode:
	cli

	mov ax,0x23
	mov ds,ax
	mov es,ax 
	mov fs,ax 
	mov gs,ax 

	mov eax, [esp + 4]

	sub eax, 4
	mov ebx, [esp + 12]
	mov dword [eax], ebx

	mov ebx, [esp + 8] 

	push 0x23 
	push eax 
	pushf			

	pop eax
	or eax, 0x200
	push eax

	push 0x1B
	push ebx  
	iret

[global return_usermode]
return_usermode:
	cli

	mov ax,0x23
	mov ds,ax
	mov es,ax 
	mov fs,ax 
	mov gs,ax 

	mov eax, [esp + 4]

	push dword [eax + 18*4] 
	push dword [eax + 17*4] 
	push dword [eax + 16*4]	
	push dword [eax + 15*4] 
	push dword [eax + 14*4] 

	mov edi, [eax + 4*4]
	mov esi, [eax + 5*4]
	mov ebp, [eax + 6*4]
	mov ebx, [eax + 8*4]
	mov edx, [eax + 9*4]
	mov ecx, [eax + 10*4]
	mov eax, [eax + 11*4]

	iret