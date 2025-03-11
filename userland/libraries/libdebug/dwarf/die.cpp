/**
 * @file die.cpp
 * @author Krisna Pranav
 * @brief die
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#include "die.h"
#include "compilationunit.h"
#include "dwarfinfo.h"
#include <mods/bytebuffer.h>
#include <mods/memorystream.h>

namespace Debug::Dwarf 
{

    /**
     * @brief Construct a new DIE::DIE object
     * 
     * @param unit 
     * @param offset 
     * @param parent_offset 
     */
    DIE::DIE(CompilationUnit const& unit, u32 offset, Optional<u32> parent_offset)
        : m_compilation_unit(unit)
    {
        rehydrate_from(offset, parent_offset);
    }

    /**
     * @param offset 
     * @param parent_offset 
     */
    void DIE::rehydrate_from(u32 offset, Optional<u32> parent_offset)
    {
        m_offset = offset;

        InputMemoryStream stream(m_compilation_unit.dwarf_info().debug_info_data());
        stream.discard_or_error(m_offset);
        stream.read_LEB128_unsigned(m_abbreviation_code);
        m_data_offset = stream.offset();

        if (m_abbreviation_code == 0) {
            m_tag = EntryTag::None;
        } else {
            auto abbreviation_info = m_compilation_unit.abbreviations_map().get(m_abbreviation_code);
            VERIFY(abbreviation_info);

            m_tag = abbreviation_info->tag;
            m_has_children = abbreviation_info->has_children;

            for (auto& attribute_spec : abbreviation_info->attribute_specifications) {
                m_compilation_unit.dwarf_info().get_attribute_value(attribute_spec.form, attribute_spec.value, stream, &m_compilation_unit);
            }
        }
        m_size = stream.offset() - m_offset;
        m_parent_offset = parent_offset;
    }

    /**
     * @param attribute 
     * @return Optional<AttributeValue> 
     */
    Optional<AttributeValue> DIE::get_attribute(Attribute const& attribute) const
    {
        InputMemoryStream stream { m_compilation_unit.dwarf_info().debug_info_data() };
        stream.discard_or_error(m_data_offset);

        auto abbreviation_info = m_compilation_unit.abbreviations_map().get(m_abbreviation_code);
        VERIFY(abbreviation_info);

        for (auto const& attribute_spec : abbreviation_info->attribute_specifications) {
            auto value = m_compilation_unit.dwarf_info().get_attribute_value(attribute_spec.form, attribute_spec.value, stream, &m_compilation_unit);
            if (attribute_spec.attribute == attribute) {
                return value;
            }
        }
        return {};
    }

    /**
     * @param callback 
     */
    void DIE::for_each_child(Function<void(DIE const& child)> callback) const
    {
        if (!m_has_children)
            return;

        auto current_child = DIE(m_compilation_unit, m_offset + m_size, m_offset);
        while (true) {
            callback(current_child);
            if (current_child.is_null())
                break;
            if (!current_child.has_children()) {
                current_child.rehydrate_from(current_child.offset() + current_child.size(), m_offset);
                continue;
            }

            auto sibling = current_child.get_attribute(Attribute::Sibling);
            u32 sibling_offset = 0;
            if (sibling.has_value()) {
                sibling_offset = sibling.value().as_unsigned();
            } else {
                current_child.for_each_child([&](DIE const& sub_child) {
                    sibling_offset = sub_child.offset() + sub_child.size();
                });
            }
            current_child.rehydrate_from(sibling_offset, m_offset);
        }
    }

} // namespace Debug::Dwarf
