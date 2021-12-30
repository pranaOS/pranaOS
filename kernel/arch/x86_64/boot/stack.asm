section .bss
align 16

global _kernel_stack_top
global _kernel_stack_bottom

_kernel_stack_bottom:
    resb 1024 * 16 
_kernel_stack_top: