#define ASM_FILE        1
#include "multiboot.h"

#define STACK_SIZE                      0x4000
#define MULTIBOOT_HEADER_FLAGS  MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_VIDEO_MODE | MULTIBOOT_INFO_CMDLINE

.align  4
.global multiboot_header
multiboot_header:
  .long MULTIBOOT_HEADER_MAGIC
  .long MULTIBOOT_HEADER_FLAGS
  .long -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
  .long   0
  .long   0
  .long   0
  .long   0
  .long   0 
  .long 0
  .long 640
  .long 480
  .long 32

.global _start
_start:
  jmp     multiboot_entry

multiboot_entry:

  movl    $(stack + STACK_SIZE), %esp

  pushl   $0
  popf

  pushl   %ebx

  pushl   %eax

  call    multibootmain

.comm   stack, STACK_SIZE