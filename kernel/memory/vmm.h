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


/**
 * @brief pt entry
 * 
 */
typedef uint32_t pt_entry;

/**
 * @brief PDE[PRESENT, WRITABLE, USER, PWT, PCD, ACCESSED, DIRTY, 4MB, CPUGLOBAL, LV4GLOBAL, FRAME]
 * 
 */
enum PAGE_PDE_FLAGS {

	I86_PDE_PRESENT = 1,		 
	I86_PDE_WRITABLE = 2,		 
	I86_PDE_USER = 4,			 
	I86_PDE_PWT = 8,			 
	I86_PDE_PCD = 0x10,			 
	I86_PDE_ACCESSED = 0x20,	 
	I86_PDE_DIRTY = 0x40,		 
	I86_PDE_4MB = 0x80,			 
	I86_PDE_CPU_GLOBAL = 0x100,	 
	I86_PDE_LV4_GLOBAL = 0x200,	 
	I86_PDE_FRAME = 0x7FFFF000	 
};

/**
 * @brief pd entry
 * 
 */
typedef uint32_t pd_entry;

/**
 * @brief PAGES PER TABLE + PER DIR
 * 
 */
#define PAGES_PER_TABLE 1024
#define PAGES_PER_DIR 1024

/**
 * @brief page
 * 
 */
struct page {
	uint32_t frame;
    uint32_t virtual;
	struct list_head sibling;
};

/**
 * @brief pages
 * 
 */
struct pages {
	uint32_t paddr;
	uint32_t number_of_frames;
	uint32_t vaddr;
};

struct ptable {
	pt_entry m_entries[PAGES_PER_TABLE];
};

struct pdirectory {
	pd_entry m_entries[PAGES_PER_DIR];
};

struct pdirectory *vmm_get_directory();
void vmm_map_address(struct pdirectory *dir, uint32_t virt, uint32_t phys, uint32_t flags);
void vmm_unmap_address(struct pdirectory *va_dir, uint32_t virt);
void vmm_unmap_range(struct pdirectory *va_dir, uint32_t vm_start, uint32_t vm_end);
void *create_kernel_stack(int32_t blocks);
struct pdirectory *vmm_create_address_space(struct pdirectory *dir);
uint32_t vmm_get_physical_address(uint32_t vaddr, bool is_page);
struct pdirectory *vmm_fork(struct pdirectory *va_dir);

void *sbrk(size_t n);
void *kmalloc(size_t n);
void *kcalloc(size_t n, size_t size);
void *krealloc(void *ptr, size_t size);
void kfree(void *ptr);
void *kalign_heap(size_t size);

struct vm_area_struct *get_unmapped_area(uint32_t addr, uint32_t len);
int32_t do_mmap(uint32_t addr,
				size_t len, uint32_t prot,
				uint32_t flag, int32_t fd, off_t off);
int do_munmap(struct mm_struct *mm, uint32_t addr, size_t len);
uint32_t do_brk(uint32_t addr, size_t len);

void kmap(struct page *p);
void kmaps(struct pages *p);
void kunmap(struct page *p);
void kunmaps(struct pages *p);

void vmm_init();