/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Assertions.h>
#include <base/Memory.h>
#include <kernel/heap/SlabAllocator.h>
#include <kernel/heap/kmalloc.h>
#include <kernel/Sections.h>
#include <kernel/vm/Region.h>

#define SANITIZE_SLABS

namespace Kernel {

template<size_t templated_slab_size>
class SlabAllocator {
public:
    SlabAllocator() = default;

    void init(size_t size)
    {
        m_base = kmalloc_eternal(size);
        m_end = (u8*)m_base + size;
        FreeSlab* slabs = (FreeSlab*)m_base;
        m_slab_count = size / templated_slab_size;
        for (size_t i = 1; i < m_slab_count; ++i) {
            slabs[i].next = &slabs[i - 1];
        }
        slabs[0].next = nullptr;
        m_freelist = &slabs[m_slab_count - 1];
        m_num_allocated = 0;
    }

    constexpr size_t slab_size() const { return templated_slab_size; }
    size_t slab_count() const { return m_slab_count; }

    void* alloc()
    {
        FreeSlab* free_slab;
        {

            ScopedCritical critical;
            FreeSlab* next_free;
            free_slab = m_freelist.load(Base::memory_order_consume);
            do {
                if (!free_slab)
                    return kmalloc(slab_size());

                next_free = free_slab->next;
            } while (!m_freelist.compare_exchange_strong(free_slab, next_free, Base::memory_order_acq_rel));

            m_num_allocated++;
        }

#ifdef SANITIZE_SLABS
        memset(free_slab, SLAB_ALLOC_SCRUB_BYTE, slab_size());
#endif
        return free_slab;
    }

    void dealloc(void* ptr)
    {
        VERIFY(ptr);
        if (ptr < m_base || ptr >= m_end) {
            kfree(ptr);
            return;
        }
        FreeSlab* free_slab = (FreeSlab*)ptr;
#ifdef SANITIZE_SLABS
        if (slab_size() > sizeof(FreeSlab*))
            memset(free_slab->padding, SLAB_DEALLOC_SCRUB_BYTE, sizeof(FreeSlab::padding));
#endif

        ScopedCritical critical;
        FreeSlab* next_free = m_freelist.load(Base::memory_order_consume);
        do {
            free_slab->next = next_free;
        } while (!m_freelist.compare_exchange_strong(next_free, free_slab, Base::memory_order_acq_rel));

        m_num_allocated--;
    }

    size_t num_allocated() const { return m_num_allocated; }
    size_t num_free() const { return m_slab_count - m_num_allocated; }

private:
    struct FreeSlab {
        FreeSlab* next;
        char padding[templated_slab_size - sizeof(FreeSlab*)];
    };

    Atomic<FreeSlab*> m_freelist { nullptr };
    Atomic<size_t, Base::MemoryOrder::memory_order_relaxed> m_num_allocated;
    size_t m_slab_count;
    void* m_base { nullptr };
    void* m_end { nullptr };

    static_assert(sizeof(FreeSlab) == templated_slab_size);
};

}