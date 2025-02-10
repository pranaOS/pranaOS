/**
 * @file idallocator.h
 * @author Krisna Pranav
 * @brief IDAllocator
 * @version 6.0
 * @date 2023-07-20
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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

        int allocate()
        {
            VERIFY(m_allocated_ids.size() < (INT32_MAX - 2));
            int id = 0;
            for(;;)
            {
                id = static_cast<int>(get_random_uniform(NumericLimits<int>::max()));
                if(id == 0)
                    continue;
                if(m_allocated_ids.set(id) == Mods::HashSetResult::InsertedNewEntry)
                    break;
            }
            return id;
        }

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
