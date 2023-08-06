/**
 * @file validation.cpp
 * @author Krisna Pranav
 * @brief validation
 * @version 1.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <libelf/validation.h>
#include <libelf/exec_elf.h>
#include <mods/assertions.h>
#include <mods/string.h>

namespace ELF
{

    /**
     * @param elf_header 
     * @param file_size 
     * @param verbose 
     * @return true 
     * @return false 
     */
    bool validate_elf_header(const Elf32_Ehdr& elf_header, size_t file_size, bool verbose)
    {
        if (!IS_ELF(elf_header)) {
            if (verbose)
                dbgputstr("File is not an ELF file.\n");
            return false;
        }

        if (ELFCLASS32 != elf_header.e_ident[EI_CLASS]) {
            if (verbose)
                dbgputstr("File is not a 32 bit ELF file.\n");
            
            return false;
        }

        if (ELFDATA2LSB != elf_header.e_ident[EI_DATA]) {
            if (verbose)
                dbgputstr("File is not a little endian ELF file.\n");
            return false;
        }

        if (EV_CURRENT != elf_header.e_ident[EI_VERSION]) {
            if (verbose)
                dbgputstr("File has unrecognized EFL version (%d)\n");
            return false;
        }

        if (0 != elf_header.e_ident[EI_ABIVERSION]) {
            if (verbose)
                dbgputstr("File has unknown SYSV ABI version (%d)\n", elf_header.e_ident[EI_ABIVERSION]);
            return false;
        }
    }

} // namespace ELF