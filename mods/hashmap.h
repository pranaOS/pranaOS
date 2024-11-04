/**
 * @file hashmap.h
 * @author Krisna Pranav
 * @brief HashMap
 * @version 6.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/hashtable.h>
#include <mods/optional.h>
#include <mods/vector.h>
#include <initializer_list>

namespace Mods 
{
    /**
     * @tparam K 
     * @tparam V 
     * @tparam KeyTraits 
     * @tparam IsOrdered 
     */
    template<typename K, typename V, typename KeyTraits, bool IsOrdered>
    class HashMap 
    {
    private:
        struct Entry 
        {
            K key;
            V value;
        }; // struct Entry

        struct EntryTraits 
        {
            /**
             * @param entry 
             * @return unsigned 
             */
            static unsigned hash(const Entry& entry) 
            { 
                return KeyTraits::hash(entry.key); 
            }

            /**
             * @param a 
             * @param b 
             * @return true 
             * @return false 
             */
            static bool equals(const Entry& a, const Entry& b) 
            { 
                return KeyTraits::equals(a.key, b.key); 
            }
        }; // struct EntryTraits

    public:
        using KeyType = K;
        using ValueType = V;

        /**
         * @brief Construct a new Hash Map object
         * 
         */
        HashMap() = default;

        /**
         * @brief Construct a new Hash Map object
         * 
         * @param list 
         */
        HashMap(std::initializer_list<Entry> list)
        {
            ensure_capacity(list.size());
            for (auto& item : list)
                set(item.key, item.value);
        }

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool is_empty() const 
        {
            return m_table.is_empty();
        }

        [[nodiscard]] size_t size() const 
        { 
            return m_table.size(); 
        }

        [[nodiscard]] size_t capacity() const 
        { 
            return m_table.capacity(); 
        }

        void clear() 
        { 
            m_table.clear(); 
        }

        void clear_with_capacity() 
        { 
            m_table.clear_with_capacity(); 
        }

        /**
         * @param key 
         * @param value 
         * @return HashSetResult 
         */
        HashSetResult set(const K& key, const V& value) 
        { 
            return m_table.set({ key, value }); 
        }

        /**
         * @param key 
         * @param value 
         * @return HashSetResult 
         */
        HashSetResult set(const K& key, V&& value) 
        { 
            return m_table.set({ key, move(value) }); 
        }

        /**
         * @param key 
         * @param value 
         * @return HashSetResult 
         */
        HashSetResult set(K&& key, V&& value) 
        { 
            return m_table.set({ move(key), move(value) }); 
        }

        /**
         * @param key 
         * @param value 
         * @return ErrorOr<HashSetResult> 
         */
        ErrorOr<HashSetResult> try_set(const K& key, const V& value) 
        { 
            return m_table.try_set({ key, value }); 
        }

        /**
         * @param key 
         * @param value 
         * @return ErrorOr<HashSetResult> 
         */
        ErrorOr<HashSetResult> try_set(const K& key, V&& value) 
        { 
            return m_table.try_set({ key, move(value) }); 
        }

        /**
         * @param key 
         * @param value 
         * @return ErrorOr<HashSetResult> 
         */
        ErrorOr<HashSetResult> try_set(K&& key, V&& value) 
        { 
            return m_table.try_set({ move(key), move(value) }); 
        }

        /**
         * @param key 
         * @return true 
         * @return false 
         */
        bool remove(const K& key)
        {
            auto it = find(key);

            if (it != end()) {
                m_table.remove(it);
                return true;
            }

            return false;
        }

        /**
         * @tparam Key 
         */
        template<Concepts::HashCompatible<K> Key>
        requires(IsSame<KeyTraits, Traits<K>>) bool remove(Key const& key)
        {
            auto it = find(key);
            if (it != end()) {
                m_table.remove(it);
                return true;
            }
            return false;
        }

        /**
         * @tparam TUnaryPredicate 
         * @param predicate 
         * @return true 
         * @return false 
         */
        template<typename TUnaryPredicate>
        bool remove_all_matching(TUnaryPredicate predicate)
        {
            bool something_was_removed = false;
            for (auto it = begin(); it != end();) {
                if (predicate(it->key, it->value)) {
                    it = remove(it);
                    something_was_removed = true;
                } else {
                    ++it;
                }
            }
            return something_was_removed;
        }

        using HashTableType = HashTable<Entry, EntryTraits, IsOrdered>;
        using IteratorType = typename HashTableType::Iterator;
        using ConstIteratorType = typename HashTableType::ConstIterator;

        [[nodiscard]] IteratorType begin() 
        { 
            return m_table.begin(); 
        }
        
        [[nodiscard]] IteratorType end() 
        { 
            return m_table.end(); 
        }

        /**
         * @param key 
         * @return IteratorType 
         */
        [[nodiscard]] IteratorType find(const K& key)
        {
            return m_table.find(KeyTraits::hash(key), [&](auto& entry) { return KeyTraits::equals(key, entry.key); });
        }

        /**
         * @tparam TUnaryPredicate 
         * @param hash 
         * @param predicate 
         * @return IteratorType 
         */
        template<typename TUnaryPredicate>
        [[nodiscard]] IteratorType find(unsigned hash, TUnaryPredicate predicate)
        {
            return m_table.find(hash, predicate);
        }

        [[nodiscard]] ConstIteratorType begin() const 
        { 
            return m_table.begin(); 
        }

        [[nodiscard]] ConstIteratorType end() const 
        { 
            return m_table.end(); 
        }
        
        /**
         * @param key 
         * @return ConstIteratorType 
         */
        [[nodiscard]] ConstIteratorType find(const K& key) const
        {
            return m_table.find(KeyTraits::hash(key), [&](auto& entry) { return KeyTraits::equals(key, entry.key); });
        }

        /**
         * @tparam TUnaryPredicate 
         * @param hash 
         * @param predicate 
         * @return ConstIteratorType 
         */
        template<typename TUnaryPredicate>
        [[nodiscard]] ConstIteratorType find(unsigned hash, TUnaryPredicate predicate) const
        {
            return m_table.find(hash, predicate);
        }

        /**
         * @tparam Key 
         */
        template<Concepts::HashCompatible<K> Key>
        requires(IsSame<KeyTraits, Traits<K>>) [[nodiscard]] IteratorType find(Key const& key)
        {
            return m_table.find(Traits<Key>::hash(key), [&](auto& entry) { return Traits<K>::equals(key, entry.key); });
        }       

        /**
         * @tparam Key 
         */
        template<Concepts::HashCompatible<K> Key>
        requires(IsSame<KeyTraits, Traits<K>>) [[nodiscard]] ConstIteratorType find(Key const& key) const
        {
            return m_table.find(Traits<Key>::hash(key), [&](auto& entry) { return Traits<K>::equals(key, entry.key); });
        }

        /**
         * @param capacity 
         */
        void ensure_capacity(size_t capacity) 
        { 
            m_table.ensure_capacity(capacity); 
        }

        /**
         * @param capacity 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> try_ensure_capacity(size_t capacity) 
        { 
            return m_table.try_ensure_capacity(capacity); 
        }

        /**
         * @param key
         * 
         */
        Optional<typename Traits<V>::PeekType> get(const K& key) const requires(!IsPointer<typename Traits<V>::PeekType>)
        {
            auto it = find(key);

            if (it == end())
                return {};

            return (*it).value;
        }

        /**
         * @param key
         * 
         */
        Optional<typename Traits<V>::ConstPeekType> get(const K& key) const requires(IsPointer<typename Traits<V>::PeekType>)
        {
            auto it = find(key);
            if (it == end())
                return {};
            return (*it).value;
        }

        /**
         * @param key
         * 
         */
        Optional<typename Traits<V>::PeekType> get(const K& key) requires(!IsConst<typename Traits<V>::PeekType>)
        {
            auto it = find(key);
            if (it == end())
                return {};
            return (*it).value;
        }

        /**
         * @tparam Key 
         */
        template<Concepts::HashCompatible<K> Key>
        requires(IsSame<KeyTraits, Traits<K>>) Optional<typename Traits<V>::PeekType> get(const Key& key) const requires(!IsPointer<typename Traits<V>::PeekType>)
        {
            auto it = find(key);
            if (it == end())
                return {};
            return (*it).value;
        }

        /**
         * @brief Construct a new requires object
         * 
         * @tparam Key 
         */
        template<Concepts::HashCompatible<K> Key>
        requires(IsSame<KeyTraits, Traits<K>>) Optional<typename Traits<V>::ConstPeekType> get(const Key& key) const requires(IsPointer<typename Traits<V>::PeekType>)
        {
            auto it = find(key);
            if (it == end())
                return {};
            return (*it).value;
        }

        /**
         * @tparam Key 
         */
        template<Concepts::HashCompatible<K> Key>
        requires(IsSame<KeyTraits, Traits<K>>) Optional<typename Traits<V>::PeekType> get(const Key& key) requires(!IsConst<typename Traits<V>::PeekType>)
        {
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
        [[nodiscard]] bool contains(const K& key) const
        {
            return find(key) != end();
        }

        /**
         * @tparam Key 
         */
        template<Concepts::HashCompatible<K> Key>
        requires(IsSame<KeyTraits, Traits<K>>) [[nodiscard]] bool contains(Key const& value)
        {
            return find(value) != end();
        }

        /**
         * @param it 
         * @return IteratorType 
         */
        IteratorType remove(IteratorType it)
        {
            return m_table.remove(it);
        }

        /**
         * @param key 
         * @return V& 
         */
        V& ensure(const K& key)
        {
            auto it = find(key);

            if (it != end())
                return it->value;

            auto result = set(key, V());
            VERIFY(result == HashSetResult::InsertedNewEntry);
            return find(key)->value;
        }

        /**
         * @tparam Callback 
         * @param key 
         * @param initialization_callback 
         * @return V& 
         */
        template<typename Callback>
        V& ensure(K const& key, Callback initialization_callback)
        {
            auto it = find(key);

            if (it != end())
                return it->value;

            auto result = set(key, initialization_callback());
            VERIFY(result == HashSetResult::InsertedNewEntry);
            return find(key)->value;
        }

        /**
         * @return Vector<K> 
         */
        [[nodiscard]] Vector<K> keys() const
        {
            Vector<K> list;
            list.ensure_capacity(size());
            for (auto& it : *this)
                list.unchecked_append(it.key);
            return list;
        }

        [[nodiscard]] u32 hash() const
        {
            u32 hash = 0;
            for (auto& it : *this) {
                auto entry_hash = pair_int_hash(it.key.hash(), it.value.hash());
                hash = pair_int_hash(hash, entry_hash);
            }
            return hash;
        }

    private:
        HashTableType m_table;
    }; // class HashMap
} // namespace Mods

using Mods::HashMap;
using Mods::OrderedHashMap;
