/**
 * @file string_hash.h
 * @author Krisna Pranav
 * @brief string hash
 * @version 6.0
 * @date 2024-08-29
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/types.h>

namespace Mods
{
    /**
     * @param characters 
     * @param length 
     * @param seed 
     * @return constexpr u32 
     */
    constexpr u32 string_hash(char const* characters, size_t length, u32 seed = 0)
    {
        u32 hash = seed;
        for(size_t i = 0; i < length; ++i)
        {
            hash += (u32)characters[i];
            hash += (hash << 10);
            hash ^= (hash >> 6);
        }
        hash += hash << 3;
        hash ^= hash >> 11;
        hash += hash << 15;
        return hash;
    }

    /**
     * @param characters 
     * @param length 
     * @param seed 
     * @return constexpr u32 
     */
    constexpr u32 case_insensitive_string_hash(char const* characters, size_t length, u32 seed = 0)
    {
        auto to_lowercase = [](char ch) -> u32
        {
            if(ch >= 'A' && ch <= 'Z')
                return static_cast<u32>(ch) + 0x20;
            return static_cast<u32>(ch);
        };

        u32 hash = seed;
        for(size_t i = 0; i < length; ++i)
        {
            hash += to_lowercase(characters[i]);
            hash += (hash << 10);
            hash ^= (hash >> 6);
        }
        hash += hash << 3;
        hash ^= hash >> 11;
        hash += hash << 15;
        return hash;
    }
} // namespace Mods

using Mods::string_hash;