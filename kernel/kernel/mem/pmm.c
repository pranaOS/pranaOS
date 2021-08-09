/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libkernel/log.h>
#include <mem/pmm.h>

static inline uint32_t _pmm_round_ceil(uint32_t value);
static inline uint32_t _pmm_round_floor(uint32_t value);
static inline void _pmm_mat_alloc_block(uint32_t block_id);
static inline void _pmm_mat_free_block(uint32_t block_id);
static inline bool _pmm_mat_tesblock(uint32_t block_id);
uint32_t _pmm_first_free_block();
uint32_t _pmm_first_free_block_n(uint32_t t_size);
void _pmm_init_region(uint32_t t_region_start, uint32_t t_region_length);
void _pmm_deinit_region(uint32_t t_region_start, uint32_t t_region_length);
void _pmm_deinit_mat();
void _pmm_calc_ram_size(mem_desc_t* mem_desc);
void _pmm_allocate_mat(void* t_mat_base);
static int last_i = 0;

static inline uint32_t _pmm_round_ceil(uint32_t value)
{
    if ((value & (PMM_BLOCK_SIZE - 1)) != 0) {
        value += PMM_BLOCK_SIZE;
        value &= (0xffffffff - (PMM_BLOCK_SIZE - 1));
    }
    return value;
}

static inline uint32_t _pmm_round_floor(uint32_t value)
{
    return (value & (0xffffffff - (PMM_BLOCK_SIZE - 1)));
}

static inline void _pmm_mat_alloc_block(uint32_t block_id)
{
    pmm_mat[block_id / PMM_BLOCKS_PER_BYTE] |= (1 << (block_id % PMM_BLOCKS_PER_BYTE));
}

static inline void _pmm_mat_free_block(uint32_t block_id)
{
    pmm_mat[block_id / PMM_BLOCKS_PER_BYTE] &= ~(1 << (block_id % PMM_BLOCKS_PER_BYTE));
}

static inline bool _pmm_mat_tesblock(uint32_t block_id)
{
    return (pmm_mat[block_id / PMM_BLOCKS_PER_BYTE] >> (block_id % PMM_BLOCKS_PER_BYTE)) & 1;
}

uint32_t _pmm_first_free_block()
{
    uint32_t* mat_big = (uint32_t*)pmm_mat;
    for (uint32_t i = last_i; i < pmm_mat_size / 4; i++) {
        if (mat_big[i] != 0xffffffff) {
            for (uint8_t j = 0; j < 32; j++) {
                uint32_t currenblock = i * 32 + j;
                if (!_pmm_mat_tesblock(currenblock)) {
                    return currenblock;
                }
            }
        }
    }
    return 0x0;
}
