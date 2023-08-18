/**
 * @file rangeallocator.h
 * @author Krisna Pranav
 * @brief rangeallocator
 * @version 6.0
 * @date 2023-08-07
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/spinlock.h>
#include <kernel/virtual_address.h>
#include <mods/string.h>
#include <mods/traits.h>
#include <mods/vector.h>

namespace Kernel 
{

    class Range 
    {
        friend class RangeAllocator;

    public:
        Range() { }

        /**
         * @param base 
         * @param size 
         */
        Range(VirtualAddress base, size_t size)
            : m_base(base)
            , m_size(size)
        {
        }

        /**
         * @return VirtualAddress 
         */
        VirtualAddress base() const 
        { 
            return m_base; 
        }

        /**
         * @return size_t 
         */
        size_t size() const 
        {
            return m_size; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_valid() const 
        { 
            return !m_base.is_null(); 
        }

        /**
         * @param vaddr 
         * @return true 
         * @return false 
         */
        bool contains(VirtualAddress vaddr) const 
        { 
            return vaddr >= base() && vaddr < end(); 
        }

        /**
         * @return VirtualAddress 
         */
        VirtualAddress end() const 
        { 
            return m_base.offset(m_size); 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const Range& other) const
        {
            return m_base == other.m_base && m_size == other.m_size;
        }

        /**
         * @param base 
         * @param size 
         * @return true 
         * @return false 
         */
        bool contains(VirtualAddress base, size_t size) const
        {
            if (base.offset(size) < base)
                return false;

            return base >= m_base && base.offset(size) <= end();
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool contains(const Range& other) const
        {
            return contains(other.base(), other.size());
        }

        /**
         * @return Vector<Range, 2> 
         */
        Vector<Range, 2> carve(const Range&);

    private:
        VirtualAddress m_base;
        size_t m_size { 0 };
    }; // class Range

    class RangeAllocator 
    {
    public:
        RangeAllocator();
        ~RangeAllocator();

        /// @brief: initialize_with_range
        void initialize_with_range(VirtualAddress, size_t);
        void initialize_from_parent(const RangeAllocator&);

        /**
         * @param alignment 
         * @return Range 
         */
        Range allocate_anywhere(size_t, size_t alignment = PAGE_SIZE);

        /**
         * @return Range 
         */
        Range allocate_specific(VirtualAddress, size_t);

        /// @brief: deallocate
        void deallocate(Range);

        void dump() const;

        /**
         * @param range 
         * @return true 
         * @return false 
         */
        bool contains(const Range& range) const
        {
            ScopedSpinLock lock(m_lock);
            return m_total_range.contains(range);
        }

    private:
        void carve_at_index(int, const Range&);

        Vector<Range> m_available_ranges;

        Range m_total_range;
        mutable SpinLock<u8> m_lock;
    }; // class RangeAllocator

    /**
     * @param stream 
     * @param value 
     * @return const LogStream& 
     */
    inline const LogStream& operator<<(const LogStream& stream, const Range& value)
    {
        return stream << String::format("Range(%x-%x)", value.base().get(), value.end().get() - 1);
    }

} // namespace Kernel

namespace Mods 
{
    template<>
    struct Traits<Kernel::Range> : public GenericTraits<Kernel::Range> 
    {
        /**
         * @return true 
         * @return false 
         */
        static constexpr bool is_trivial() 
        { 
            return true; 
        }
    };
} // namespace Mods