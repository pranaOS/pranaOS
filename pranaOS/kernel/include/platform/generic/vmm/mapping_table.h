/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_PLATFORM_GENERIC_VMM_MAPPING_TABLE_H
#define _KERNEL_PLATFORM_GENERIC_VMM_MAPPING_TABLE_H

// includes
#include <libkernel/types.h>

struct mapping_entry {
    uint32_t paddr;
    uint32_t vaddr;
    uint32_t flags;
    uint16_t pages;
    uint16_t last; 
};
typedef struct mapping_entry mapping_entry_t;

extern mapping_entry_t kernel_mapping_table[]; 
extern mapping_entry_t extern_mapping_table[]; 

#endif 