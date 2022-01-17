GLOBAL EnableSSE
EnableSSE:
    mov eax, cr0
    and ax, 0xFFFB		
    or ax, 0x2			
    mov cr0, eax
    mov eax, cr4
    or ax, 3 << 9		
    mov cr4, eax
    ret
