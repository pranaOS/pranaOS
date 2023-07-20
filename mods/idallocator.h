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

    class IDAllocator {

    public:

        /**
         * @brief Construct a new IDAllocator object
         * 
         */
        IDAllocator() { }

        /**
         * @brief Destroy the IDAllocator object
         * 
         */
        ~IDAllocator() { }

        /**
         * @return int 
         */
        int allocate()
        {
            int r = rand();
            for (int i = 0; i < 100000; ++i) {
                int allocated_id = r + i;
                if (allocated_id == 0)
                    ++allocated_id;
                if (!m_allocated_ids.contains(allocated_id)) {
                    m_allocated_ids.set(allocated_id);
                    return allocated_id;
                }
            }
            ASSERT_NOT_REACHED();
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
    };
}

using Mods::IDAllocator;
