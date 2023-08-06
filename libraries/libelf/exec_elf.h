/**
 * @file exec_elf.h
 * @author Krisna Pranav
 * @brief exec_elf
 * @version 1.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#ifndef _SYS_EXEC_ELF_H_
#define _SYS_EXEC_ELF_H_

#include <mods/types.h>

typedef uint8_t Elf_Byte;

typedef uint32_t Elf32_Addr;
typedef uint32_t Elf32_Off;
typedef int32_t Elf32_Sword;
typedef uint32_t Elf32_Word;
typedef uint64_t Elf32_Lword;

#ifdef __alpha__
typedef int64_t Elf64_Sword;
typedef uint64_t Elf64_Word;
#else
typedef int32_t Elf64_Sword;
typedef uint32_t Elf64_Word;
#endif

#endif 