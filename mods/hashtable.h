/**
 * @file hashtable.h
 * @author Krisna Pranav
 * @brief Hash Table
 * @version 6.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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
        InsertedNewEntry,
        ReplacedExistingEntry
    };

    template<typename HashTableType, typename T, typename BucketType>
    class HashTableIterator {
        friend HashTableType;

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
        T& operator*() { 
            return *m_bucket->slot(); 
        }

        /**
         * @return T* 
         */
        T* operator->() { 
            return m_bucket->slot(); 
        }

        void operator++() { 
            skip_to_next(); 
        }

    private:
        /// @brief skip_to_nxt bucket check whether it is used or not.
        void skip_to_next() {
            if (!m_bucket)
                return;
            do {
                ++m_bucket;
                if (m_bucket->used)
                    return;
            } while (!m_bucket->end);
            if (m_bucket->end)
                m_bucket = nullptr;
        }

        /**
         * @brief Construct a new Hash Table Iterator object
         * 
         * @param bucket 
         */
        explicit HashTableIterator(BucketType* bucket) : m_bucket(bucket)
        {
        }

        BucketType* m_bucket { nullptr };
    };

    template<typename T, typename TraitsForT>
    class HashTable {
        static constexpr size_t load_factor_in_percent = 60;

        struct Bucket {
            bool used;
            bool deleted;
            bool end;
            alignas(T) u8 storage[sizeof(T)];

            /**
             * @return T* 
             */
            T* slot() { 
                return reinterpret_cast<T*>(storage); 
            }

            /**
             * @return const T* 
             */
            const T* slot() const { 
                return reinterpret_cast<const T*>(storage); 
            }
        };

    public:
        HashTable() { }

        /**
         * @brief Construct a new Hash Table object
         * 
         * @param capacity 
         */
        HashTable(size_t capacity) { 
            rehash(capacity); 
        }

        /**
         * @brief Destroy the Hash Table object
         * 
         */
        ~HashTable() {
            if (!m_buckets)
                return;

            for (size_t i = 0; i < m_capacity; ++i) {
                if (m_buckets[i].used)
                    m_buckets[i].slot()->~T();
            }

            kfree(m_buckets);
        }

        /**
         * @brief Construct a new Hash Table object
         * 
         * @param other 
         */
        HashTable(const HashTable& other) {
            rehash(other.capacity());
            for (auto& it : other)
                set(it);
        }

        /**
         * @param other 
         * @return HashTable& 
         */
        HashTable& operator=(const HashTable& other) {
            HashTable temporary(other);
            swap(*this, temporary);
            return *this;
        }

        /**
         * @param other 
         */
        HashTable(HashTable&& other) noexcept
            : m_buckets(other.m_buckets)
            , m_size(other.m_size)
            , m_capacity(other.m_capacity)
            , m_deleted_count(other.m_deleted_count)
        {
            other.m_size = 0;
            other.m_capacity = 0;
            other.m_deleted_count = 0;
            other.m_buckets = nullptr;
        }

        /**
         * @param other 
         * @return HashTable& 
         */
        HashTable& operator=(HashTable&& other) noexcept {
            swap(*this, other);
            return *this;
        }

        /**
         * @param a 
         * @param b 
         */
        friend void swap(HashTable& a, HashTable& b) noexcept {
            swap(a.m_buckets, b.m_buckets);
            swap(a.m_size, b.m_size);
            swap(a.m_capacity, b.m_capacity);
            swap(a.m_deleted_count, b.m_deleted_count);
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const { 
            return !m_size; 
        }

        /**
         * @return size_t 
         */
        size_t size() const { 
            return m_size; 
        }
        
        /**
         * @return size_t 
         */
        size_t capacity() const { 
            return m_capacity; 
        }

        /**
         * @brief Set the from object
         * 
         * @tparam U 
         * @tparam N 
         */
        template<typename U, size_t N>
        void set_from(U (&from_array)[N]) {
            for (size_t i = 0; i < N; ++i) {
                set(from_array[i]);
            }
        }

        /**
         * @param capacity 
         */
        void ensure_capacity(size_t capacity) {
            ASSERT(capacity >= size());
            rehash(capacity * 2);
        }
        
        /**
         * @param value 
         * @return true 
         * @return false 
         */
        bool contains(const T& value) const {
            return find(value) != end();
        }

        using Iterator = HashTableIterator<HashTable, T, Bucket>;

        /**
         * @return Iterator 
         */
        Iterator begin() {
            for (size_t i = 0; i < m_capacity; ++i) {
                if (m_buckets[i].used)
                    return Iterator(&m_buckets[i]);
            }
            return end();
        }

        /**
         * @return Iterator 
         */
        Iterator end() {
            return Iterator(nullptr);
        }

        using ConstIterator = HashTableIterator<const HashTable, const T, const Bucket>;

        /**
         * @return ConstIterator 
         */
        ConstIterator begin() const {
            for (size_t i = 0; i < m_capacity; ++i) {
                if (m_buckets[i].used)
                    return ConstIterator(&m_buckets[i]);
            }
            return end();
        }

        /**
         * @return ConstIterator 
         */
        ConstIterator end() const {
            return ConstIterator(nullptr);
        }

        /**
         * @brief clear
         * 
         */
        void clear() {
            *this = HashTable();
        }

        /**
         * @param value 
         * @return HashSetResult 
         */
        HashSetResult set(T&& value) {
            auto& bucket = lookup_for_writing(value);
            if (bucket.used) {
                (*bucket.slot()) = move(value);
                return HashSetResult::ReplacedExistingEntry;
            }

            new (bucket.slot()) T(move(value));
            bucket.used = true;
            if (bucket.deleted) {
                bucket.deleted = false;
                --m_deleted_count;
            }
            ++m_size;
            return HashSetResult::InsertedNewEntry;
        }

        /**
         * @param value 
         * @return HashSetResult 
         */
        HashSetResult set(const T& value) {
            return set(T(value));
        }

        /**
         * @tparam Finder 
         * @param hash 
         * @param finder 
         * @return Iterator 
         */
        template<typename Finder>
        Iterator find(unsigned hash, Finder finder) {
            return Iterator(lookup_with_hash(hash, move(finder)));
        }

        /**
         * @param value 
         * @return Iterator 
         */
        Iterator find(const T& value) {
            return find(TraitsForT::hash(value), [&](auto& other) { return TraitsForT::equals(value, other); });
        }

        /**
         * @tparam Finder 
         * @param hash 
         * @param finder 
         * @return ConstIterator 
         */
        template<typename Finder>
        ConstIterator find(unsigned hash, Finder finder) const {
            return ConstIterator(lookup_with_hash(hash, move(finder)));
        }

        /**
         * @param value 
         * @return ConstIterator 
         */
        ConstIterator find(const T& value) const {
            return find(TraitsForT::hash(value), [&](auto& other) { return TraitsForT::equals(value, other); });
        }

        /**
         * @param value 
         * @return true 
         * @return false 
         */
        bool remove(const T& value) {
            auto it = find(value);
            if (it != end()) {
                remove(it);
                return true;
            }
            return false;
        }

        /**
         * @param iterator 
         */
        void remove(Iterator iterator) {
            ASSERT(iterator.m_bucket);
            auto& bucket = *iterator.m_bucket;
            ASSERT(bucket.used);
            ASSERT(!bucket.end);
            ASSERT(!bucket.deleted);
            bucket.slot()->~T();
            bucket.used = false;
            bucket.deleted = true;
            --m_size;
            ++m_deleted_count;
        }

    private:    
        /**
         * @param value 
         */
        void insert_during_rehash(T&& value) {
            auto& bucket = lookup_for_writing(value);
            new (bucket.slot()) T(move(value));
            bucket.used = true;
        }

        /**
         * @param new_capacity 
         */
        void rehash(size_t new_capacity) {
            new_capacity = max(new_capacity, static_cast<size_t>(4));

            auto* old_buckets = m_buckets;
            auto old_capacity = m_capacity;

            m_buckets = (Bucket*)kmalloc(sizeof(Bucket) * (new_capacity + 1));
            __builtin_memset(m_buckets, 0, sizeof(Bucket) * (new_capacity + 1));
            m_capacity = new_capacity;
            m_deleted_count = 0;

            m_buckets[m_capacity].end = true;

            if (!old_buckets)
                return;

            for (size_t i = 0; i < old_capacity; ++i) {
                auto& old_bucket = old_buckets[i];
                if (old_bucket.used) {
                    insert_during_rehash(move(*old_bucket.slot()));
                    old_bucket.slot()->~T();
                }
            }

            kfree(old_buckets);
        }
        
        /**
         * @tparam Finder 
         * @param hash 
         * @param finder 
         * @param usable_bucket_for_writing 
         * @return Bucket* 
         */
        template<typename Finder>
        Bucket* lookup_with_hash(unsigned hash, Finder finder, Bucket** usable_bucket_for_writing = nullptr) const {
            if (is_empty())
                return nullptr;
            size_t bucket_index = hash % m_capacity;
            for (;;) {
                auto& bucket = m_buckets[bucket_index];

                if (usable_bucket_for_writing && !*usable_bucket_for_writing && !bucket.used) {
                    *usable_bucket_for_writing = &bucket;
                }

                if (bucket.used && finder(*bucket.slot()))
                    return &bucket;

                if (!bucket.used && !bucket.deleted)
                    return nullptr;

                hash = double_hash(hash);
                bucket_index = hash % m_capacity;
            }
        }

        /**
         * @param value 
         * @return const Bucket* 
         */
        const Bucket* lookup_for_reading(const T& value) const {
            return lookup_with_hash(TraitsForT::hash(value), [&value](auto& entry) { return TraitsForT::equals(entry, value); });
        }

        /**
         * @param value 
         * @return Bucket& 
         */
        Bucket& lookup_for_writing(const T& value) {
            auto hash = TraitsForT::hash(value);
            Bucket* usable_bucket_for_writing = nullptr;
            if (auto* bucket_for_reading = lookup_with_hash(
                    hash,
                    [&value](auto& entry) { return TraitsForT::equals(entry, value); },
                    &usable_bucket_for_writing)) {
                return *const_cast<Bucket*>(bucket_for_reading);
            }

            if (should_grow())
                rehash(capacity() * 2);
            else if (usable_bucket_for_writing)
                return *usable_bucket_for_writing;

            size_t bucket_index = hash % m_capacity;

            for (;;) {
                auto& bucket = m_buckets[bucket_index];
                if (!bucket.used)
                    return bucket;
                hash = double_hash(hash);
                bucket_index = hash % m_capacity;
            }
        }

        /**
         * @return size_t 
         */
        size_t used_bucket_count() const { 
            return m_size + m_deleted_count; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool should_grow() const { 
            return ((used_bucket_count() + 1) * 100) >= (m_capacity * load_factor_in_percent); 
        }

        Bucket* m_buckets { nullptr };
        size_t m_size { 0 };
        size_t m_capacity { 0 };
        size_t m_deleted_count { 0 };
    };

}

using Mods::HashTable;
