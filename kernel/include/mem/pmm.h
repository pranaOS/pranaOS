/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_MEM_PMM_H
#define _KERNEL_MEM_PMM_H

// includes
#include <libkernel/types.h>
#include <platform/generic/pmm/settings.h>

typedef struct {
    uint32_t startLo;
    uint32_t startHi;
    uint32_t sizeLo;
    uint32_t sizeHi;
    uint32_t type;
    uint32_t acpi_3_0;
} memory_map_t;

typedef struct {
    uint16_t memory_map_size;
    uint16_t kernel_size;
} mem_desc_t;

static uint32_t pmm_ram_size;
static uint32_t pmm_max_blocks;
static uint32_t pmm_used_blocks;
static uint8_t* pmm_mat;
static uint32_t pmm_mat_size;

#endif 