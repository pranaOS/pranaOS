/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

#define HASHMAP_FUNCS_DECLARE(name, key_type, data_type)                 \
	data_type *name##_hashmap_put(struct hashmap *map,                   \
								  const key_type *key, data_type *data); \
	data_type *name##_hashmap_get(const struct hashmap *map,             \
								  const key_type *key);                  \
	data_type *name##_hashmap_remove(struct hashmap *map,                \
									 const key_type *key);               \
	const key_type *name##_hashmap_iter_get_key(                         \
		const struct hashmap_iter *iter);                                \
	data_type *name##_hashmap_iter_get_data(                             \
		const struct hashmap_iter *iter);                                \
	void name##_hashmap_iter_set_data(const struct hashmap_iter *iter,   \
									  data_type *data);                  \
	int name##_hashmap_foreach(const struct hashmap *map,                \
							   int (*func)(const key_type *, data_type *, void *), void *arg);


struct hashmap_iter;
struct hashmap_entry;

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