/**
 * @file validation.h
 * @author Krisna Pranav
 * @brief validation
 * @version 1.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <libelf/exec_elf.h>

namespace ELF 
{

    /**
     * @param elf_header 
     * @param file_size 
     * @param verbose 
     * @return true 
     * @return false 
     */
    bool validate_elf_header(const Elf32_Ehdr& elf_header, size_t file_size, bool verbose = true);

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
    bool validate_program_headers(const Elf32_Ehdr& elf_header, size_t file_size, const u8* buffer, size_t buffer_size, String* interpreter_path, bool verbose = true);

} // namespace ELF