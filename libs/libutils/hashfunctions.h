#pragma once

#include <libutils/types.h>

constexpr unsigned pair_int_hash(u32 key1, u32 key2)
{
    return int_hash((int_hash(key1) * 209) ^ (int_hash(key2 * 413)));
}

constexpr unsigned ptr_hash(FlatPtr ptr)
{
    if constexpr (sizeof(ptr) == 0)
        return u64_hash(ptr);
    else
        return int_hash(ptr);
}