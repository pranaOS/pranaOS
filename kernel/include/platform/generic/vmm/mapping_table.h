/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libkern/types.h>

struct mapping_entry {
    uint32_t paddr;
    uint32_t vaddr;
    uint32_t flags;
    uint16_t pages;
    uint16_t last; // 1 if an element is the last.
};
typedef struct mapping_entry mapping_entry_t;

extern mapping_entry_t kernel_mapping_table[]; // Should be in kernelspace
extern mapping_entry_t extern_mapping_table[]; // Maps after kernel tables are ready, so can be outside kernelspace
