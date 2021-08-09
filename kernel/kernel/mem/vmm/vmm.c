/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libkernel/bits/errno.h>
#include <libkernel/libkernel.h>
#include <libkernel/lock.h>
#include <libkernel/log.h>
#include <mem/kmalloc.h>
#include <mem/vmm/vmm.h>
#include <mem/vmm/zoner.h>
#include <platform/generic/cpu.h>
#include <platform/generic/system.h>
#include <platform/generic/vmm/mapping_table.h>
#include <platform/generic/vmm/pf_types.h>
#include <tasking/tasking.h>


#define pdir_t pdirectory_t
#define VMM_TOTAL_PAGES_PER_TABLE VMM_PTE_COUNT
#define VMM_TOTAL_TABLES_PER_DIRECTORY VMM_PDE_COUNT
#define PDIR_SIZE sizeof(pdirectory_t)
#define PTABLE_SIZE sizeof(ptable_t)
#define IS_INDIVIDUAL_PER_DIR(index) (index < VMM_KERNEL_TABLES_START || (index == VMM_OFFSET_IN_DIRECTORY(pspace_zone.start)))

static pdir_t* _vmm_kernel_pdir;
static lock_t _vmm_lock;
static zone_t pspace_zone;
static uint32_t kernel_ptables_start_paddr = 0x0;

#define vmm_kernel_pdir_phys2virt(paddr) ((void*)((uint32_t)paddr + KERNEL_BASE - KERNEL_PM_BASE))


inline static void* _vmm_pspace_get_vaddr_of_active_pdir();
inline static void* _vmm_pspace_get_nth_active_ptable(uint32_t n);
inline static void* _vmm_pspace_get_vaddr_of_active_ptable(uint32_t vaddr);
static int _vmm_split_pspace();
static void _vmm_pspace_init();
static void _vmm_pspace_gen(pdirectory_t*);
static void* _vmm_kernel_convert_vaddr2paddr(uint32_t vaddr);
static void* _vmm_convert_vaddr2paddr(uint32_t vaddr);

inline static void* _vmm_alloc_kernel_pdir();
inline static uint32_t _vmm_alloc_pdir_paddr();
inline static uint32_t _vmm_alloc_ptable_paddr();
inline static uint32_t _vmm_alloc_page_paddr();
static bool _vmm_init_switch_to_kernel_pdir();
static void _vmm_map_init_kernel_pages(uint32_t paddr, uint32_t vaddr);
static bool _vmm_create_kernel_ptables();
static bool _vmm_map_kernel();

inline static uint32_t _vmm_round_ceil_to_page(uint32_t value);
inline static uint32_t _vmm_round_floor_to_page(uint32_t value);
inline static table_desc_t* _vmm_pdirectory_lookup(pdirectory_t* t_pdir, uint32_t t_addr);
inline static page_desc_t* _vmm_ptable_lookup(ptable_t* t_ptable, uint32_t t_addr);

static bool _vmm_is_copy_on_write(uint32_t vaddr);
static int _vmm_resolve_copy_on_write(proc_t* p, uint32_t vaddr);
static void _vmm_ensure_cow_for_page(uint32_t vaddr);
static void _vmm_ensure_cow_for_range(uint32_t vaddr, uint32_t length);
static int _vmm_copy_page_to_resolve_cow(proc_t* p, uint32_t vaddr, ptable_t* src_ptable, int page_index);

static bool _vmm_is_zeroing_on_demand(uint32_t vaddr);
static void _vmm_resolve_zeroing_on_demand(uint32_t vaddr);

static int _vmm_self_test();

inline static void* _vmm_alloc_kernel_pdir()
{
    uint32_t paddr = (uint32_t)pmm_alloc_aligned(PDIR_SIZE, PDIR_SIZE);
    return vmm_kernel_pdir_phys2virt(paddr);
}

