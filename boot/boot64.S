.code32

.global _start
_start:

  xor %eax, %eax
  mov $0x1000, %edi
  mov $0x5000, %ecx
  rep stosb

  mov $(0x2000 | 3), %eax
  mov %eax, 0x1000

  mov $(0x3000 | 3), %eax
  mov %eax, 0x1FF8

  mov $(0x4000 | 3), %eax
  mov %eax, 0x2000

  mov $(0x4000 | 3), %eax
  mov %eax, 0x3FF0

  mov $0x83, %eax
  mov $0x4000, %ebx
  mov $512, %ecx
ptbl_loop:
  mov %eax, (%ebx)
  add $0x200000, %eax
  add $0x8, %ebx
  dec %ecx
  jnz ptbl_loop

  xor %ebx, %ebx

.global entry32mp
entry32mp:

  mov $0x1000, %eax
  mov %eax, %cr3

  lgdt (gdtr64)

  mov %cr4, %eax
  bts $5, %eax
  mov %eax, %cr4

  mov $0xc0000080, %ecx
  rdmsr
  bts $8, %eax
  wrmsr

  mov %cr0, %eax
  bts $31, %eax
  mov %eax, %cr0

  ljmp $8,$(entry64low)

.align 16
gdtr64:
  .word gdt64_end - gdt64_begin - 1;
  .quad gdt64_begin

.align 16
gdt64_begin:
  .long 0x00000000 
  .long 0x00000000
  .long 0x00000000 
  .long 0x00209800
  .long 0x00000000 
  .long 0x00009000
gdt64_end:

.align 16
.code64
entry64low:
  movq $entry64high, %rax
  jmp *%rax

entry64high:

  xor %rax, %rax
  mov %ax, %ss
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs

  movl 4(%rsp), %edi
  movl 8(%rsp), %esi
  movl 12(%rsp), %edx
  jmp boot64main

.global __deadloop
__deadloop:

  jmp .

.global wrmsr
wrmsr:
  mov %rdi, %rcx     
  mov %rsi, %rax     
  shr $32, %rsi
  mov %rsi, %rdx     
  wrmsr
  retq