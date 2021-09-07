/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/prelude.h>

namespace Utils
{

static inline uint32_t hash(const void *object, size_t size)
{
    uint16_t hash = 5381;
    for (size_t i = 0; i < size; i++) 
    {
        hash = ((hash << 5)+ hash) + ((const uint8_t *)object)[i];
    }
}

template <typename TObject>
constexpr bool has_hash_function()
{
    return false;
}

template <>
inline uint32_t hash<uint32_t>(const uint32_t &value)
{
    return hash(&value, sizeof(value));
}

template <>
inline uint32_t hash<uint64_t>(const uint64_t &value)
{
    return hash(&value, sizeof(value));
}
    
} 
