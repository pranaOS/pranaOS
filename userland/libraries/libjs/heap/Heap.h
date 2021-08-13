/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <base/HashTable.h>
#include <base/IntrusiveList.h>
#include <base/Noncopyable.h>
#include <base/NonnullOwnPtr.h>
#include <base/Types.h>
#include <base/Vector.h>
#include <libcore/Forward.h>
#include <libjs/Forward.h>
#include <libjs/heap/BlockAllocator.h>
#include <libjs/heap/Cell.h>
#include <libjs/heap/CellAllocator.h>
#include <libjs/heap/Handle.h>
#include <libjs/runtime/Object.h>
#include <libjs/runtime/WeakContainer.h>

namespace JS {

class Heap {
    BASE_MAKE_NONCOPYABLE(Heap);
    BASE_MAKE_NONMOVABLE(Heap);

public:
    explicit Heap(VM&);
    ~Heap();

    template<typename T, typename... Args>
    T* allocate_without_global_object(Args&&... args)
    {
        auto* memory = allocate_cell(sizeof(T));
        new (memory) T(forward<Args>(args)...);
        return static_cast<T*>(memory);
    }

    template<typename T, typename... Args>
    T* allocate(GlobalObject& global_object, Args&&... args)
    {
        auto* memory = allocate_cell(sizeof(T));
        new (memory) T(forward<Args>(args)...);
        auto* cell = static_cast<T*>(memory);
        constexpr bool is_object = IsBaseOf<Object, T>;
        if constexpr (is_object)
            static_cast<Object*>(cell)->disable_transitions();
        cell->initialize(global_object);
        if constexpr (is_object)
            static_cast<Object*>(cell)->enable_transitions();
        return cell;
    }

    enum class CollectionType {
        CollectGarbage,
        CollectEverything,
    };

    void collect_garbage(CollectionType = CollectionType::CollectGarbage, bool print_report = false);

    VM& vm() { return m_vm; }

    bool should_collect_on_every_allocation() const { return m_should_collect_on_every_allocation; }
    void set_should_collect_on_every_allocation(bool b) { m_should_collect_on_every_allocation = b; }

    void did_create_handle(Badge<HandleImpl>, HandleImpl&);
    void did_destroy_handle(Badge<HandleImpl>, HandleImpl&);

    void did_create_marked_value_list(Badge<MarkedValueList>, MarkedValueList&);
    void did_destroy_marked_value_list(Badge<MarkedValueList>, MarkedValueList&);

    void did_create_weak_container(Badge<WeakContainer>, WeakContainer&);
    void did_destroy_weak_container(Badge<WeakContainer>, WeakContainer&);

    void defer_gc(Badge<DeferGC>);
    void undefer_gc(Badge<DeferGC>);

    BlockAllocator& block_allocator() { return m_block_allocator; }

private:
    Cell* allocate_cell(size_t);

    void gather_roots(HashTable<Cell*>&);
    void gather_conservative_roots(HashTable<Cell*>&);
    void mark_live_cells(const HashTable<Cell*>& live_cells);
    void sweep_dead_cells(bool print_report, const Core::ElapsedTimer&);

    CellAllocator& allocator_for_size(size_t);

    template<typename Callback>
    void for_each_block(Callback callback)
    {
        for (auto& allocator : m_allocators) {
            if (allocator->for_each_block(callback) == IterationDecision::Break)
                return;
        }
    }

    size_t m_max_allocations_between_gc { 10000 };
    size_t m_allocations_since_last_gc { 0 };

    bool m_should_collect_on_every_allocation { false };

    VM& m_vm;

    Vector<NonnullOwnPtr<CellAllocator>> m_allocators;

    HandleImpl::List m_handles;

    MarkedValueList::List m_marked_value_lists;

    WeakContainer::List m_weak_containers;

    BlockAllocator m_block_allocator;

    size_t m_gc_deferrals { 0 };
    bool m_should_gc_when_deferral_ends { false };

    bool m_collecting_garbage { false };
};

}
