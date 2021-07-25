/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <base/Assertions.h>
#include <base/Memory.h>
#include <base/StringView.h>
#include <kernel/BootInfo.h>
#include <kernel/CMOS.h>
#include <kernel/FileSystem/Inode.h>
#include <kernel/Heap/kmalloc.h>
#include <kernel/Multiboot.h>
#include <kernel/Panic.h>
#include <kernel/Process.h>
#include <kernel/Sections.h>
#include <kernel/StdLib.h>
#include <kernel/vm/AnonymousVMObject.h>
#include <kernel/vm/ContiguousVMObject.h>
#include <kernel/vm/MemoryManager.h>
#include <kernel/vm/PageDirectory.h>
#include <kernel/vm/PhysicalRegion.h>
#include <kernel/vm/SharedInodeVMObject.h>

extern u8 start_of_kernel_image[];
extern u8 end_of_kernel_image[];
extern u8 start_of_kernel_text[];
extern u8 start_of_kernel_data[];
extern u8 end_of_kernel_bss[];
extern u8 start_of_ro_after_init[];
extern u8 end_of_ro_after_init[];
extern u8 start_of_unmap_after_init[];
extern u8 end_of_unmap_after_init[];
extern u8 start_of_kernel_ksyms[];
extern u8 end_of_kernel_ksyms[];

extern multiboot_module_entry_t multiboot_copy_boot_modules_array[16];
extern size_t multiboot_copy_boot_modules_count;

__attribute__((section(".super_pages"))) static u8 super_pages[1 * MiB];

namespace Kernel {

static MemoryManager* s_the;
RecursiveSpinLock s_mm_lock;

MemoryManager& MM
{
    return *s_the;
}

bool MemoryManager::is_initialized()
{
    return s_the != nullptr;
}

UNMAP_AFTER_INIT MemoryManager::MemoryManager()
{
    s_the = this;

    ScopedSpinLock lock(s_mm_lock);
    parse_memory_map();
    write_cr3(kernel_page_directory().cr3());
    protect_kernel_image();

    if (!commit_user_physical_pages(2))
        VERIFY_NOT_REACHED();

    m_shared_zero_page = allocate_committed_user_physical_page();

    m_lazy_committed_page = allocate_committed_user_physical_page();
}

UNMAP_AFTER_INIT MemoryManager::~MemoryManager()
{
}

UNMAP_AFTER_INIT void MemoryManager::protect_kernel_image()
{
    ScopedSpinLock page_lock(kernel_page_directory().get_lock());
    for (auto i = start_of_kernel_text; i < start_of_kernel_data; i += PAGE_SIZE) {
        auto& pte = *ensure_pte(kernel_page_directory(), VirtualAddress(i));
        pte.set_writable(false);
    }
    if (Processor::current().has_feature(CPUFeature::NX)) {

        for (auto i = start_of_kernel_data; i < end_of_kernel_image; i += PAGE_SIZE) {
            auto& pte = *ensure_pte(kernel_page_directory(), VirtualAddress(i));
            pte.set_execute_disabled(true);
        }
    }
}

UNMAP_AFTER_INIT void MemoryManager::protect_readonly_after_init_memory()
{
    ScopedSpinLock mm_lock(s_mm_lock);
    ScopedSpinLock page_lock(kernel_page_directory().get_lock());
    for (auto i = (FlatPtr)&start_of_ro_after_init; i < (FlatPtr)&end_of_ro_after_init; i += PAGE_SIZE) {
        auto& pte = *ensure_pte(kernel_page_directory(), VirtualAddress(i));
        pte.set_writable(false);
        flush_tlb(&kernel_page_directory(), VirtualAddress(i));
    }
}

void MemoryManager::unmap_text_after_init()
{
    ScopedSpinLock mm_lock(s_mm_lock);
    ScopedSpinLock page_lock(kernel_page_directory().get_lock());

    auto start = page_round_down((FlatPtr)&start_of_unmap_after_init);
    auto end = page_round_up((FlatPtr)&end_of_unmap_after_init);

    for (auto i = start; i < end; i += PAGE_SIZE) {
        auto& pte = *ensure_pte(kernel_page_directory(), VirtualAddress(i));
        pte.clear();
        flush_tlb(&kernel_page_directory(), VirtualAddress(i));
    }

    dmesgln("Unmapped {} KiB of kernel text after init! :^)", (end - start) / KiB);
}

void MemoryManager::unmap_ksyms_after_init()
{
    ScopedSpinLock mm_lock(s_mm_lock);
    ScopedSpinLock page_lock(kernel_page_directory().get_lock());

    auto start = page_round_down((FlatPtr)start_of_kernel_ksyms);
    auto end = page_round_up((FlatPtr)end_of_kernel_ksyms);
 
    for (auto i = start; i < end; i += PAGE_SIZE) {
        auto& pte = *ensure_pte(kernel_page_directory(), VirtualAddress(i));
        pte.clear();
        flush_tlb(&kernel_page_directory(), VirtualAddress(i));
    }

    dmesgln("Unmapped {} KiB of kernel symbols after init! :^)", (end - start) / KiB);
}

UNMAP_AFTER_INIT void MemoryManager::register_reserved_ranges()
{
    VERIFY(!m_physical_memory_ranges.is_empty());
    ContiguousReservedMemoryRange range;
    for (auto& current_range : m_physical_memory_ranges) {
        if (current_range.type != PhysicalMemoryRangeType::Reserved) {
            if (range.start.is_null())
                continue;
            m_reserved_memory_ranges.append(ContiguousReservedMemoryRange { range.start, current_range.start.get() - range.start.get() });
            range.start.set((FlatPtr) nullptr);
            continue;
        }
        if (!range.start.is_null()) {
            continue;
        }
        range.start = current_range.start;
    }
    if (m_physical_memory_ranges.last().type != PhysicalMemoryRangeType::Reserved)
        return;
    if (range.start.is_null())
        return;
    m_reserved_memory_ranges.append(ContiguousReservedMemoryRange { range.start, m_physical_memory_ranges.last().start.get() + m_physical_memory_ranges.last().length - range.start.get() });
}

bool MemoryManager::is_allowed_to_mmap_to_userspace(PhysicalAddress start_address, Range const& range) const
{
    VERIFY(!m_reserved_memory_ranges.is_empty());
    for (auto& current_range : m_reserved_memory_ranges) {
        if (!(current_range.start <= start_address))
            continue;
        if (!(current_range.start.offset(current_range.length) > start_address))
            continue;
        if (current_range.length < range.size())
            return false;
        return true;
    }
    return false;
}

}