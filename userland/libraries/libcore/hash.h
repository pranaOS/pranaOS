/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

struct hashmap_iter;
struct hashmap_entry;

/**
 * @brief hash map struct [table size, num entries]
 * 
 */
struct hashmap {
	size_t table_size_init;
	size_t table_size;
	size_t num_entries;
	struct hashmap_entry *table;
	size_t (*hash)(const void *);
	int (*key_compare)(const void *, const void *);
	void *(*key_alloc)(const void *);
	void (*key_free)(void *);
};

/**
 * @brief hash map init function
 * 
 * @param map 
 * @param hash_func 
 * @param key_compare_func 
 * @param initial_size 
 * @return int 
 */
int hashmap_init(struct hashmap *map, size_t (*hash_func)(const void *), int (*key_compare_func)(const void *, const void *), size_t initial_size);

/**
 * @brief hashmap compare uint32
 *
 * @param s1
 * @param s2
 * @return
 */
int hashmap_compare_uint32(const void *s1, const void *s2);

/**
 * @brief hashmap destroy
 * 
 * @param map 
 */
void hashmap_destroy(struct hashmap *map);

/**
 * @brief hash map clear 
 * 
 * @param map 
 */
void hashmap_clear(struct hashmap *map);

/**
 * @brief hashmap size
 *
 * @param map
 * @return
 */
size_t hashmap_size(const struct hashmap *map);

/**
 * @breif hashmap has string
 *
 * @param key
 * @return
 */
size_t hashmap_hash_string(const void *key);

/**
 * @brief hashmap hash uint32
 *
 * @param key
 * @return
 */
size_t hashmap_hash_uint32_t(const void *key);

/**
 * @breif hashmap iterator
 *
 * @param map
 * @return
 */
struct hashmap_iter *hashmap_iter(const struct hashmap *map);

#ifndef HASHMAP_METER

/**
 * @brief hash map load factor
 *
 * @param map
 * @return
 */
double hashmap_load_factor(const struct hashmap *map);

double hashmap_collisions_mean(const struct hashmap *map);

#endif