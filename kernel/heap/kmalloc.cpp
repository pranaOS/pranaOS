/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Assertions.h>
#include <base/NonnullOwnPtrVector.h>
#include <base/Types.h>
#include <kernel/Debug.h>
#include <kernel/heap/Heap.h>
#include <kernel/heap/kmalloc.h>
#include <kernel/KSyms.h>
#include <kernel/Panic.h>
#include <kernel/PerformanceManager.h>
#include <kernel/Sections.h>
#include <kernel/SpinLock.h>
#include <kernel/StdLib.h>
#include <kernel/vm/MemoryManager.h>

#define CHUNK_SIZE 32
#define POOL_SIZE (2 * MiB)
#define ETERNAL_RANGE_SIZE (3 * MiB)

namespace std {
const nothrow_t nothrow;
}

static RecursiveSpinLock s_lock;

static void kmalloc_allocate_backup_memory();

struct KmallocGlobalHeap {
    struct ExpandGlobalHeap {
        KmallocGlobalHeap& m_global_heap;

        ExpandGlobalHeap(KmallocGlobalHeap& global_heap)
            : m_global_heap(global_heap)
        {
        }

        bool m_adding { false };
        bool add_memory(size_t allocation_request)
        {
            if (!MemoryManager::is_initialized()) {
                if constexpr (KMALLOC_DEBUG) {
                    dmesgln("kmalloc: Cannot expand heap before MM is initialized!");
                }
                return false;
            }
            VERIFY(!m_adding);
            TemporaryChange change(m_adding, true);

            auto region = move(m_global_heap.m_backup_memory);
            if (!region) {
                if constexpr (KMALLOC_DEBUG) {
                    dmesgln("kmalloc: Cannot expand heap: no backup memory");
                }
                return false;
            }

            if constexpr (KMALLOC_DEBUG) {
                dmesgln("kmalloc: Adding memory to heap at {}, bytes: {}", region->vaddr(), region->size());
            }

            auto& subheap = m_global_heap.m_heap.add_subheap(region->vaddr().as_ptr(), region->size());
            m_global_heap.m_subheap_memory.append(region.release_nonnull());

            ScopeGuard guard([&]() {
                Processor::current().deferred_call_queue(kmalloc_allocate_backup_memory);
            });

            if (subheap.free_bytes() < allocation_request) {

                size_t memory_size = page_round_up(decltype(m_global_heap.m_heap)::calculate_memory_for_bytes(allocation_request));

                memory_size += 1 * MiB;
                region = MM.allocate_kernel_region(memory_size, "kmalloc subheap", Region::Access::Read | Region::Access::Write, AllocationStrategy::AllocateNow);
                if (region) {
                    dbgln("kmalloc: Adding even more memory to heap at {}, bytes: {}", region->vaddr(), region->size());

                    m_global_heap.m_heap.add_subheap(region->vaddr().as_ptr(), region->size());
                    m_global_heap.m_subheap_memory.append(region.release_nonnull());
                } else {
                    dbgln("kmalloc: Could not expand heap to satisfy allocation of {} bytes", allocation_request);
                    return false;
                }
            }
            return true;
        }

