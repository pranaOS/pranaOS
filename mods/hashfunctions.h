/**
 * @file hashfunctions.h
 * @author Krisna Pranav
 * @brief HashFunctions
 * @version 1.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/types.h>

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

    return key;
}

/**
 * @brief ptr_hash
 * 
 * @param ptr 
 * @return constexpr unsigned int 
 */
constexpr unsigned int ptr_hash(FlatPtr ptr) {
    if constexpr(sizeof(ptr) == 8) 
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