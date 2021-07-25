/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Memory.h>
#include <base/Singleton.h>
#include <kernel/prekernel/Prekernel.h>
#include <kernel/Process.h>
#include <kernel/Random.h>
#include <kernel/Sections.h>
#include <kernel/vm/MemoryManager.h>
#include <kernel/vm/PageDirectory.h>

extern u8 end_of_kernel_image[];

namespace Kernel {

static Base::Singleton<HashMap<FlatPtr, PageDirectory*>> s_cr3_map;

static HashMap<FlatPtr, PageDirectory*>& cr3_map()
{
    VERIFY_INTERRUPTS_DISABLED();
    return *s_cr3_map;
}

RefPtr<PageDirectory> PageDirectory::find_by_cr3(FlatPtr cr3)
{
    ScopedSpinLock lock(s_mm_lock);
    return cr3_map().get(cr3).value_or({});
}

UNMAP_AFTER_INIT PageDirectory::PageDirectory()
{
    FlatPtr start_of_range = ((FlatPtr)end_of_kernel_image & ~(FlatPtr)0x1fffff) + 0x200000;
    m_range_allocator.initialize_with_range(VirtualAddress(start_of_range), KERNEL_PD_END - start_of_range);
    m_identity_range_allocator.initialize_with_range(VirtualAddress(FlatPtr(0x00000000)), 0x00200000);
}

UNMAP_AFTER_INIT void PageDirectory::allocate_kernel_directory()
{

#if ARCH(X86_64)
    dmesgln("MM: boot_pml4t @ {}", boot_pml4t);
    m_pml4t = PhysicalPage::create(boot_pml4t, MayReturnToFreeList::No);
#endif
    dmesgln("MM: boot_pdpt @ {}", boot_pdpt);
    dmesgln("MM: boot_pd0 @ {}", boot_pd0);
    dmesgln("MM: boot_pd_kernel @ {}", boot_pd_kernel);
    m_directory_table = PhysicalPage::create(boot_pdpt, MayReturnToFreeList::No);
    m_directory_pages[0] = PhysicalPage::create(boot_pd0, MayReturnToFreeList::No);
    m_directory_pages[(kernel_base >> 30) & 0x1ff] = PhysicalPage::create(boot_pd_kernel, MayReturnToFreeList::No);
}

PageDirectory::PageDirectory(const RangeAllocator* parent_range_allocator)
{
    constexpr FlatPtr userspace_range_base = 0x00800000;
    FlatPtr userspace_range_ceiling = USER_RANGE_CEILING;

    ScopedSpinLock lock(s_mm_lock);
    if (parent_range_allocator) {
        m_range_allocator.initialize_from_parent(*parent_range_allocator);
    } else {
        size_t random_offset = (get_fast_random<u8>() % 32 * MiB) & PAGE_MASK;
        u32 base = userspace_range_base + random_offset;
        m_range_allocator.initialize_with_range(VirtualAddress(base), userspace_range_ceiling - base);
    }

#if ARCH(X86_64)
    m_pml4t = MM.allocate_user_physical_page();
    if (!m_pml4t)
        return;
#endif
    m_directory_table = MM.allocate_user_physical_page();
    if (!m_directory_table)
        return;
    auto kernel_pd_index = (kernel_base >> 30) & 0x1ffu;
    for (size_t i = 0; i < kernel_pd_index; i++) {
        m_directory_pages[i] = MM.allocate_user_physical_page();
        if (!m_directory_pages[i])
            return;
    }

    m_directory_pages[kernel_pd_index] = MM.kernel_page_directory().m_directory_pages[kernel_pd_index];

#if ARCH(X86_64)
    {
        auto& table = *(PageDirectoryPointerTable*)MM.quickmap_page(*m_pml4t);
        table.raw[0] = (FlatPtr)m_directory_table->paddr().as_ptr() | 7;
        MM.unquickmap_page();
    }
#endif

    {
        auto& table = *(PageDirectoryPointerTable*)MM.quickmap_page(*m_directory_table);
        for (size_t i = 0; i < sizeof(m_directory_pages) / sizeof(m_directory_pages[0]); i++) {
            if (m_directory_pages[i]) {
#if ARCH(I386)
                table.raw[i] = (FlatPtr)m_directory_pages[i]->paddr().as_ptr() | 1;
#else
                table.raw[i] = (FlatPtr)m_directory_pages[i]->paddr().as_ptr() | 7;
#endif
            }
        }

        u64 max_physical_address = (1ULL << Processor::current().physical_address_bit_width()) - 1;


        constexpr u64 pdpte_bit_flags = 0x80000000000000BF;

        for (auto table_entry : table.raw)
            VERIFY((table_entry & ~pdpte_bit_flags) <= max_physical_address);

        MM.unquickmap_page();
    }

    PageDirectoryEntry buffer;
    auto* kernel_pd = MM.quickmap_pd(MM.kernel_page_directory(), 0);
    memcpy(&buffer, kernel_pd, sizeof(PageDirectoryEntry));
    auto* new_pd = MM.quickmap_pd(*this, 0);
    memcpy(new_pd, &buffer, sizeof(PageDirectoryEntry));

    m_valid = true;

    cr3_map().set(cr3(), this);
}

PageDirectory::~PageDirectory()
{
    ScopedSpinLock lock(s_mm_lock);
    if (m_space)
        cr3_map().remove(cr3());
}

}