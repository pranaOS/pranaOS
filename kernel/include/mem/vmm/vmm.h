/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_MEM_VMM_VMM_H
#define _KERNEL_MEM_VMM_VMM_H

// includes
#include <libkernel/libkernel.h>
#include <mem/pmm.h>
#include <platform/generic/vmm/consts.h>
#include <platform/generic/vmm/pde.h>
#include <platform/generic/vmm/pte.h>

#define vmm_is_kernel_address(add) (add >= KERNEL_BASE)

enum PAGE_FLAGS {
    PAGE_WRITABLE = 0x1,
    PAGE_READABLE = 0x2,
    PAGE_EXECUTABLE = 0x4,
    PAGE_NOT_CACHEABLE = 0x8,
    PAGE_COW = 0x10,
    PAGE_USER = 0x20,
};

#define USER_PAGE true
#define KERNEL_PAGE false
#define PAGE_CHOOSE_OWNER(vaddr) (vaddr >= KERNEL_BASE ? 0 : PAGE_USER)

typedef struct {
    page_desc_t entities[VMM_PTE_COUNT];
} ptable_t;

int vmm_setup();

int vmm_allocate_ptable(uint32_t vaddr);
int vmm_free_ptable(uint32_t vaddr, struct dynamic_array* zones);
int vmm_free_pdir(pdirectory_t* pdir, struct dynamic_array* zones);

int vmm_map_page(uint32_t vaddr, uint32_t paddr, uint32_t settings);
int vmm_map_pages(uint32_t vaddr, uint32_t paddr, uint32_t n_pages, uint32_t settings);
int vmm_unmap_page(uint32_t vaddr);
int vmm_unmap_pages(uint32_t vaddr, uint32_t n_pages);
int vmm_copy_page(uint32_t to_vaddr, uint32_t src_vaddr, ptable_t* src_ptable);

pdirectory_t* vmm_new_user_pdir();
pdirectory_t* vmm_new_forked_user_pdir();
void* vmm_bring_to_kernel(uint8_t* src, uint32_t length);
void vmm_prepare_active_pdir_for_copying_at(uint32_t dest_vaddr, uint32_t length);
void vmm_copy_to_user(void* dest, void* src, uint32_t length);
void vmm_copy_to_pdir(pdirectory_t* pdir, void* src, uint32_t dest_vaddr, uint32_t length);
void vmm_zero_user_pages(pdirectory_t* pdir);

#endif 