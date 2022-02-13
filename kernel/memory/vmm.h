/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <ak/list.h>
#include <ak/types.h>
#include <info.h>
#include <pmm.h>
#include <stdint.h>

/**
 * @brief kernel heap top + bottom 
 * 
 */
#define KERNEL_HEAP_TOP 0xF0000000
#define KERNEL_HEAP_BOTTOM 0xD0000000

/**
 * @brief user heap
 * 
 */
#define USER_HEAP_TOP 0x40000000

/**
 * @brief vm area struct 
 * 
 */
struct vm_area_struct;

/**
 * @brief mm struct [empty]
 * 
 */
struct mm_struct;

/**
 * @brief PAGE_PTE_FLAGS[PRESENT, WRITABLE, USER, WRITETHOUGH, NOT CACHEABLE, ACCESSED, DIRTY, PAT, GLOBAL ,FRAME]
 * 
 */
enum PAGE_PTE_FLAGS {

	I86_PTE_PRESENT = 1,		   
	I86_PTE_WRITABLE = 2,		   
	I86_PTE_USER = 4,			   
	I86_PTE_WRITETHOUGH = 8,	   
	I86_PTE_NOT_CACHEABLE = 0x10,  
	I86_PTE_ACCESSED = 0x20,	   
	I86_PTE_DIRTY = 0x40,		   
	I86_PTE_PAT = 0x80,			   
	I86_PTE_CPU_GLOBAL = 0x100,	   
	I86_PTE_LV4_GLOBAL = 0x200,	   
	I86_PTE_FRAME = 0x7FFFF000	   
};