inline static uint32_t _vmm_alloc_pdir_paddr()
{
    return (uint32_t)pmm_alloc_aligned(PDIR_SIZE, PDIR_SIZE);
}

inline static uint32_t _vmm_alloc_ptable_paddr()
{
    return (uint32_t)pmm_alloc(PTABLE_SIZE);
}

inline static uint32_t _vmm_alloc_ptables_to_cover_page()
{
    return (uint32_t)pmm_alloc_aligned(VMM_PAGE_SIZE, VMM_PAGE_SIZE);
}

inline static void _vmm_free_ptables_to_cover_page(uint32_t addr)
{
    pmm_free((void*)addr, VMM_PAGE_SIZE);
}

inline static uint32_t _vmm_alloc_page_paddr()
{
    return (uint32_t)pmm_alloc_aligned(VMM_PAGE_SIZE, VMM_PAGE_SIZE);
}

inline static void _vmm_free_page_paddr(uint32_t addr)
{
    pmm_free((void*)addr, VMM_PAGE_SIZE);
}

static zone_t _vmm_alloc_mapped_zone(uint32_t size, uint32_t alignment)
{
    if (size % VMM_PAGE_SIZE) {
        size += VMM_PAGE_SIZE - (size % VMM_PAGE_SIZE);
    }
    if (alignment % VMM_PAGE_SIZE) {
        alignment += VMM_PAGE_SIZE - (alignment % VMM_PAGE_SIZE);
    }

    // TODO: Currently only sequence allocation is implemented.
    zone_t zone = zoner_new_zone_aligned(size, alignment);
    uint32_t paddr = (uint32_t)pmm_alloc_aligned(size, alignment);
    vmm_map_pages_lockless(zone.start, paddr, size / VMM_PAGE_SIZE, PAGE_READABLE | PAGE_WRITABLE);
    return zone;
}

static int _vmm_free_mapped_zone(zone_t zone)
{
    ptable_t* ptable = (ptable_t*)_vmm_pspace_get_vaddr_of_active_ptable(zone.start);
    page_desc_t* page = _vmm_ptable_lookup(ptable, zone.start);
    pmm_free((void*)page_desc_get_frame(*page), zone.len);
    vmm_unmap_pages_lockless(zone.start, zone.len / VMM_PAGE_SIZE);
    zoner_free_zone(zone);
    return 0;
}

inline static pdirectory_t* _vmm_alloc_pdir()
{
    zone_t zone = _vmm_alloc_mapped_zone(PDIR_SIZE, PDIR_SIZE);
    return (pdirectory_t*)zone.ptr;
}

inline static void _vmm_free_pdir(pdirectory_t* pdir)
{
    if (pdir == _vmm_kernel_pdir) {
        return;
    }

    zone_t zone;
    zone.start = (uint32_t)pdir;
    zone.len = PDIR_SIZE;
    _vmm_free_mapped_zone(zone);
}


inline static void* _vmm_pspace_get_vaddr_of_active_pdir()
{
    return (void*)THIS_CPU->pdir;
}

inline static void* _vmm_pspace_get_nth_active_ptable(uint32_t n)
{
    return (void*)(pspace_zone.start + n * PTABLE_SIZE);
}

inline static void* _vmm_pspace_get_vaddr_of_active_ptable(uint32_t vaddr)
{
    return (void*)_vmm_pspace_get_nth_active_ptable(VMM_OFFSET_IN_DIRECTORY(vaddr));
}

static int _vmm_split_pspace()
{
    pspace_zone = zoner_new_zone(4 * MB);

    if (VMM_OFFSET_IN_TABLE(pspace_zone.start) != 0) {
        kpanic("WRONG PSPACE START ADDR");
    }

    _vmm_kernel_pdir = (pdir_t*)_vmm_alloc_kernel_pdir();
    THIS_CPU->pdir = (pdir_t*)_vmm_kernel_pdir;
    memset((void*)THIS_CPU->pdir, 0, PDIR_SIZE);
    return 0;
}