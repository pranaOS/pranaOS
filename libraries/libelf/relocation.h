/**
 * @file relocation.h
 * @author Krisna Pranav
 * @brief Relocation
 * @version 6.0
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/concept.h>
#include <kernel/virtual_address.h>

namespace ELF 
{
    /**
     * @param base_address 
     * @return true 
     * @return false 
     */
    bool perform_relative_relocations(FlatPtr base_address);
} // namespace ELF
