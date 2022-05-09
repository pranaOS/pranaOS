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