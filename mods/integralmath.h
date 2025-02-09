/**
 * @file integralmath.h
 * @author Krisna Pranav
 * @brief Integral Math
 * @version 6.0
 * @date 2023-07-20
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once 

#include <mods/forward.h>
#include <mods/hashtable.h>
#include <mods/random.h>

namespace Mods
{
    class IDAllocator
    {
    public:
        /**
         * @brief Construct a new IDAllocator object
         * 
         */
        IDAllocator() = default;

        /**
         * @brief Destroy the IDAllocator object
         * 
         */
        ~IDAllocator() = default;

        void allocate() {}

        /**
         * @param id 
         */
        void deallocate(int id)
        {
            m_allocated_ids.remove(id);
        }
    private:
        HashTable<int> m_allocated_ids;
    }; // class IDAllocator
} // namespace Mods

using Mods::IDAllocator;