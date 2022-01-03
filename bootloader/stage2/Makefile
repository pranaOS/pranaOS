BUILD_C = $(shell find $(PREFIX)/stage2 -type f -name "*.c")
BUILD_H = $(shell find $(PREFIX)/stage2 -type f -name "*.h")

OBJECTS = ${BUILD_C:.c=.o}
SMLRC = smlrc


SMLRC_FLAGS = -unreal -SI $(PREFIX)/stage2 -Wall -D KERNEL_ENTRY=$(KERNEL_ENTRY) -D KERNEL_INODE=$(KERNEL_INODE)
NASM_FLAGS = -f elf -g -F dwarf
LD_FLAGS = -m elf_i386 -T linker.ld -Ttext $(SECOND_STAGE_ENTRY)

.PHONY = all clean

all: $(PREFIX)/second_stage.bin

$(PREFIX)/second_stage.bin: main.o ${OBJECTS} 
	$(LD) ${LD_FLAGS} -o $(PREFIX)/second_stage.bin $^ --oformat binary

$(PREFIX)/second_stage.elf: main.o ${OBJECTS}
	$(LD) ${LD_FLAGS} -o $(PREFIX)/second_stage.elf $^

%.o: %.c ${BUILD_H}
	$(SMLRC) ${SMLRC_FLAGS} $< tmp
	$(NASM) ${NASM_FLAGS} tmp -o $@

%.o: %.asm
	$(NASM) ${NASM_FLAGS} $< -o $@

clean:
	rm -rf tmp $(PREFIX)/second_stage.bin main.o ${OBJECTS}