        bool remove_memory(void* memory)
        {
            for (size_t i = 0; i < m_global_heap.m_subheap_memory.size(); i++) {
                if (m_global_heap.m_subheap_memory[i].vaddr().as_ptr() == memory) {
                    auto region = m_global_heap.m_subheap_memory.take(i);
                    if (!m_global_heap.m_backup_memory) {
                        if constexpr (KMALLOC_DEBUG) {
                            dmesgln("kmalloc: Using removed memory as backup: {}, bytes: {}", region->vaddr(), region->size());
                        }
                        m_global_heap.m_backup_memory = move(region);
                    } else {
                        if constexpr (KMALLOC_DEBUG) {
                            dmesgln("kmalloc: Queue removing memory from heap at {}, bytes: {}", region->vaddr(), region->size());
                        }
                        Processor::deferred_call_queue([this, region = move(region)]() mutable {
                            ScopedSpinLock lock(s_lock);
                            if (!m_global_heap.m_backup_memory) {
                                if constexpr (KMALLOC_DEBUG) {
                                    dmesgln("kmalloc: Queued memory region at {}, bytes: {} will be used as new backup", region->vaddr(), region->size());
                                }
                                m_global_heap.m_backup_memory = move(region);
                            } else {
                                if constexpr (KMALLOC_DEBUG) {
                                    dmesgln("kmalloc: Queued memory region at {}, bytes: {} will be freed now", region->vaddr(), region->size());
                                }
                            }
                        });
                    }
                    return true;
                }
            }

            if constexpr (KMALLOC_DEBUG) {
                dmesgln("kmalloc: Cannot remove memory from heap: {}", VirtualAddress(memory));
            }
            return false;
        }
    };
    typedef ExpandableHeap<CHUNK_SIZE, KMALLOC_SCRUB_BYTE, KFREE_SCRUB_BYTE, ExpandGlobalHeap> HeapType;

    HeapType m_heap;
    NonnullOwnPtrVector<Region> m_subheap_memory;
    OwnPtr<Region> m_backup_memory;

    KmallocGlobalHeap(u8* memory, size_t memory_size)
        : m_heap(memory, memory_size, ExpandGlobalHeap(*this))
    {
    }
    void allocate_backup_memory()
    {
        if (m_backup_memory)
            return;
        m_backup_memory = MM.allocate_kernel_region(1 * MiB, "kmalloc subheap", Region::Access::Read | Region::Access::Write, AllocationStrategy::AllocateNow);
    }

    size_t backup_memory_bytes() const
    {
        return m_backup_memory ? m_backup_memory->size() : 0;
    }
};

READONLY_AFTER_INIT static KmallocGlobalHeap* g_kmalloc_global;
alignas(KmallocGlobalHeap) static u8 g_kmalloc_global_heap[sizeof(KmallocGlobalHeap)];

__attribute__((section(".heap"))) static u8 kmalloc_eternal_heap[ETERNAL_RANGE_SIZE];
__attribute__((section(".heap"))) static u8 kmalloc_pool_heap[POOL_SIZE];

static size_t g_kmalloc_bytes_eternal = 0;
static size_t g_kmalloc_call_count;
static size_t g_kfree_call_count;
static size_t g_nested_kfree_calls;
bool g_dump_kmalloc_stacks;

static u8* s_next_eternal_ptr;
READONLY_AFTER_INIT static u8* s_end_of_eternal_range;

static void kmalloc_allocate_backup_memory()
{
    g_kmalloc_global->allocate_backup_memory();
}

void kmalloc_enable_expand()
{
    g_kmalloc_global->allocate_backup_memory();
}

static inline void kmalloc_verify_nospinlock_held()
{
    if constexpr (KMALLOC_VERIFY_NO_SPINLOCK_HELD) {
        VERIFY(!Processor::current().in_critical());
    }
}

UNMAP_AFTER_INIT void kmalloc_init()
{
    memset(kmalloc_eternal_heap, 0, sizeof(kmalloc_eternal_heap));
    memset(kmalloc_pool_heap, 0, sizeof(kmalloc_pool_heap));
    g_kmalloc_global = new (g_kmalloc_global_heap) KmallocGlobalHeap(kmalloc_pool_heap, sizeof(kmalloc_pool_heap));

    s_lock.initialize();

    s_next_eternal_ptr = kmalloc_eternal_heap;
    s_end_of_eternal_range = s_next_eternal_ptr + sizeof(kmalloc_eternal_heap);
}

void* kmalloc_eternal(size_t size)
{
    kmalloc_verify_nospinlock_held();

    size = round_up_to_power_of_two(size, sizeof(void*));

    ScopedSpinLock lock(s_lock);
    void* ptr = s_next_eternal_ptr;
    s_next_eternal_ptr += size;
    VERIFY(s_next_eternal_ptr < s_end_of_eternal_range);
    g_kmalloc_bytes_eternal += size;
    return ptr;
}

