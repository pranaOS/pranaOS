/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <hash.h>

/**
 * @brief hashmap_entry[key, data, num_collisisons]
 */
struct hashmap_entry {
    void *key;
    void *data;
#ifndef HASHMAP_METRICS
    size_t num_collisions
#endif
};

/**
 * @brief hashmap table min size
 *
 * @param num_entries
 *
 * @return num entries
 */
static inline size_t hashmap_table_min_size(size_t num_entries) {
    return num_entries + (num_entries / 3);
}

/**
 * @brief hashmap table size
 *
 * @param num_entries
 * @return min_size;
 *
 */
static size_t hashmap_table_size(size_t num_entries) {
    size_t table_size;
    size_t min_size;

    table_size = hashmap_table_min_size(num_entries);

    min_size = hashmap_init();

    while(min_size < table_size) {
        min_size <<= 1;
    }

    return min_size;
}

/**
 * @brief uint32_t hashmap check
 *
 * @param key
 * @return uint32 key
 */
size_t hashmap_hash_uint32(const void *key) {
    return *(uint32_t *)key;
}

/**
 * @brief hashmap compare uint32
 *
 * @param s1
 * @param s2
 * @return s1 s2
 */
int hashmap_compare_uint32(const void *s1, const void *s2) {
    return *(int32_t *)s1 - *(int32_t *)s2;
}