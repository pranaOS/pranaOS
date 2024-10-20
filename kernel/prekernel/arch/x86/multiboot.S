.code32
.set MULTIBOOT_MAGIC,         0x1badb002
.set MULTIBOOT_PAGE_ALIGN,    0x1
.set MULTIBOOT_MEMORY_INFO,   0x2
.set MULTIBOOT_VIDEO_MODE,    0x4
.set multiboot_flags,         MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
.set multiboot_checksum,      -(MULTIBOOT_MAGIC + multiboot_flags)

.section .multiboot, "a"
.align 4

.long MULTIBOOT_MAGIC
.long multiboot_flags
.long multiboot_checksum


.long 0x00000000     
.long 0x00000000     
.long 0x00000000    
.long 0x00000000     
.long 0x00000000     

.long 0x00000000     
.long 1280           
.long 1024          
.long 32          
