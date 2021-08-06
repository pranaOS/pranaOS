/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_MEM_VMM_ZONER_H
#define _KERNEL_MEM_VMM_ZONER_H

// includes
#include <libkernel/types.h>

struct __zone {
    union {
        uint32_t start;
        uint8_t* ptr;
    };
    uint32_t len;
};

typedef struct __zone zone_t;

void zoner_init(uint32_t start_vaddr);
void zoner_place_bitmap();

zone_t zoner_new_zone(uint32_t size);
zone_t zoner_new_zone_aligned(uint32_t size, uint32_t alignment);
int zoner_free_zone(zone_t zone);

#endif 