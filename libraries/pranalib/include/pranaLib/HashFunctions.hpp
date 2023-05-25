//
//  HashFunctions.hpp
//  Libraries
//
//  Created by Pranav on 28/06/22.
//

#pragma once

#include "Common.hpp"

constexpr uint32_t u32_hash(uint32_t x)
{
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

constexpr uint32_t u64_hash(uint64_t x)
{
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    x = x ^ (x >> 31);
    return x;
}

constexpr uint32_t string_hash(char const* characters, size_t length)
{
    constexpr uint32_t p = 31;
    constexpr uint32_t m = 1e9 + 9;
    uint32_t hash_value = 0;
    uint32_t p_pow = 1;
    for (size_t i = 0; i < length; i++) {
        hash_value = (hash_value + (characters[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}
