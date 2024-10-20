/**
 * @file virtualrangeallocator.h
 * @author Krisna Pranav
 * @brief Virtual Range Allocator
 * @version 6.0
 * @date 2024-10-20
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/locking/spinlock.h>
#include <kernel/memory/virtualrange.h>
#include <mods/rbtree.h>
#include <mods/traits.h>


namespace Kernel::Memory 
{

    class VirtualRangeAllocator 
    {
    public:
        /**
         * @brief Construct a new Virtual Range Allocator object
         * 
         */
        VirtualRangeAllocator();

        /**
         * @brief Destroy the Virtual Range Allocator object
         * 
         */
        ~VirtualRangeAllocator() = default;

        /**
         * @return ErrorOr<void> 
         */
        ErrorOr<void> initialize_with_range(VirtualAddress, size_t);

        /**
         * @return ErrorOr<void> 
         */
        ErrorOr<void> initialize_from_parent(VirtualRangeAllocator const&);

        /**
         * @param alignment 
         * @return ErrorOr<VirtualRange> 
         */
        ErrorOr<VirtualRange> try_allocate_anywhere(size_t, size_t alignment = PAGE_SIZE);

        /**
         * @return ErrorOr<VirtualRange> 
         */
        ErrorOr<VirtualRange> try_allocate_specific(VirtualAddress, size_t);

        /**
         * @param alignment 
         * @return ErrorOr<VirtualRange> 
         */
        ErrorOr<VirtualRange> try_allocate_randomized(size_t, size_t alignment);
        
        void deallocate(VirtualRange const&);

        void dump() const;

        /**
         * @param range 
         * @return true 
         * @return false 
         */
        bool contains(VirtualRange const& range) const 
        { 
            return m_total_range.contains(range); 
        }

    private:
        /**
         * @param from 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> carve_from_region(VirtualRange const& from, VirtualRange const&);

        RedBlackTree<FlatPtr, VirtualRange> m_available_ranges;
        VirtualRange m_total_range;
        mutable Spinlock m_lock;
    }; // class VirtualRangeAllocator

} // namespace Kernel::Memory

namespace Mods
{
    template<>
    struct Traits<Kernel::Memory::VirtualRange> : public GenericTraits<Kernel::Memory::VirtualRange> 
    {
        static constexpr bool is_trivial() 
        { 
            return true; 
        }
    };
} // namespace Mods