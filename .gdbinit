# Connect to QEMU
target remote localhost:1234

# For bootloader debugging
# change architecture to i386 if debugging STAGE2 32-bit code
#set architecture i8086
set architecture i386
#break *0x7C00

# For kernel debugging
# Add KERNEL file for debugging information
add-symbol-file KERNEL 0xC0001000

set disassemble-next-line on
set disassembly-flavor intel