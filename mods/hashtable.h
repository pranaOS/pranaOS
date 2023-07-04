/**
 * @file hashtable.h
 * @author Krisna Pranav
 * @brief Hash Table
 * @version 1.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/logstream.h>
#include <mods/stdlibextra.h>
#include <mods/types.h>
#include <mods/kmalloc.h>
#include <mods/hashfunctions.h>

namespace Mods {
    enum class HashSetResult {
        InserrtedNewEntry,
        ReplacedExistingEntry
    };

    template<typename HashTableType, typename T, typename BucketType>
    class HashTableIterator {
    public:
        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const HashTableIterator& other) const {
            return m_bucket == other.m_bucket;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const HashTableIterator& other) const {
            return m_bucket != other.m_bucket;
        }

        /**
         * @return T& 
         */
        T& operator*() const {
            return *m_bucket->slot();
        }

        /**
         * @return T& 
         */
        T& operator->() {
            return m_bucket->slot();
        }
        
        void operator++() {
            skip_to_next();
        }


    private:
        void skip_to_next() {
            if (!m_bucket)
                return;
            
            do {
                ++m_bucket;
                if (m_bucket->used)
                    return
            } while (!m_bucket->end);
            if (m_bucket->end)
                m_bucket = nullptr;
        }

        explicit HashTableIterator(BucketType* bucket)
            : m_bucket(bucket) 
        {}

        BucketType* m_bucket { nullptr };
    };
}