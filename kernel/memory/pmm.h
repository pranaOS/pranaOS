/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <multiboot/multiboot.h>
#include "info.h"

/**
 * @brief pmm frames per byte
 * 
 */
#define PMM_FRAMES_PER_BYTE 8

/**
 * @brief pmm frames size
 * 
 */
#define PMM_FRAME_SIZE 4096

/**
 * @brief pmm frame align
 * 
 */
#define PMM_FRAME_ALIGN PMM_FRAME_SIZE

/**
 * @brief page mask
 * 
 */
#define PAGE_MASK (~(PMM_FRAME_SIZE - 1))

/**
 * @brief page align
 * 
 */
#define PAGE_ALIGN(addr) (((addr) + PMM_FRAME_SIZE - 1) & PAGE_MASK)

/**
 * @brief pmm init 
 * 
 */
void pmm_init(struct multiboot_tag_basic_meminfo *, struct multiboot_tag_mmap *);

/**
 * @brief pmm alloc block
 * 
 * @return void* 
 */
void *pmm_alloc_block();

/**
 * @brief pmm aloc blocks
 * 
 * @param num 
 * @return void* 
 */
void *pmm_alloc_blocks(size_t num);
uint32_t get_total_frames();