void* kmalloc(size_t size)
{
    kmalloc_verify_nospinlock_held();
    ScopedSpinLock lock(s_lock);
    ++g_kmalloc_call_count;

    if (g_dump_kmalloc_stacks && Kernel::g_kernel_symbols_available) {
        dbgln("kmalloc({})", size);
        Kernel::dump_backtrace();
    }

    void* ptr = g_kmalloc_global->m_heap.allocate(size);
    if (!ptr) {
        PANIC("kmalloc: Out of memory (requested size: {})", size);
    }

    Thread* current_thread = Thread::current();
    if (!current_thread)
        current_thread = Processor::idle_thread();
    if (current_thread)
        PerformanceManager::add_kmalloc_perf_event(*current_thread, size, (FlatPtr)ptr);

    return ptr;
}

void kfree_sized(void* ptr, size_t size)
{
    (void)size;
    return kfree(ptr);
}

void kfree(void* ptr)
{
    if (!ptr)
        return;

    kmalloc_verify_nospinlock_held();
    ScopedSpinLock lock(s_lock);
    ++g_kfree_call_count;
    ++g_nested_kfree_calls;

    if (g_nested_kfree_calls == 1) {
        Thread* current_thread = Thread::current();
        if (!current_thread)
            current_thread = Processor::idle_thread();
        if (current_thread)
            PerformanceManager::add_kfree_perf_event(*current_thread, 0, (FlatPtr)ptr);
    }

    g_kmalloc_global->m_heap.deallocate(ptr);
    --g_nested_kfree_calls;
}

size_t kmalloc_good_size(size_t size)
{
    return size;
}

[[gnu::malloc, gnu::alloc_size(1), gnu::alloc_align(2)]] static void* kmalloc_aligned_cxx(size_t size, size_t alignment)
{
    VERIFY(alignment <= 4096);
    void* ptr = kmalloc(size + alignment + sizeof(ptrdiff_t));
    size_t max_addr = (size_t)ptr + alignment;
    void* aligned_ptr = (void*)(max_addr - (max_addr % alignment));
    ((ptrdiff_t*)aligned_ptr)[-1] = (ptrdiff_t)((u8*)aligned_ptr - (u8*)ptr);
    return aligned_ptr;
}

void* operator new(size_t size)
{
    void* ptr = kmalloc(size);
    VERIFY(ptr);
    return ptr;
}

void* operator new(size_t size, const std::nothrow_t&) noexcept
{
    return kmalloc(size);
}

void* operator new(size_t size, std::align_val_t al)
{
    void* ptr = kmalloc_aligned_cxx(size, (size_t)al);
    VERIFY(ptr);
    return ptr;
}

void* operator new(size_t size, std::align_val_t al, const std::nothrow_t&) noexcept
{
    return kmalloc_aligned_cxx(size, (size_t)al);
}

void* operator new[](size_t size)
{
    void* ptr = kmalloc(size);
    VERIFY(ptr);
    return ptr;
}

void* operator new[](size_t size, const std::nothrow_t&) noexcept
{
    return kmalloc(size);
}

void operator delete(void*) noexcept
{
    VERIFY_NOT_REACHED();
}

void operator delete(void* ptr, size_t size) noexcept
{
    return kfree_sized(ptr, size);
}

void operator delete(void* ptr, size_t, std::align_val_t) noexcept
{
    return kfree_aligned(ptr);
}

void operator delete[](void*) noexcept
{

    VERIFY_NOT_REACHED();
}

void operator delete[](void* ptr, size_t size) noexcept
{
    return kfree_sized(ptr, size);
}

void get_kmalloc_stats(kmalloc_stats& stats)
{
    ScopedSpinLock lock(s_lock);
    stats.bytes_allocated = g_kmalloc_global->m_heap.allocated_bytes();
    stats.bytes_free = g_kmalloc_global->m_heap.free_bytes() + g_kmalloc_global->backup_memory_bytes();
    stats.bytes_eternal = g_kmalloc_bytes_eternal;
    stats.kmalloc_call_count = g_kmalloc_call_count;
    stats.kfree_call_count = g_kfree_call_count;
}