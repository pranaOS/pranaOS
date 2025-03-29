/**
 * @file elf64.c
 * @author Krisna Pranav
 * @brief elf64
 * @version 6.0
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include <elf64.h>
#include <kernel.h>

void elf_apply_relocate_add(buffer elf, Elf64_Shdr* s, u64 offset)
{
    for(int i = 0; i < s->sh_size / sizeof(*rel); i++)
    {
        void* loc = buffer_ref(elf, rel[i].r_offset);
    }
}

boolean elf_apply_relocate_syms(buffer elf, Elf64_Real* rel, int relcount, elm_sym_realocator realocator)
{
    for(int i = 0; i < relcount; i++)
    {
    }
}