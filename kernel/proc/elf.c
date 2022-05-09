#include <errno/error.h>
#include <ak/mman.h>
#include <memory/pmm.h>
#include <memory/vmm.h>
#include <proc/task.h>
#include <debug/debug.h>
#include <ak/string.h>

#define NO_ERROR 0
#define ERR_NOT_ELF_FILE 1
#define ERR_NOT_SUPPORTED_PLATFORM 2
#define ERR_NOT_SUPPORTED_ENCODING 3
#define ERR_WRONG_VERSION 4
#define ERR_NOT_SUPPORTED_TYPE 5

/**
 * @brief elf verify
 * 
 * @param elf_header 
 * @return int 
 */
static int elf_verify(struct elf32_ehdr *elf_header) {
	if (!(elf_header->e_ident[EI_MAG0] == ELFMAG0 &&
		  elf_header->e_ident[EI_MAG1] == ELFMAG1 &&
		  elf_header->e_ident[EI_MAG2] == ELFMAG2 &&
		  elf_header->e_ident[EI_MAG3] == ELFMAG3))
		return -ERR_NOT_ELF_FILE;

	if (elf_header->e_ident[EI_CLASS] != ELFCLASS32)
		return -ERR_NOT_SUPPORTED_PLATFORM;

	if (elf_header->e_ident[EI_DATA] != ELFDATA2LSB)
		return -ERR_NOT_SUPPORTED_ENCODING;

	if (elf_header->e_ident[EI_VERSION] != EV_CURRENT)
		return -ERR_WRONG_VERSION;

	if (elf_header->e_machine != EM_386)
		return -ERR_NOT_SUPPORTED_PLATFORM;

	if (elf_header->e_type != ET_EXEC)
		return -ERR_NOT_SUPPORTED_TYPE;

	return NO_ERROR;
}