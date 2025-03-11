/**
 * @file validation.cpp
 * @author Krisna Pranav
 * @brief validation
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "validation.h"
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
                dbgprintf("File has unrecognized ELF version (%d), expected (%d)!\n", elf_header.e_ident[EI_VERSION], EV_CURRENT);
            return false;
        }

        if (ELFOSABI_SYSV != elf_header.e_ident[EI_OSABI]) {
            if (verbose)
                dbgprintf("File has unknown OS ABI (%d), expected SYSV(0)!\n", elf_header.e_ident[EI_OSABI]);
            return false;
        }

        if (0 != elf_header.e_ident[EI_ABIVERSION]) {
            if (verbose)
                dbgprintf("File has unknown SYSV ABI version (%d)!\n", elf_header.e_ident[EI_ABIVERSION]);
            return false;
        }

        if (EM_386 != elf_header.e_machine) {
            if (verbose)
                dbgprintf("File has unknown machine (%d), expected i386 (3)!\n", elf_header.e_machine);
            return false;
        }

        if (ET_EXEC != elf_header.e_type && ET_DYN != elf_header.e_type && ET_REL != elf_header.e_type && ET_CORE != elf_header.e_type) {
            if (verbose)
                dbgprintf("File has unloadable ELF type (%d), expected REL (1), EXEC (2), DYN (3) or CORE(4)!\n", elf_header.e_type);
            return false;
        }

        if (EV_CURRENT != elf_header.e_version) {
            if (verbose)
                dbgprintf("File has unrecognized ELF version (%d), expected (%d)!\n", elf_header.e_version, EV_CURRENT);
            return false;
        }

        if (sizeof(Elf32_Ehdr) != elf_header.e_ehsize) {
            if (verbose)
                dbgprintf("File has incorrect ELF header size..? (%d), expected (%zu)!\n", elf_header.e_ehsize, sizeof(Elf32_Ehdr));
            return false;
        }

        if (elf_header.e_phoff < elf_header.e_ehsize || (elf_header.e_shnum != SHN_UNDEF && elf_header.e_shoff < elf_header.e_ehsize)) {
            if (verbose) {
                dbgprintf("program header offset (%d) or section header offset (%d) overlap with ELF header!\n",
                    elf_header.e_phoff, elf_header.e_shoff);
            }
            return false;
        }

        if (elf_header.e_phoff > file_size || elf_header.e_shoff > file_size) {
            if (verbose) {
                dbgprintf("program header offset (%d) or section header offset (%d) are past the end of the file!\n",
                    elf_header.e_phoff, elf_header.e_shoff);
            }
            return false;
        }

        if (elf_header.e_phnum == 0 && elf_header.e_phoff != 0) {
            if (verbose)
                dbgputstr("File has no program headers, but it does have a program header offset (%d)!\n", elf_header.e_phoff);
            return false;
        }

        if (elf_header.e_phnum != 0 && elf_header.e_phoff != elf_header.e_ehsize) {
            if (verbose) {
                dbgprintf("File does not have program headers directly after the ELF header? program header offset (%d), expected (%d).\n",
                    elf_header.e_phoff, elf_header.e_ehsize);
            }
            return false;
        }

        if (0 != elf_header.e_flags) {
            if (verbose)
                dbgprintf("File has incorrect ELF header flags...? (%d), expected (%d).\n", elf_header.e_flags, 0);
            return false;
        }

        if (0 != elf_header.e_phnum && sizeof(Elf32_Phdr) != elf_header.e_phentsize) {
            if (verbose)
                dbgprintf("File has incorrect program header size..? (%d), expected (%zu).\n", elf_header.e_phentsize, sizeof(Elf32_Phdr));
            return false;
        }

        if (sizeof(Elf32_Shdr) != elf_header.e_shentsize) {
            if (verbose)
                dbgprintf("File has incorrect section header size..? (%d), expected (%zu).\n", elf_header.e_shentsize, sizeof(Elf32_Shdr));
            return false;
        }

        size_t end_of_last_program_header = elf_header.e_phoff + (elf_header.e_phnum * elf_header.e_phentsize);
        
        if (end_of_last_program_header > file_size) {
            if (verbose)
                dbgprintf("End of last program header (%zu) is past the end of the file!\n", end_of_last_program_header);
            return false;
        }

        if (elf_header.e_shoff != SHN_UNDEF && elf_header.e_shoff < end_of_last_program_header) {
            if (verbose) {
                dbgprintf("Section header table begins at file offset %d, which is within program headers [ %d - %zu ]!\n",
                    elf_header.e_shoff, elf_header.e_phoff, end_of_last_program_header);
            }
            return false;
        }

        size_t end_of_last_section_header = elf_header.e_shoff + (elf_header.e_shnum * elf_header.e_shentsize);

        if (end_of_last_section_header > file_size) {
            if (verbose)
                dbgprintf("End of last section header (%zu) is past the end of the file!\n", end_of_last_section_header);
            return false;
        }

        if (elf_header.e_shstrndx != SHN_UNDEF && elf_header.e_shstrndx >= elf_header.e_shnum) {
            if (verbose)
                dbgprintf("Section header string table index (%d) is not a valid index given we have %d section headers!\n", elf_header.e_shstrndx, elf_header.e_shnum);
            return false;
        }

        return true;
    }

    /**
     * @param elf_header 
     * @param file_size 
     * @param buffer 
     * @param buffer_size 
     * @param interpreter_path 
     * @param verbose 
     * @return true 
     * @return false 
     */
    bool validate_program_headers(const Elf32_Ehdr& elf_header, size_t file_size, const u8* buffer, size_t buffer_size, String* interpreter_path, bool verbose)
    {
        size_t end_of_last_program_header = elf_header.e_phoff + (elf_header.e_phnum * elf_header.e_phentsize);
        if (end_of_last_program_header > buffer_size) {
            if (verbose)
                dbgprintf("Unable to parse program headers from buffer, buffer too small! Buffer size: %zu, End of program headers %zu\n",
                    buffer_size, end_of_last_program_header);
            return false;
        }

        if (file_size < buffer_size) {
            dbgputstr("We somehow read more from a file than was in the file in the first place!\n");
            ASSERT_NOT_REACHED();
        }

        size_t num_program_headers = elf_header.e_phnum;

        auto program_header_begin = (const Elf32_Phdr*)&(buffer[elf_header.e_phoff]);

        for (size_t header_index = 0; header_index < num_program_headers; ++header_index) {
            auto& program_header = program_header_begin[header_index];
            switch (program_header.p_type) {
            case PT_INTERP:
                if (ET_DYN != elf_header.e_type) {
                    if (verbose)
                        dbgprintf("Found PT_INTERP header (%zu) in non-DYN ELF object! What? We can't handle this!\n", header_index);
                    return false;
                }
                // We checked above that file_size was >= buffer size. We only care about buffer size anyway, we're trying to read this!
                if (program_header.p_offset + program_header.p_filesz > buffer_size) {
                    if (verbose)
                        dbgprintf("Found PT_INTERP header (%zu), but the .interp section was not within our buffer :( Your program will not be loaded today.\n", header_index);
                    return false;
                }
                if (interpreter_path)
                    *interpreter_path = String((const char*)&buffer[program_header.p_offset], program_header.p_filesz - 1);
                break;
            case PT_LOAD:
            case PT_DYNAMIC:
            case PT_NOTE:
            case PT_PHDR:
            case PT_TLS:
                if (program_header.p_offset + program_header.p_filesz > file_size) {
                    if (verbose)
                        dbgprintf("Program header %zu segment leaks beyond end of file!\n", header_index);
                    return false;
                }
                if ((program_header.p_flags & PF_X) && (program_header.p_flags & PF_W)) {
                    if (verbose)
                        dbgprintf("Program header %zu segment is marked write and execute\n", header_index);
                    return false;
                }
                break;
            case PT_GNU_STACK:
                if (program_header.p_flags & PF_X) {
                    if (verbose)
                        dbgprintf("Possible Validating an ELF with executable stack.\n");
                }
                break;
            case PT_GNU_RELRO:
                if ((program_header.p_flags & PF_X) && (program_header.p_flags & PF_W)) {
                    if (verbose)
                        dbgprintf("Program header %zu segment is marked write and execute\n", header_index);
                    return false;
                }
                break;
            default:
                if (verbose)
                    dbgprintf("Found program header (%zu) of unrecognized type %x!\n", header_index, program_header.p_type);
                return false;
            }
        }
        return true;
    }

} // namespace ELF
