/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <filesystem/vfs.h>
#include <memory/vmm.h>
#include <stdint.h>

typedef uint16_t elf32_half;  
typedef uint32_t elf32_off;	  
typedef uint32_t elf32_addr;  
typedef uint32_t elf32_word;  
typedef int32_t elf32_sword;  

#define EI_MAG0 0		 
#define EI_MAG1 1		 
#define EI_MAG2 2		 
#define EI_MAG3 3		 
#define EI_CLASS 4		 
#define EI_DATA 5		 
#define EI_VERSION 6	 
#define EI_OSABI 7		 
#define EI_ABIVERSION 8	 
#define EI_PAD = 9		 
#define EI_NIDENT 16

#define ELFMAG0 0x7F  
#define ELFMAG1 'E'	  
#define ELFMAG2 'L'	  
#define ELFMAG3 'F'	  

#define ELFCLASSNONE 0	
#define ELFCLASS32 1	
#define ELFCLASS64 2	

#define ELFDATANONE 0  
#define ELFDATA2LSB 1  
#define ELFDATA2MSB 2  

#define EV_NONE 0	  
#define EV_CURRENT 1  

#define ET_NONE 0  
#define ET_REL 1   
#define ET_EXEC 2  
#define ET_DYN 3   
#define ET_CORE 4  

#define EM_NONE 0	
#define EM_M32 1	
#define EM_SPARC 2	
#define EM_386 3	
#define EM_68K 4	
#define EM_88K 5	
#define EM_486 6	
#define EM_860 7	
#define EM_MIPS 8	

struct elf32_ehdr {
	unsigned char e_ident[EI_NIDENT];
	elf32_half e_type;
	elf32_half e_machine;
	elf32_word e_version;
	elf32_addr e_entry; 
	elf32_off e_phoff;
	elf32_off e_shoff;
	elf32_word e_flags;
	elf32_half e_ehsize;
	elf32_half e_phentsize;
	elf32_half e_phnum;
	elf32_half e_shentsize;
	elf32_half e_shnum;
	elf32_half e_shstrndx;
};

#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4
#define SHT_HASH 5
#define SHT_DYNAMIC 6
#define SHT_NOTE 7
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_SHLIB 10
#define SHT_DYNSYM 11
#define SHT_NUM 12
#define SHT_LOPROC 0x70000000
#define SHT_HIPROC 0x7fffffff
#define SHT_LOUSER 0x80000000
#define SHT_HIUSER 0xffffffff

#define SHF_WRITE 0x1
#define SHF_ALLOC 0x2
#define SHF_EXECINSTR 0x4
#define SHF_MASKPROC 0xf0000000

#define SHN_UNDEF 0
#define SHN_LORESERVE 0xff00
#define SHN_LOPROC 0xff00
#define SHN_HIPROC 0xff1f
#define SHN_ABS 0xfff1
#define SHN_COMMON 0xfff2
#define SHN_HIRESERVE 0xffff

struct elf32_shdr {
	elf32_word sh_name;
	elf32_word sh_type;
	elf32_word sh_flags;
	elf32_addr sh_addr;
	elf32_off sh_offset;
	elf32_word sh_size;
	elf32_word sh_link;
	elf32_word sh_info;
	elf32_word sh_addralign;
	elf32_word sh_entsize;
};

#define PF_X 0x1
#define PF_W 0x2
#define PF_R 0x4

#define PT_NULL 0
#define PT_LOAD 1
#define PT_DYNAMIC 2
#define PT_INTERP 3
#define PT_NOTE 4
#define PT_SHLIB 5
#define PT_PHDR 6
#define PT_TLS 7		   
#define PT_LOOS 0x60000000 
#define PT_HIOS 0x6fffffff 
#define PT_LOPROC 0x70000000
#define PT_HIPROC 0x7fffffff
#define PT_GNU_EH_FRAME 0x6474e550

#define PT_GNU_STACK (PT_LOOS + 0x474e551)

struct elf32_phdr {
	elf32_word p_type;
	elf32_off p_offset;
	elf32_addr p_vaddr;
	elf32_addr p_paddr;
	elf32_word p_filesz;
	elf32_word p_memsz;
	elf32_word p_flags;
	elf32_word p_align;
};