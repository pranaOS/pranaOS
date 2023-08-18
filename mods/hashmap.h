/**
 * @file hashmap.h
 * @author Krisna Pranav
 * @brief HashMap
 * @version 6.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "hashtable.h"
#include "optional.h"
#include "stdlibextra.h"
#include "vector.h"

namespace Mods {

    template<typename K, typename V, typename KeyTraits>
    class HashMap {
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

    public:
        HashMap() { }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const { 
            return m_table.is_empty(); 
        }

        /**
         * @return size_t 
         */
        size_t size() const { 
            return m_table.size(); 
        }

        /**
         * @return size_t 
         */
        size_t capacity() const { 
            return m_table.capacity(); 
        }

        /**
         * @brief clear
         * 
         */
        void clear() { 
            m_table.clear(); 
        }

        /**
         * @param key 
         * @param value 
         * @return HashSetResult 
         */
        HashSetResult set(const K& key, const V& value) { 
            return m_table.set({ key, value }); 
        }

        /**
         * @param key 
         * @param value 
         * @return HashSetResult 
         */
        HashSetResult set(const K& key, V&& value) { 
            return m_table.set({ key, move(value) }); 
        }

        /**
         * @param key 
         * @return true 
         * @return false 
         */
        bool remove(const K& key) {
            auto it = find(key);
            if (it != end()) {
                m_table.remove(it);
                return true;
            }
            return false;
        }

        /**
         * @return *void
         * 
         */
        void remove_one_randomly() { 
            m_table.remove(m_table.begin()); 
        }

        using HashTableType = HashTable<Entry, EntryTraits>;
        using IteratorType = typename HashTableType::Iterator;
        using ConstIteratorType = typename HashTableType::ConstIterator;

        /**
         * @return IteratorType 
         */
        IteratorType begin() { 
            return m_table.begin(); 
        }

        /**
         * @return IteratorType 
         */
        IteratorType end() { 
            return m_table.end(); 
        }

        /**
         * @param key 
         * @return IteratorType 
         */
        IteratorType find(const K& key) {
            return m_table.find(KeyTraits::hash(key), [&](auto& entry) { return KeyTraits::equals(key, entry.key); });
        }

        /** 
         * @tparam Finder 
         * @param hash 
         * @param finder 
         * @return IteratorType 
         */
        template<typename Finder>
        IteratorType find(unsigned hash, Finder finder) {
            return m_table.find(hash, finder);
        }

        /**
         * @return ConstIteratorType 
         */
        ConstIteratorType begin() const { 
            return m_table.begin(); 
        }

        /**
         * @return ConstIteratorType 
         */
        ConstIteratorType end() const { 
            return m_table.end(); 
        }

        /** 
         * @param key 
         * @return ConstIteratorType 
         */
        ConstIteratorType find(const K& key) const {
            return m_table.find(KeyTraits::hash(key), [&](auto& entry) { return KeyTraits::equals(key, entry.key); });
        }

        /** 
         * @tparam Finder 
         * @param hash 
         * @param finder 
         * @return ConstIteratorType 
         */
        template<typename Finder>
        ConstIteratorType find(unsigned hash, Finder finder) const {
            return m_table.find(hash, finder);
        }

        /** 
         * @param capacity 
         */
        void ensure_capacity(size_t capacity) { 
            m_table.ensure_capacity(capacity); 
        }

        /**
         * @param key 
         * @return Optional<typename Traits<V>::PeekType> 
         */
        Optional<typename Traits<V>::PeekType> get(const K& key) const {
            auto it = find(key);
            if (it == end())
                return {};
            return (*it).value;
        }

        /**
         * @param key 
         * @return true 
         * @return false 
         */
        bool contains(const K& key) const {
            return find(key) != end();
        }

        /**
         * @param it 
         */
        void remove(IteratorType it) {
            m_table.remove(it);
        }

        /**
         * @param key 
         * @return V& 
         */
        V& ensure(const K& key) {
            auto it = find(key);
            if (it == end())
                set(key, V());
            return find(key)->value;
        }

        /**
         * @return Vector<K> 
         */
        Vector<K> keys() const {
            Vector<K> list;
            list.ensure_capacity(size());
            for (auto& it : *this)
                list.unchecked_append(it.key);
            return list;
        }

    private:
        HashTableType m_table;
    };

}

using Mods::HashMap;
