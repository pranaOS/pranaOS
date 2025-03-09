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

namespace Debug::Dwarf
{
    /**
     * @brief Construct a new DIE::DIE object
     * 
     */
    DIE::DIE()
    {}

    /**
     * @param offset 
     * @param parent_offset 
     */
    void DIE::rehydrate_from(u32 offset, Optional<u32> parent_offset)
    {
        m_offset = offset;
    }
} // namespace Debug::Dwarf
