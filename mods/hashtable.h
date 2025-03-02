/**
 * @file hashtable.h
 * @author Krisna Pranav
 * @brief Hash Table
 * @version 6.0
 * @date 2023-07-04
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/concept.h>
#include <mods/error.h>
#include <mods/forward.h>
#include <mods/hashfunctions.h>
#include <mods/stdlibextra.h>
#include <mods/traits.h>
#include <mods/types.h>
#include <mods/kmalloc.h>

namespace Mods
{
    enum class HashSetResult
    {
        InsertedNewEntry,
        ReplacedExistingEntry,
        KeptExistingEntry
    }; // enum class HashSetResult

    enum class HashSetExistingEntryBehavior
    {
        Keep,
        Replace
    }; // enum class HashSetExistingEntryBehavior

    enum class BucketState : u8
    {
        Free = 0x00,
        Used = 0x10,
        Deleted = 0x01,
        Rehashed = 0x12,
        End = 0xFF,
    }; // enum class BucketState : u8

    /**
     * @param state 
     * @return true 
     * @return false 
     */
    constexpr bool is_used_bucket(BucketState state)
    {
        return (static_cast<u8>(state) & 0xf0) == 0x10;
    }

    /**
     * @param state 
     * @return true 
     * @return false 
     */
    constexpr bool is_free_bucket(BucketState state)
    {
        return (static_cast<u8>(state) & 0xf0) == 0x00;
    }

    /**
     * @tparam HashTableType 
     * @tparam T 
     * @tparam BucketType 
     */
    template <typename HashTableType, typename T, typename BucketType>
    class HashTableIterator
    {
        friend HashTableType;

    public:
        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(HashTableIterator const& other) const
        {
            return m_bucket == other.m_bucket;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(HashTableIterator const& other) const
        {
            return m_bucket != other.m_bucket;
        }

        /**
         * @return T& 
         */
        T& operator*()
        {
            return *m_bucket->slot();
        }

        /**
         * @return T* 
         */
        T* operator->()
        {
            return m_bucket->slot();
        }
        
        void operator++()
        {
            skip_to_next();
        }

    private:
        void skip_to_next()
        {
            if(!m_bucket)
                return;
            do
            {
                ++m_bucket;
                if(m_bucket->state == BucketState::Used)
                    return;
            } while(m_bucket->state != BucketState::End);
            if(m_bucket->state == BucketState::End)
                m_bucket = nullptr;
        }

        /**
         * @brief Construct a new Hash Table Iterator object
         * 
         * @param bucket 
         */
        explicit HashTableIterator(BucketType* bucket)
            : m_bucket(bucket)
        {
        }

        BucketType* m_bucket{nullptr};
    }; // class HashTableIterator

    /**
     * @tparam OrderedHashTableType 
     * @tparam T 
     * @tparam BucketType 
     */
    template <typename OrderedHashTableType, typename T, typename BucketType>
    class OrderedHashTableIterator
    {
        friend OrderedHashTableType;

    public:
        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(OrderedHashTableIterator const& other) const
        {
            return m_bucket == other.m_bucket;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(OrderedHashTableIterator const& other) const
        {
            return m_bucket != other.m_bucket;
        }

        /**
         * @return T& 
         */
        T& operator*()
        {
            return *m_bucket->slot();
        }

        /**
         * @return T* 
         */
        T* operator->()
        {
            return m_bucket->slot();
        }

        void operator++()
        {
            m_bucket = m_bucket->next;
        }

        void operator--()
        {
            m_bucket = m_bucket->previous;
        }

    private:
        /**
         * @brief Construct a new Ordered Hash Table Iterator object
         * 
         * @param bucket 
         */
        explicit OrderedHashTableIterator(BucketType* bucket)
            : m_bucket(bucket)
        {
        }

        BucketType* m_bucket{nullptr};
    }; // class OrderedHashTableIterator

    /**
     * @tparam T 
     * @tparam TraitsForT 
     * @tparam IsOrdered 
     */
    template <typename T, typename TraitsForT, bool IsOrdered>
    class HashTable
    {
        static constexpr size_t load_factor_in_percent = 60;

        struct Bucket
        {
            BucketState state;
            alignas(T) u8 storage[sizeof(T)];

            /**
             * @return T* 
             */
            T* slot()
            {
                return reinterpret_cast<T*>(storage);
            }

            /**
             * @return const T* 
             */
            const T* slot() const
            {
                return reinterpret_cast<const T*>(storage);
            }
        }; // struct Bucket

        struct OrderedBucket
        {
            OrderedBucket* previous;
            OrderedBucket* next;
            BucketState state;
            alignas(T) u8 storage[sizeof(T)];

            /**
             * @return T* 
             */
            T* slot()
            {
                return reinterpret_cast<T*>(storage);
            }

            /**
             * @return const T* 
             */
            const T* slot() const
            {
                return reinterpret_cast<const T*>(storage);
            }
        }; // struct OrderedBucket

        using BucketType = Conditional<IsOrdered, OrderedBucket, Bucket>;

        struct CollectionData
        {
        }; // struct CollectionData

        struct OrderedCollectionData
        {
            BucketType* head{nullptr};
            BucketType* tail{nullptr};
        }; // struct OrderedCollectionData

        using CollectionDataType = Conditional<IsOrdered, OrderedCollectionData, CollectionData>;

    public:
        /**
         * @brief Construct a new Hash Table object
         * 
         */
        HashTable() = default;

        /**
         * @brief Construct a new Hash Table object
         * 
         * @param capacity 
         */
        explicit HashTable(size_t capacity)
        {
            rehash(capacity);
        }   

        /**
         * @brief Destroy the Hash Table object
         * 
         */
        ~HashTable()
        {
            if(!m_buckets)
                return;

            for(size_t i = 0; i < m_capacity; ++i)
            {
                if(is_used_bucket(m_buckets[i].state))
                    m_buckets[i].slot()->~T();
            }

            kfree_sized(m_buckets, size_in_bytes(m_capacity));
        }

        /**
         * @brief Construct a new Hash Table object
         * 
         * @param other 
         */
        HashTable(HashTable const& other)
        {
            rehash(other.capacity());
            for(auto& it : other)
                set(it);
        }

        /**
         * @param other 
         * @return HashTable& 
         */
        HashTable& operator=(HashTable const& other)
        {
            HashTable temporary(other);
            swap(*this, temporary);
            return *this;
        }

        /**
         * @brief Construct a new Hash Table object
         * 
         * @param other 
         */
        HashTable(HashTable&& other) noexcept
            : m_buckets(other.m_buckets), m_collection_data(other.m_collection_data), m_size(other.m_size), m_capacity(other.m_capacity), m_deleted_count(other.m_deleted_count)
        {
            other.m_size = 0;
            other.m_capacity = 0;
            other.m_deleted_count = 0;
            other.m_buckets = nullptr;
            if constexpr(IsOrdered)
                other.m_collection_data = {nullptr, nullptr};
        }

        /**
         * @param other 
         * @return HashTable& 
         */
        HashTable& operator=(HashTable&& other) noexcept
        {
            HashTable temporary{move(other)};
            swap(*this, temporary);
            return *this;
        }

        /**
         * @param a 
         * @param b 
         */
        friend void swap(HashTable& a, HashTable& b) noexcept
        {
            swap(a.m_buckets, b.m_buckets);
            swap(a.m_size, b.m_size);
            swap(a.m_capacity, b.m_capacity);
            swap(a.m_deleted_count, b.m_deleted_count);

            if constexpr(IsOrdered)
                swap(a.m_collection_data, b.m_collection_data);
        }

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool is_empty() const
        {
            return m_size == 0;
        }

        /**
         * @return size_t 
         */
        [[nodiscard]] size_t size() const
        {
            return m_size;
        }

        /**
         * @return size_t 
         */
        [[nodiscard]] size_t capacity() const
        {
            return m_capacity;
        }

        /**
         * @tparam U 
         * @tparam N 
         * @return ErrorOr<void> 
         */
        template <typename U, size_t N>
        ErrorOr<void> try_set_from(U (&from_array)[N])
        {
            for(size_t i = 0; i < N; ++i)
                TRY(try_set(from_array[i]));
            return {};
        }

        /**
         * @brief Set the from object
         * 
         * @tparam U 
         * @tparam N 
         */
        template <typename U, size_t N>
        void set_from(U (&from_array)[N])
        {
            MUST(try_set_from(from_array));
        }

        /**
         * @param capacity 
         */
        void ensure_capacity(size_t capacity)
        {
            VERIFY(capacity >= size());
            rehash(capacity * 2);
        }

        /**
         * @param capacity 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> try_ensure_capacity(size_t capacity)
        {
            VERIFY(capacity >= size());
            return try_rehash(capacity * 2);
        }

        /**
         * @param value 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool contains(T const& value) const
        {
            return find(value) != end();
        }

        /**
         * @tparam K 
         */
        template <Concepts::HashCompatible<T> K>
            requires(IsSame<TraitsForT, Traits<T>>)
        [[nodiscard]] bool contains(K const& value) const
        {
            return find(value) != end();
        }

        using Iterator = Conditional<IsOrdered,
                                    OrderedHashTableIterator<HashTable, T, BucketType>,
                                    HashTableIterator<HashTable, T, BucketType>>;

        /**
         * @return Iterator 
         */
        [[nodiscard]] Iterator begin()
        {
            if constexpr(IsOrdered)
                return Iterator(m_collection_data.head);

            for(size_t i = 0; i < m_capacity; ++i)
            {
                if(is_used_bucket(m_buckets[i].state))
                    return Iterator(&m_buckets[i]);
            }
            return end();
        }

        /**
         * @return Iterator 
         */
        [[nodiscard]] Iterator end()
        {
            return Iterator(nullptr);
        }

        using ConstIterator = Conditional<IsOrdered,
                                        OrderedHashTableIterator<const HashTable, const T, const BucketType>,
                                        HashTableIterator<const HashTable, const T, const BucketType>>;

        [[nodiscard]] ConstIterator begin() const
        {
            if constexpr(IsOrdered)
                return ConstIterator(m_collection_data.head);

            for(size_t i = 0; i < m_capacity; ++i)
            {
                if(is_used_bucket(m_buckets[i].state))
                    return ConstIterator(&m_buckets[i]);
            }
            return end();
        }

        [[nodiscard]] ConstIterator end() const
        {
            return ConstIterator(nullptr);
        }

        void clear()
        {
            *this = HashTable();
        }

        void clear_with_capacity()
        {
            if constexpr(!Detail::IsTriviallyDestructible<T>)
            {
                for(auto* bucket : *this)
                    bucket->~T();
            }
            __builtin_memset(m_buckets, 0, size_in_bytes(capacity()));
            m_size = 0;
            m_deleted_count = 0;

            if constexpr(IsOrdered)
                m_collection_data = {nullptr, nullptr};
            else
                m_buckets[m_capacity].state = BucketState::End;
        }

        /**
         * @tparam U 
         * @param value 
         * @param existing_entry_behavior 
         * @return ErrorOr<HashSetResult> 
         */
        template <typename U = T>
        ErrorOr<HashSetResult> try_set(U&& value, HashSetExistingEntryBehavior existing_entry_behavior = HashSetExistingEntryBehavior::Replace)
        {
            auto* bucket = TRY(try_lookup_for_writing(value));
            if(is_used_bucket(bucket->state))
            {
                if(existing_entry_behavior == HashSetExistingEntryBehavior::Keep)
                    return HashSetResult::KeptExistingEntry;
                (*bucket->slot()) = forward<U>(value);
                return HashSetResult::ReplacedExistingEntry;
            }

            new(bucket->slot()) T(forward<U>(value));
            if(bucket->state == BucketState::Deleted)
                --m_deleted_count;
            bucket->state = BucketState::Used;

            if constexpr(IsOrdered)
            {
                if(!m_collection_data.head) [[unlikely]]
                {
                    m_collection_data.head = bucket;
                }
                else
                {
                    bucket->previous = m_collection_data.tail;
                    m_collection_data.tail->next = bucket;
                }
                m_collection_data.tail = bucket;
            }

            ++m_size;
            return HashSetResult::InsertedNewEntry;
        }

        /**
         * @tparam U 
         * @param value 
         * @param existing_entry_behaviour 
         * @return HashSetResult 
         */
        template <typename U = T>
        HashSetResult set(U&& value, HashSetExistingEntryBehavior existing_entry_behaviour = HashSetExistingEntryBehavior::Replace)
        {
            return MUST(try_set(forward<U>(value), existing_entry_behaviour));
        }

        /**
         * @tparam TUnaryPredicate 
         * @param hash 
         * @param predicate 
         * @return Iterator 
         */
        template <typename TUnaryPredicate>
        [[nodiscard]] Iterator find(unsigned hash, TUnaryPredicate predicate)
        {
            return Iterator(lookup_with_hash(hash, move(predicate)));
        }

        /**
         * @param value 
         * @return Iterator 
         */
        [[nodiscard]] Iterator find(T const& value)
        {
            return find(TraitsForT::hash(value), [&](auto& other)
                        { return TraitsForT::equals(value, other); });
        }   

        /**
         * @tparam TUnaryPredicate 
         * @param hash 
         * @param predicate 
         * @return ConstIterator 
         */
        template <typename TUnaryPredicate>
        [[nodiscard]] ConstIterator find(unsigned hash, TUnaryPredicate predicate) const
        {
            return ConstIterator(lookup_with_hash(hash, move(predicate)));
        }

        /**
         * @param value 
         * @return ConstIterator 
         */
        [[nodiscard]] ConstIterator find(T const& value) const
        {
            return find(TraitsForT::hash(value), [&](auto& other)
                        { return TraitsForT::equals(value, other); });
        }
        
        /**
         * @tparam K 
         */
        template <Concepts::HashCompatible<T> K>
            requires(IsSame<TraitsForT, Traits<T>>)
        [[nodiscard]] Iterator find(K const& value)
        {
            return find(Traits<K>::hash(value), [&](auto& other)
                        { return Traits<T>::equals(other, value); });
        }

        /**
         * @tparam K 
         * @tparam TUnaryPredicate 
         */
        template <Concepts::HashCompatible<T> K, typename TUnaryPredicate>
            requires(IsSame<TraitsForT, Traits<T>>)
        [[nodiscard]] Iterator find(K const& value, TUnaryPredicate predicate)
        {
            return find(Traits<K>::hash(value), move(predicate));
        }

        /**
         * @tparam K 
         */
        template <Concepts::HashCompatible<T> K>
            requires(IsSame<TraitsForT, Traits<T>>)
        [[nodiscard]] ConstIterator find(K const& value) const
        {
            return find(Traits<K>::hash(value), [&](auto& other)
                        { return Traits<T>::equals(other, value); });
        }

        /**
         * @tparam K 
         * @tparam TUnaryPredicate 
         */
        template <Concepts::HashCompatible<T> K, typename TUnaryPredicate>
            requires(IsSame<TraitsForT, Traits<T>>)
        [[nodiscard]] ConstIterator find(K const& value, TUnaryPredicate predicate) const
        {
            return find(Traits<K>::hash(value), move(predicate));
        }

        /**
         * @param value 
         * @return true 
         * @return false 
         */
        bool remove(const T& value)
        {
            auto it = find(value);
            if(it != end())
            {
                remove(it);
                return true;
            }
            return false;
        }

        /**
         * @tparam K 
         */
        template <Concepts::HashCompatible<T> K>
            requires(IsSame<TraitsForT, Traits<T>>)
        bool remove(K const& value)
        {
            auto it = find(value);
            if(it != end())
            {
                remove(it);
                return true;
            }
            return false;
        }   

        /**
         * @param iterator 
         */
        void remove(Iterator iterator)
        {
            VERIFY(iterator.m_bucket);
            auto& bucket = *iterator.m_bucket;
            VERIFY(is_used_bucket(bucket.state));

            delete_bucket(bucket);
            --m_size;
            ++m_deleted_count;

            rehash_in_place_if_needed();
        }

        /**
         * @tparam TUnaryPredicate 
         * @param predicate 
         * @return true 
         * @return false 
         */
        template <typename TUnaryPredicate>
        bool remove_all_matching(TUnaryPredicate predicate)
        {
            size_t removed_count = 0;
            for(size_t i = 0; i < m_capacity; ++i)
            {
                auto& bucket = m_buckets[i];
                if(is_used_bucket(bucket.state) && predicate(*bucket.slot()))
                {
                    delete_bucket(bucket);
                    ++removed_count;
                }
            }
            if(removed_count)
            {
                m_deleted_count += removed_count;
                m_size -= removed_count;
            }
            rehash_in_place_if_needed();
            return removed_count;
        }

    private:
        /**
         * @param value 
         */
        void insert_during_rehash(T&& value)
        {
            auto& bucket = lookup_for_writing(value);
            new(bucket.slot()) T(move(value));
            bucket.state = BucketState::Used;

            if constexpr(IsOrdered)
            {
                if(!m_collection_data.head) [[unlikely]]
                {
                    m_collection_data.head = &bucket;
                }
                else
                {
                    bucket.previous = m_collection_data.tail;
                    m_collection_data.tail->next = &bucket;
                }
                m_collection_data.tail = &bucket;
            }
        }

        /**
         * @param capacity 
         * @return constexpr size_t 
         */
        [[nodiscard]] static constexpr size_t size_in_bytes(size_t capacity)
        {
            if constexpr(IsOrdered)
            {
                return sizeof(BucketType) * capacity;
            }
            else
            {
                return sizeof(BucketType) * (capacity + 1);
            }
        }

        /**
         * @param new_capacity 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> try_rehash(size_t new_capacity)
        {
            if(new_capacity == m_capacity && new_capacity >= 4)
            {
                rehash_in_place();
                return {};
            }

            new_capacity = max(new_capacity, static_cast<size_t>(4));
            new_capacity = kmalloc_good_size(new_capacity * sizeof(BucketType)) / sizeof(BucketType);

            auto* old_buckets = m_buckets;
            auto old_capacity = m_capacity;
            Iterator old_iter = begin();

            auto* new_buckets = kcalloc(1, size_in_bytes(new_capacity));
            if(!new_buckets)
                return Error::from_errno(ENOMEM);

            m_buckets = (BucketType*)new_buckets;

            m_capacity = new_capacity;
            m_deleted_count = 0;

            if constexpr(IsOrdered)
                m_collection_data = {nullptr, nullptr};
            else
                m_buckets[m_capacity].state = BucketState::End;

            if(!old_buckets)
                return {};

            for(auto it = move(old_iter); it != end(); ++it)
            {
                insert_during_rehash(move(*it));
                it->~T();
            }

            kfree_sized(old_buckets, size_in_bytes(old_capacity));
            return {};
        }

        /**
         * @param new_capacity 
         */
        void rehash(size_t new_capacity)
        {
            MUST(try_rehash(new_capacity));
        }

        void rehash_in_place()
        {
            for(size_t i = 0; i < m_capacity; ++i)
            {
                auto& bucket = m_buckets[i];

                if(bucket.state == BucketState::Rehashed || bucket.state == BucketState::End || bucket.state == BucketState::Free)
                    continue;
                if(bucket.state == BucketState::Deleted)
                {
                    bucket.state = BucketState::Free;
                    continue;
                }

                auto const new_hash = TraitsForT::hash(*bucket.slot());
                if(new_hash % m_capacity == i)
                {
                    bucket.state = BucketState::Rehashed;
                    continue;
                }

                auto target_hash = new_hash;
                auto const to_move_hash = i;
                BucketType* target_bucket = &m_buckets[target_hash % m_capacity];
                BucketType* bucket_to_move = &m_buckets[i];

                while(!is_free_bucket(bucket_to_move->state))
                {
                    if(to_move_hash == target_hash % m_capacity)
                    {
                        bucket_to_move->state = BucketState::Rehashed;
                        break;
                    }

                    if(is_free_bucket(target_bucket->state))
                    {
                        new(target_bucket->slot()) T(move(*bucket_to_move->slot()));
                        target_bucket->state = BucketState::Rehashed;
                        bucket_to_move->state = BucketState::Free;

                        if constexpr(IsOrdered)
                        {
                            swap(bucket_to_move->previous, target_bucket->previous);
                            swap(bucket_to_move->next, target_bucket->next);

                            if(target_bucket->previous)
                                target_bucket->previous->next = target_bucket;
                            else
                                m_collection_data.head = target_bucket;
                            if(target_bucket->next)
                                target_bucket->next->previous = target_bucket;
                            else
                                m_collection_data.tail = target_bucket;
                        }
                    }
                    else if(target_bucket->state == BucketState::Rehashed)
                    {
                        target_hash = double_hash(target_hash);
                        target_bucket = &m_buckets[target_hash % m_capacity];
                    }
                    else
                    {
                        VERIFY(target_bucket->state != BucketState::End);                        
                        swap(*bucket_to_move->slot(), *target_bucket->slot());
                        bucket_to_move->state = target_bucket->state;
                        target_bucket->state = BucketState::Rehashed;

                        if constexpr(IsOrdered)
                        {
                            swap(bucket_to_move->previous, target_bucket->previous);
                            swap(bucket_to_move->next, target_bucket->next);

                            if(target_bucket->previous)
                                target_bucket->previous->next = target_bucket;
                            else
                                m_collection_data.head = target_bucket;
                            if(target_bucket->next)
                                target_bucket->next->previous = target_bucket;
                            else
                                m_collection_data.tail = target_bucket;
                        }

                        target_hash = TraitsForT::hash(*bucket_to_move->slot());
                        target_bucket = &m_buckets[target_hash % m_capacity];

                        if(target_hash % m_capacity == to_move_hash)
                        {
                            bucket_to_move->state = BucketState::Rehashed;
                            if constexpr(IsOrdered)
                            {
                                if(bucket_to_move->previous)
                                    bucket_to_move->previous->next = bucket_to_move;
                                else
                                    m_collection_data.head = bucket_to_move;
                                if(bucket_to_move->next)
                                    bucket_to_move->next->previous = bucket_to_move;
                                else
                                    m_collection_data.tail = bucket_to_move;
                            }
                            break;
                        }
                    }
                }
                
                if(bucket_to_move->state == BucketState::Deleted)
                    bucket_to_move->state = BucketState::Free;
            }

            for(size_t i = 0; i < m_capacity; ++i)
            {
                if(m_buckets[i].state == BucketState::Rehashed)
                    m_buckets[i].state = BucketState::Used;
            }

            m_deleted_count = 0;
        }

        void rehash_in_place_if_needed()
        {
            if(m_deleted_count >= m_size && should_grow())
                rehash_in_place();
        }

        /**
         * @tparam TUnaryPredicate 
         * @param hash 
         * @param predicate 
         * @return BucketType* 
         */
        template <typename TUnaryPredicate>
        [[nodiscard]] BucketType* lookup_with_hash(unsigned hash, TUnaryPredicate predicate) const
        {
            if(is_empty())
                return nullptr;

            for(;;)
            {
                auto& bucket = m_buckets[hash % m_capacity];

                if(is_used_bucket(bucket.state) && predicate(*bucket.slot()))
                    return &bucket;

                if(bucket.state != BucketState::Used && bucket.state != BucketState::Deleted)
                    return nullptr;

                hash = double_hash(hash);
            }
        }

        /**
         * @param value 
         * @return ErrorOr<BucketType*> 
         */
        ErrorOr<BucketType*> try_lookup_for_writing(T const& value)
        {
            if(should_grow())
                TRY(try_rehash(capacity() * 2));
            auto hash = TraitsForT::hash(value);
            BucketType* first_empty_bucket = nullptr;
            for(;;)
            {
                auto& bucket = m_buckets[hash % m_capacity];

                if(is_used_bucket(bucket.state) && TraitsForT::equals(*bucket.slot(), value))
                    return &bucket;

                if(!is_used_bucket(bucket.state))
                {
                    if(!first_empty_bucket)
                        first_empty_bucket = &bucket;

                    if(bucket.state != BucketState::Deleted)
                        return const_cast<BucketType*>(first_empty_bucket);
                }

                hash = double_hash(hash);
            }
        }

        /**
         * @param value 
         * @return BucketType& 
         */
        [[nodiscard]] BucketType& lookup_for_writing(T const& value)
        {
            return *MUST(try_lookup_for_writing(value));
        }

        /**
         * @return size_t 
         */
        [[nodiscard]] size_t used_bucket_count() const
        {
            return m_size + m_deleted_count;
        }

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool should_grow() const
        {
            return ((used_bucket_count() + 1) * 100) >= (m_capacity * load_factor_in_percent);
        }

        /**
         * @param bucket 
         */
        void delete_bucket(auto& bucket)
        {
            bucket.slot()->~T();
            bucket.state = BucketState::Deleted;

            if constexpr(IsOrdered)
            {
                if(bucket.previous)
                    bucket.previous->next = bucket.next;
                else
                    m_collection_data.head = bucket.next;

                if(bucket.next)
                    bucket.next->previous = bucket.previous;
                else
                    m_collection_data.tail = bucket.previous;
            }
        }

        BucketType* m_buckets{nullptr};

        [[no_unique_address]] CollectionDataType m_collection_data;
        size_t m_size{0};
        size_t m_capacity{0};
        size_t m_deleted_count{0};
    }; // class HashTable
} // namespace Mods

using Mods::HashTable;
using Mods::OrderedHashTable;
