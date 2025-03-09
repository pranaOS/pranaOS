/**
 * @file compilationunit.cpp
 * @author Krisna Pranav
 * @brief Compilation Unit
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#include "compilationunit.h"
#include "die.h"
#include <mods/bytereader.h>

namespace Debug::Dwarf 
{

    /**
     * @brief Construct a new CompilationUnit::CompilationUnit object
     * 
     * @param dwarf_info 
     * @param offset 
     * @param header 
     * @param line_program 
     */
    CompilationUnit::CompilationUnit(DwarfInfo const& dwarf_info, u32 offset, CompilationUnitHeader const& header, NonnullOwnPtr<LineProgram>&& line_program)
        : m_dwarf_info(dwarf_info)
        , m_offset(offset)
        , m_header(header)
        , m_abbreviations(dwarf_info, header.abbrev_offset())
        , m_line_program(move(line_program))
    {
        VERIFY(header.version() < 5 || header.unit_type() == CompilationUnitType::Full);
    }

    /**
     * @return DIE 
     */
    DIE CompilationUnit::root_die() const
    {
        return DIE(*this, m_offset + m_header.header_size());
    }

    /**
     * @param die_offset 
     * @return DIE 
     */
    DIE CompilationUnit::get_die_at_offset(u32 die_offset) const
    {
        VERIFY(die_offset >= offset() && die_offset < offset() + size());
        return DIE(*this, die_offset);
    }

    /**
     * @return Optional<FlatPtr> 
     */
    Optional<FlatPtr> CompilationUnit::base_address() const
    {
        if (m_has_cached_base_address)
            return m_cached_base_address;

        auto die = root_die();
        auto res = die.get_attribute(Attribute::LowPc);

        if (res.has_value()) {
            m_cached_base_address = res->as_addr();
        }

        m_has_cached_base_address = true;
        return m_cached_base_address;
    }

    /**
     * @return u64 
     */
    u64 CompilationUnit::address_table_base() const
    {
        if (m_has_cached_address_table_base)
            return m_cached_address_table_base;

        auto die = root_die();
        auto res = die.get_attribute(Attribute::AddrBase);
        if (res.has_value()) {
            VERIFY(res->form() == AttributeDataForm::SecOffset);
            m_cached_address_table_base = res->as_unsigned();
        }
        m_has_cached_address_table_base = true;
        return m_cached_address_table_base;
    }

    /**
     * @return u64 
     */
    u64 CompilationUnit::string_offsets_base() const
    {
        if (m_has_cached_string_offsets_base)
            return m_cached_string_offsets_base;

        auto die = root_die();
        auto res = die.get_attribute(Attribute::StrOffsetsBase);
        if (res.has_value()) {
            VERIFY(res->form() == AttributeDataForm::SecOffset);
            m_cached_string_offsets_base = res->as_unsigned();
        }
        m_has_cached_string_offsets_base = true;
        return m_cached_string_offsets_base;
    }

    /**
     * @return u64 
     */
    u64 CompilationUnit::range_lists_base() const
    {
        if (m_has_cached_range_lists_base)
            return m_cached_range_lists_base;

        auto die = root_die();
        auto res = die.get_attribute(Attribute::RngListsBase);
        if (res.has_value()) {
            VERIFY(res->form() == AttributeDataForm::SecOffset);
            m_cached_range_lists_base = res->as_unsigned();
        }
        m_has_cached_range_lists_base = true;
        return m_cached_range_lists_base;
    }

    /**
     * @param index 
     * @return FlatPtr 
     */
    FlatPtr CompilationUnit::get_address(size_t index) const
    {
        auto base = address_table_base();
        auto debug_addr_data = dwarf_info().debug_addr_data();
        VERIFY(base < debug_addr_data.size());
        auto addresses = debug_addr_data.slice(base);
        VERIFY(index * sizeof(FlatPtr) < addresses.size());
        FlatPtr value { 0 };
        ByteReader::load<FlatPtr>(addresses.offset_pointer(index * sizeof(FlatPtr)), value);
        return value;
    }

    /**
     * @param index 
     * @return char const* 
     */
    char const* CompilationUnit::get_string(size_t index) const
    {
        auto base = string_offsets_base();
        auto debug_str_offsets_data = dwarf_info().debug_str_offsets_data();
        VERIFY(base < debug_str_offsets_data.size());
        
        auto offsets = debug_str_offsets_data.slice(base);
        VERIFY(index * sizeof(u32) < offsets.size());
        auto offset = ByteReader::load32(offsets.offset_pointer(index * sizeof(u32)));
        return bit_cast<char const*>(dwarf_info().debug_strings_data().offset(offset));
    }

} // namespace Debug::Dwarf
