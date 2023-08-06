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
    public:
        explicit Image(const u8, size_t, bool verbose_logging = true);

        ~Image();

        void dump() const;

        bool is_valid() const
        {
            return m_valid;
        }

        bool parse();

        bool is_within_image(const void* address, size_t size) const
        {
            if (address < m_buffer)
                return false;
            
            if (((const u8*)address + size) > m_buffer + m_size)
                return false;
            
            return true;
        }

        class Section;
        class RealocationSection;
        class Symbol;
        class Realocation;
    
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