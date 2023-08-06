/**
 * @file image.h
 * @author Krisna Pranav
 * @brief Image
 * @version 1.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/string_view.h>
#include <mods/byte_buffer.h>
#include <mods/ownptr.h>
#include <mods/hashmap.h>
#include <kernel/virtual_address.h>
#include <libelf/exec_elf.h>

namespace ELF
{
    class Image
    {
    
    private:
        const char* raw_data(unsigned offset) const;
        const Elf32_Ehdr& header() const;
        const Elf32_Shdr& section_header(unsigned) const;
        const Elf32_Phdr& program_header_internal(unsigned) const

        StringView table_string(unsigned offset) const;
        StringView section_header_table_string(unsigned offset) const;
        StringView section_index_to_string(unsigned index) const;
        StringView table_string(unsigned table_index, unsigned offset) const;

        const u8 m_buffer { nullptr };

        size_t m_size { 0 };
        bool m_verbose_logging { true };
        bool m_valid { false };
        unsigned m_symbol_table_section_index { 0 };
        unsigned m_string_table_section_index { 0 };
    };
}