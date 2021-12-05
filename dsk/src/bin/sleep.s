[bits 64]
_start:
  mov rax, 9                
  mov rdi, __float64__(5.0) 
  mov rsi, 0
  mov rdx, 0
  int 0x80
  mov rax, 1                
  mov rdi, 0                
  int 0x80