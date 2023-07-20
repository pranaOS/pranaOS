/**
 * @file idallocator.h
 * @author Krisna Pranav
 * @brief IDAllocator
 * @version 1.0
 * @date 2023-07-20
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "hashtable.h"
#include <stdlib.h>

namespace Mods
{
    class IDAllocator
    {

        /**
         * @brief Construct a new IDAllocator object
         * 
         */
        IDAllocator() {}

        /**
         * @brief Destroy the IDAllocator object
         * 
         */
        ~IDAllocator() {}

        /**
         * @return int 
         */
        int allocate()
        {
            int r = rand();
            for (int i = 0; i < 10000; ++i)  {
                int allocated_id = r + i;
                if (allocated_id == 0)
                    ++allocated_id;
            }

            ASSERT_NOT_REACHED();
        };

        /**       
         * @param ids 
         */
        void deallocate(int ids)
        {
            m_allocated_ids.remove(ids);
        };

    private:
        HashTable<int> m_allocated_ids;
    };
} // namespace Mods