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
#define ETHERNAL_RANGE_SIZE (3 * MiB)

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

static size_t g_kmalloc_bytes_eternal = 0;

static u8 s_next_eternal_ptr;