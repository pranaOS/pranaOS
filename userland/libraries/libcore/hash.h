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