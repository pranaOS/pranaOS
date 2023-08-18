/**
 * @file hashfunctions.h
 * @author Krisna Pranav
 * @brief HashFunctions
 * @version 6.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "types.h"

/**
 * @brief int_hash
 * 
 * @param key 
 * @return constexpr unsigned 
 */
constexpr unsigned int_hash(u32 key) {
    key += ~(key << 15);
    key ^= (key >> 10);
    key += (key << 3);
    key ^= (key >> 6);
    key += ~(key << 11);
    key ^= (key >> 16);
    return key;
}

/**
 * @brief double_hash
 * 
 * @param key 
 * @return constexpr unsigned 
 */
constexpr unsigned double_hash(u32 key) {
    key = ~key + (key >> 23);
    key ^= (key << 12);
    key ^= (key >> 7);
    key ^= (key << 2);
    key ^= (key >> 20);
    return key;
}

/**
 * @brief pair_int_hash
 * 
 * @param key1 
 * @param key2 
 * @return constexpr unsigned 
 */
constexpr unsigned pair_int_hash(u32 key1, u32 key2) {
    return int_hash((int_hash(key1) * 209) ^ (int_hash(key2 * 413)));
}

/**
 * @brief u64_hash
 * 
 * @param key 
 * @return constexpr unsigned 
 */
constexpr unsigned u64_hash(u64 key) {
    u32 first = key & 0xFFFFFFFF;
    u32 last = key >> 32;
    return pair_int_hash(first, last);
}

/**
 * @brief ptr_hash
 * 
 * @param ptr 
 * @return constexpr unsigned 
 */
constexpr unsigned ptr_hash(FlatPtr ptr) {
    if constexpr (sizeof(ptr) == 8)
        return u64_hash(ptr);
    else
        return int_hash(ptr);
}

/**
 * @brief ptr_hash
 * 
 * @param ptr 
 * @return unsigned 
 */
inline unsigned ptr_hash(const void* ptr) {
    return ptr_hash(FlatPtr(ptr));
}
