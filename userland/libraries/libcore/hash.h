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