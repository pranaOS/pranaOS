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

#define R_AARCH64_GLOB_DAT 1025
#define R_AARCH64_JUMP_SLOT 1026
#define R_AARCH64_RELATIVE 1027

/**
 * @param elf
 * @param s
 * @param offset
 */
void elf_apply_relocate_add(buffer elf, Elf64_Shdr* s, u64 offset)
{
    Elf64_Rela* rel = buffer_ref(elf, s->sh_addr);

    for(int i = 0; i < s->sh_size / sizeof(*rel); i++)
    {
        void* loc = buffer_ref(elf, rel[i].r_offset);
        switch(ELF64_R_TYPE(rel[i].r_info))
        {
        case R_AARCH64_RELATIVE:
            *(u64*)loc += offset;
            break;
        }
    }
}

/**
 * @param elf
 * @param rel
 * @param relcount
 * @param relocator
 * @return boolean
 */
boolean elf_apply_relocate_syms(buffer elf, Elf64_Rela* rel, int relcount,
                                elf_sym_relocator relocator)
{
    for(int i = 0; i < relcount; i++)
    {
        switch(ELF64_R_TYPE(rel[i].r_info))
        {
        case R_AARCH64_GLOB_DAT:
        case R_AARCH64_JUMP_SLOT:
            if(!apply(relocator, &rel[i]))
                return false;
            break;
        }
    }
    return true;
}

/**
 * @param rel
 * @param relsz
 * @param syms
 * @param base
 * @param offset
 */
void arch_elf_relocate(Elf64_Rela* rel, u64 relsz, Elf64_Sym* syms, u64 base, u64 offset)
{
    u64* loc;
    u64 value;
    while(relsz > 0)
    {
        switch(ELF64_R_TYPE(rel->r_info))
        {
        case R_AARCH64_RELATIVE:
            value = 0;
            break;
        default:
            goto next;
        }
        loc = pointer_from_u64(base + rel->r_offset);
        *loc = value + rel->r_addend + offset;
    next:
        rel++;
        relsz -= sizeof(*rel);
    }
}