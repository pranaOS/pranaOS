CC:=x86_64-pc-pranaos-gcc
CXX:=x86_64-pc-pranaos-g++
LD:=x86_64-pc-pranaos-ld
LDFLAGS:= \
	--sysroot=$(SYSROOT)
AR:=x86_64-pc-pranaos-ar
ARFLAGS:=rcs

AS:=nasm
ASFLAGS:=-f elf64

STRIP:=x86_64-pc-pranaos-strip

KERNEL_CXXFLAGS += \
	-z max-page-size=0x1000 	   \
	-fno-pic                       \
	-mno-sse                       \
	-mno-sse2                      \
	-mno-mmx                       \
	-mno-80387                     \
	-mno-red-zone                  \
	-mcmodel=kernel                \
	-ffreestanding                 \
	-fno-stack-protector           \
	-fno-omit-frame-pointer