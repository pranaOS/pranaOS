/**
 * @file hashes.h
 * @author Krisna Pranav
 * @brief hashes
 * @version 6.0
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/string_view.h>

namespace ELF 
{
    /**
     * @param name 
     * @return constexpr u32 
     */
    constexpr u32 compute_sysv_hash(StringView name)
    {
        u32 hash = 0;

        for (auto ch : name) {
            hash = hash << 4;
            hash += ch;

            const u32 top_nibble_of_hash = hash & 0xf0000000u;
            hash ^= top_nibble_of_hash >> 24;
            hash &= ~top_nibble_of_hash;
        }

        return hash;
    }

    /**
     * @param name 
     * @return constexpr u32 
     */
    constexpr u32 compute_gnu_hash(StringView name)
    {
        u32 hash = 5381;

        for (auto ch : name)
            hash = hash * 33 + ch;

        return hash;
    }

} // namespace ELF