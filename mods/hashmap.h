/**
 * @file hashmap.h
 * @author Krisna Pranav
 * @brief HashMap
 * @version 1.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "hashtable.h"
#include <mods/optional.h>
#include <mods/vector.h>
#include <mods/stdlibextra.h>
#include <mods/hashtable.h>

namespace Mods {
    template<typename K, typename V, typename KeyTraits>
    class HashMap {

    public:
        HashMap() {}

        bool is_empty() const {
            return m_table.is_empty();
        }
    
    private:
        HashTableType m_table;

    private:

        struct Entry {
            K key;
            V value;
        };

        struct EntryTraits {

            /**
             * @param entry 
             * @return unsigned 
             */
            static unsigned hash(const Entry& entry) { 
                return KeyTraits::hash(entry.key); 
            }

            /**
             * @param a 
             * @param b 
             * @return true 
             * @return false 
             */
            static bool equals(const Entry& a, const Entry& b) { 
                return KeyTraits::equals(a.key, b.key); 
            }
        };
    };
}