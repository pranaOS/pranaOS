/**
 * @file rangeallocator.h
 * @author Krisna Pranav
 * @brief rangeallocator
 * @version 1.0
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
        Range() {}

        Range(VirtualAddress base, size_t size)
            : m_base(base)
            , m_size(size)
        {}

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

    private:
        VirtualAddress m_base;
        size_t m_size { 0 };
    }; // class Range
} // namespace Kernel