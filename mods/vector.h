/**
 * @file vector.h
 * @author Krisna Pranav
 * @brief Vector
 * @version 6.0
 * @date 2023-07-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/assertions.h>
#include <mods/error.h>
#include <mods/find.h>
#include <mods/forward.h>
#include <mods/iterator.h>
#include <mods/optional.h>
#include <mods/span.h>
#include <mods/stdlibextra.h>
#include <mods/traits.h>
#include <mods/typedtransfer.h>
#include <mods/kmalloc.h>
#include <initializer_list>

namespace Mods 
{
    namespace Detail 
    {
        /**
         * @tparam StorageType 
         * @tparam bool 
         */
        template<typename StorageType, bool>
        struct CanBePlacedInsideVectorHelper;

        /**
         * @tparam StorageType 
         */
        template<typename StorageType>
        struct CanBePlacedInsideVectorHelper<StorageType, true> 
        {
            template<typename U>
            static constexpr bool value = requires(U&& u) { StorageType { &u }; };
        };

        /**
         * @tparam StorageType 
         */
        template<typename StorageType>
        struct CanBePlacedInsideVectorHelper<StorageType, false> 
        {
            /**
             * @tparam U 
             */
            template<typename U>
            static constexpr bool value = requires(U&& u) { StorageType(forward<U>(u)); };
        };
    } // namespace Detail

    /**
     * @tparam T 
     * @tparam inline_capacity 
     */
    template<typename T, size_t inline_capacity>
    requires(!IsRvalueReference<T>) class Vector {
    private:
        static constexpr bool contains_reference = IsLvalueReference<T>;
        using StorageType = Conditional<contains_reference, RawPtr<RemoveReference<T>>, T>;

        using VisibleType = RemoveReference<T>;

        template<typename U>
        static constexpr bool CanBePlacedInsideVector = Detail::CanBePlacedInsideVectorHelper<StorageType, contains_reference>::template value<U>;

    public:
        using ValueType = T;
        
        /**
         * @brief Construct a new Vector object
         * 
         */
        Vector()
            : m_capacity(inline_capacity)
        {
        }

        Vector(std::initializer_list<T> list) requires(!IsLvalueReference<T>)
        {
            ensure_capacity(list.size());
            for (auto& item : list)
                unchecked_append(item);
        }

        /**
         * @brief Construct a new Vector object
         * 
         * @param other 
         */
        Vector(Vector&& other)
            : m_size(other.m_size)
            , m_capacity(other.m_capacity)
            , m_outline_buffer(other.m_outline_buffer)
        {
            if constexpr (inline_capacity > 0) {
                if (!m_outline_buffer) {
                    for (size_t i = 0; i < m_size; ++i) {
                        new (&inline_buffer()[i]) StorageType(move(other.inline_buffer()[i]));
                        other.inline_buffer()[i].~StorageType();
                    }
                }
            }
            other.m_outline_buffer = nullptr;
            other.m_size = 0;
            other.reset_capacity();
        }

        /**
         * @brief Construct a new Vector object
         * 
         * @param other 
         */
        Vector(Vector const& other)
        {
            ensure_capacity(other.size());
            TypedTransfer<StorageType>::copy(data(), other.data(), other.size());
            m_size = other.size();
        }

        /**
         * @brief Construct a new Vector object
         * 
         * @tparam other_inline_capacity 
         * @param other 
         */
        template<size_t other_inline_capacity>
        Vector(Vector<T, other_inline_capacity> const& other)
        {
            ensure_capacity(other.size());
            TypedTransfer<StorageType>::copy(data(), other.data(), other.size());
            m_size = other.size();
        }

        /**
         * @brief Destroy the Vector object
         * 
         */
        ~Vector()
        {
            clear();
        }

        Span<StorageType> span() 
        { 
            return { data(), size() }; 
        }

        Span<StorageType const> span() const 
        { 
            return { data(), size() }; 
        }

        operator Span<StorageType>() 
        { 
            return span(); 
        }

        operator Span<StorageType const>() const 
        { 
            return span(); 
        }

        bool is_empty() const
        {
            return size() == 0;
        }

        ALWAYS_INLINE size_t size() const
        {
            return m_size;
        }

        size_t capacity() const
        {
            return m_capacity;
        }

        ALWAYS_INLINE StorageType* data()
        {
            if constexpr (inline_capacity > 0)
                return m_outline_buffer ? m_outline_buffer : inline_buffer();
            return m_outline_buffer;
        }

        ALWAYS_INLINE StorageType const* data() const
        {
            if constexpr (inline_capacity > 0)
                return m_outline_buffer ? m_outline_buffer : inline_buffer();
            return m_outline_buffer;
        }

        /**
         * @param i 
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE VisibleType const& at(size_t i) const
        {
            VERIFY(i < m_size);
            if constexpr (contains_reference)
                return *data()[i];
            else
                return data()[i];
        }

        /**
         * @param i 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE VisibleType& at(size_t i)
        {
            VERIFY(i < m_size);
            if constexpr (contains_reference)
                return *data()[i];
            else
                return data()[i];
        }

        /**
         * @param i 
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE VisibleType const& operator[](size_t i) const 
        { 
            return at(i); 
        }

        /**
         * @param i 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE VisibleType& operator[](size_t i) 
        { 
            return at(i); 
        }

        /**
         * @return VisibleType const& 
         */
        VisibleType const& first() const 
        { 
            return at(0); 
        }

        VisibleType& first() 
        { 
            return at(0); 
        }

        VisibleType const& last() const 
        { 
            return at(size() - 1); 
        }

        VisibleType& last() 
        { 
            return at(size() - 1); 
        }

        /**
         * @tparam TUnaryPredicate 
         * @param predicate
         */
        template<typename TUnaryPredicate>
        Optional<VisibleType> first_matching(TUnaryPredicate predicate) requires(!contains_reference)
        {
            for (size_t i = 0; i < size(); ++i) {
                if (predicate(at(i))) {
                    return at(i);
                }
            }
            return {};
        }

        /**
         * @param predicate
         * @tparam TUnaryPredicate 
         */
        template<typename TUnaryPredicate>
        Optional<VisibleType> last_matching(TUnaryPredicate predicate) requires(!contains_reference)
        {
            for (ssize_t i = size() - 1; i >= 0; --i) {
                if (predicate(at(i))) {
                    return at(i);
                }
            }
            return {};
        }

        /**
         * @tparam V 
         * @param other 
         * @return true 
         * @return false 
         */
        template<typename V>
        bool operator==(V const& other) const
        {
            if (m_size != other.size())
                return false;
            return TypedTransfer<StorageType>::compare(data(), other.data(), size());
        }

        /**
         * @param value 
         * @return true 
         * @return false 
         */
        bool contains_slow(VisibleType const& value) const
        {
            for (size_t i = 0; i < size(); ++i) {
                if (Traits<VisibleType>::equals(at(i), value))
                    return true;
            }
            return false;
        }

        /**
         * @param value 
         * @param start 
         * @param end 
         * @return true 
         * @return false 
         */
        bool contains_in_range(VisibleType const& value, size_t const start, size_t const end) const
        {
            VERIFY(start <= end);
            VERIFY(end < size());
            for (size_t i = start; i <= end; ++i) {
                if (Traits<VisibleType>::equals(at(i), value))
                    return true;
            }
            return false;
        }

    #ifndef KERNEL

        /**
         * @tparam U 
         * @param index
         * @param value
         */
        template<typename U = T>
        void insert(size_t index, U&& value) requires(CanBePlacedInsideVector<U>)
        {
            MUST(try_insert<U>(index, forward<U>(value)));
        }

        /**
         * @tparam TUnaryPredicate 
         * @tparam U 
         */
        template<typename TUnaryPredicate, typename U = T>
        void insert_before_matching(U&& value, TUnaryPredicate predicate, size_t first_index = 0, size_t* inserted_index = nullptr) requires(CanBePlacedInsideVector<U>)
        {
            MUST(try_insert_before_matching(forward<U>(value), predicate, first_index, inserted_index));
        }

        /**
         * @param other 
         */
        void extend(Vector&& other)
        {
            MUST(try_extend(move(other)));
        }

        /**
         * @param other 
         */
        void extend(Vector const& other)
        {
            MUST(try_extend(other));
        }

    #endif

        ALWAYS_INLINE void append(T&& value)
        {
            if constexpr (contains_reference)
                MUST(try_append(value));
            else
                MUST(try_append(move(value)));
        }

        ALWAYS_INLINE void append(T const& value) requires(!contains_reference)
        {
            MUST(try_append(T(value)));
        }

    #ifndef KERNEL
        /**
         * @param values 
         * @param count 
         */
        void append(StorageType const* values, size_t count)
        {
            MUST(try_append(values, count));
        }
    #endif

        /**
         * @tparam U 
         */
        template<typename U = T>
        ALWAYS_INLINE void unchecked_append(U&& value) requires(CanBePlacedInsideVector<U>)
        {
            VERIFY((size() + 1) <= capacity());
            if constexpr (contains_reference)
                new (slot(m_size)) StorageType(&value);
            else
                new (slot(m_size)) StorageType(forward<U>(value));
            ++m_size;
        }

        /**
         * @param values 
         * @param count 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void unchecked_append(StorageType const* values, size_t count)
        {
            if (count == 0)
                return;
            VERIFY((size() + count) <= capacity());
            TypedTransfer<StorageType>::copy(slot(m_size), values, count);
            m_size += count;
        }

    #ifndef KERNEL
        /**
         * @tparam Args 
         */
        template<class... Args>
        void empend(Args&&... args) requires(!contains_reference)
        {
            MUST(try_empend(forward<Args>(args)...));
        }   

        /**
         * @tparam U 
         */
        template<typename U = T>
        void prepend(U&& value) requires(CanBePlacedInsideVector<U>)
        {
            MUST(try_insert(0, forward<U>(value)));
        }

        /**
         * @param other 
         */
        void prepend(Vector&& other)
        {
            MUST(try_prepend(move(other)));
        }

        /**
         * @param values 
         * @param count 
         */
        void prepend(StorageType const* values, size_t count)
        {
            MUST(try_prepend(values, count));
        }

    #endif

        /**
         * @param other 
         * @return Vector& 
         */
        Vector& operator=(Vector&& other)
        {
            if (this != &other) {
                clear();
                m_size = other.m_size;
                m_capacity = other.m_capacity;
                m_outline_buffer = other.m_outline_buffer;
                if constexpr (inline_capacity > 0) {
                    if (!m_outline_buffer) {
                        for (size_t i = 0; i < m_size; ++i) {
                            new (&inline_buffer()[i]) StorageType(move(other.inline_buffer()[i]));
                            other.inline_buffer()[i].~StorageType();
                        }
                    }
                }
                other.m_outline_buffer = nullptr;
                other.m_size = 0;
                other.reset_capacity();
            }
            return *this;
        }

        /**
         * @param other 
         * @return Vector& 
         */
        Vector& operator=(Vector const& other)
        {
            if (this != &other) {
                clear();
                ensure_capacity(other.size());
                TypedTransfer<StorageType>::copy(data(), other.data(), other.size());
                m_size = other.size();
            }
            return *this;
        }

        /**
         * @tparam other_inline_capacity 
         * @param other 
         * @return Vector& 
         */
        template<size_t other_inline_capacity>
        Vector& operator=(Vector<T, other_inline_capacity> const& other)
        {
            clear();
            ensure_capacity(other.size());
            TypedTransfer<StorageType>::copy(data(), other.data(), other.size());
            m_size = other.size();
            return *this;
        }

        void clear()
        {
            clear_with_capacity();
            if (m_outline_buffer) {
                kfree_sized(m_outline_buffer, m_capacity * sizeof(StorageType));
                m_outline_buffer = nullptr;
            }
            reset_capacity();
        }

        void clear_with_capacity()
        {
            for (size_t i = 0; i < m_size; ++i)
                data()[i].~StorageType();
            m_size = 0;
        }

        /**
         * @param index 
         */
        void remove(size_t index)
        {
            VERIFY(index < m_size);

            if constexpr (Traits<StorageType>::is_trivial()) {
                TypedTransfer<StorageType>::copy(slot(index), slot(index + 1), m_size - index - 1);
            } else {
                at(index).~StorageType();
                for (size_t i = index + 1; i < m_size; ++i) {
                    new (slot(i - 1)) StorageType(move(at(i)));
                    at(i).~StorageType();
                }
            }

            --m_size;
        }

        /**
         * @param index 
         * @param count 
         */
        void remove(size_t index, size_t count)
        {
            if (count == 0)
                return;
            VERIFY(index + count > index);
            VERIFY(index + count <= m_size);

            if constexpr (Traits<StorageType>::is_trivial()) {
                TypedTransfer<StorageType>::copy(slot(index), slot(index + count), m_size - index - count);
            } else {
                for (size_t i = index; i < index + count; i++)
                    at(i).~StorageType();
                for (size_t i = index + count; i < m_size; ++i) {
                    new (slot(i - count)) StorageType(move(at(i)));
                    at(i).~StorageType();
                }
            }

            m_size -= count;
        }   

        /**
         * @tparam TUnaryPredicate 
         * @param predicate 
         * @return true 
         * @return false 
         */
        template<typename TUnaryPredicate>
        bool remove_first_matching(TUnaryPredicate predicate)
        {
            for (size_t i = 0; i < size(); ++i) {
                if (predicate(at(i))) {
                    remove(i);
                    return true;
                }
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
            for (size_t i = 0; i < size();) {
                if (predicate(at(i))) {
                    remove(i);
                    something_was_removed = true;
                } else {
                    ++i;
                }
            }
            return something_was_removed;
        }

        ALWAYS_INLINE T take_last()
        {
            VERIFY(!is_empty());
            auto value = move(raw_last());
            if constexpr (!contains_reference)
                last().~T();
            --m_size;
            if constexpr (contains_reference)
                return *value;
            else
                return value;
        }

        T take_first()
        {
            VERIFY(!is_empty());
            auto value = move(raw_first());
            remove(0);
            if constexpr (contains_reference)
                return *value;
            else
                return value;
        }

        /**
         * @param index 
         * @return T 
         */
        T take(size_t index)
        {
            auto value = move(raw_at(index));
            remove(index);
            if constexpr (contains_reference)
                return *value;
            else
                return value;
        }

        /**
         * @param index 
         * @return T 
         */
        T unstable_take(size_t index)
        {
            VERIFY(index < m_size);
            swap(raw_at(index), raw_at(m_size - 1));
            return take_last();
        }

        /**
         * @tparam U 
         */
        template<typename U = T>
        ErrorOr<void> try_insert(size_t index, U&& value) requires(CanBePlacedInsideVector<U>)
        {
            if (index > size())
                return Error::from_errno(EINVAL);
            if (index == size())
                return try_append(forward<U>(value));
            TRY(try_grow_capacity(size() + 1));
            ++m_size;
            if constexpr (Traits<StorageType>::is_trivial()) {
                TypedTransfer<StorageType>::move(slot(index + 1), slot(index), m_size - index - 1);
            } else {
                for (size_t i = size() - 1; i > index; --i) {
                    new (slot(i)) StorageType(move(at(i - 1)));
                    at(i - 1).~StorageType();
                }
            }
            if constexpr (contains_reference)
                new (slot(index)) StorageType(&value);
            else
                new (slot(index)) StorageType(forward<U>(value));
            return {};
        }

        /**
         * @tparam TUnaryPredicate 
         * @tparam U 
         */
        template<typename TUnaryPredicate, typename U = T>
        ErrorOr<void> try_insert_before_matching(U&& value, TUnaryPredicate predicate, size_t first_index = 0, size_t* inserted_index = nullptr) requires(CanBePlacedInsideVector<U>)
        {
            for (size_t i = first_index; i < size(); ++i) {
                if (predicate(at(i))) {
                    TRY(try_insert(i, forward<U>(value)));
                    if (inserted_index)
                        *inserted_index = i;
                    return {};
                }
            }
            TRY(try_append(forward<U>(value)));
            if (inserted_index)
                *inserted_index = size() - 1;
            return {};
        }

        /**
         * @param other 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> try_extend(Vector&& other)
        {
            if (is_empty() && capacity() <= other.capacity()) {
                *this = move(other);
                return {};
            }
            auto other_size = other.size();
            Vector tmp = move(other);
            TRY(try_grow_capacity(size() + other_size));
            TypedTransfer<StorageType>::move(data() + m_size, tmp.data(), other_size);
            m_size += other_size;
            return {};
        }       

        /**
         * @param other 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> try_extend(Vector const& other)
        {
            TRY(try_grow_capacity(size() + other.size()));
            TypedTransfer<StorageType>::copy(data() + m_size, other.data(), other.size());
            m_size += other.m_size;
            return {};
        }

        /**
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> try_append(T&& value)
        {
            TRY(try_grow_capacity(size() + 1));
            if constexpr (contains_reference)
                new (slot(m_size)) StorageType(&value);
            else
                new (slot(m_size)) StorageType(move(value));
            ++m_size;
            return {};
        }

        /**
         * @param value 
         */
        ErrorOr<void> try_append(T const& value) requires(!contains_reference)
        {
            return try_append(T(value));
        }

        /**
         * @param values 
         * @param count 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> try_append(StorageType const* values, size_t count)
        {
            if (count == 0)
                return {};
            TRY(try_grow_capacity(size() + count));
            TypedTransfer<StorageType>::copy(slot(m_size), values, count);
            m_size += count;
            return {};
        }

        /**
         * @tparam Args 
         */
        template<class... Args>
        ErrorOr<void> try_empend(Args&&... args) requires(!contains_reference)
        {
            TRY(try_grow_capacity(m_size + 1));
            new (slot(m_size)) StorageType { forward<Args>(args)... };
            ++m_size;
            return {};
        }

        /**
         * @tparam U 
         */
        template<typename U = T>
        ErrorOr<void> try_prepend(U&& value) requires(CanBePlacedInsideVector<U>)
        {
            return try_insert(0, forward<U>(value));
        }

        /**
         * @param other 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> try_prepend(Vector&& other)
        {
            if (other.is_empty())
                return {};

            if (is_empty()) {
                *this = move(other);
                return {};
            }

            auto other_size = other.size();
            TRY(try_grow_capacity(size() + other_size));

            for (size_t i = size() + other_size - 1; i >= other.size(); --i) {
                new (slot(i)) StorageType(move(at(i - other_size)));
                at(i - other_size).~StorageType();
            }

            Vector tmp = move(other);
            TypedTransfer<StorageType>::move(slot(0), tmp.data(), tmp.size());
            m_size += other_size;
            return {};
        }

        /**
         * @param values 
         * @param count 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> try_prepend(StorageType const* values, size_t count)
        {
            if (count == 0)
                return {};
            TRY(try_grow_capacity(size() + count));
            TypedTransfer<StorageType>::move(slot(count), slot(0), m_size);
            TypedTransfer<StorageType>::copy(slot(0), values, count);
            m_size += count;
            return {};
        }

        /**
         * @param needed_capacity 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> try_grow_capacity(size_t needed_capacity)
        {
            if (m_capacity >= needed_capacity)
                return {};
            return try_ensure_capacity(padded_capacity(needed_capacity));
        }

        /**
         * @param needed_capacity 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> try_ensure_capacity(size_t needed_capacity)
        {
            if (m_capacity >= needed_capacity)
                return {};
            size_t new_capacity = kmalloc_good_size(needed_capacity * sizeof(StorageType)) / sizeof(StorageType);
            auto* new_buffer = static_cast<StorageType*>(kmalloc_array(new_capacity, sizeof(StorageType)));
            if (new_buffer == nullptr)
                return Error::from_errno(ENOMEM);

            if constexpr (Traits<StorageType>::is_trivial()) {
                TypedTransfer<StorageType>::copy(new_buffer, data(), m_size);
            } else {
                for (size_t i = 0; i < m_size; ++i) {
                    new (&new_buffer[i]) StorageType(move(at(i)));
                    at(i).~StorageType();
                }
            }
            if (m_outline_buffer)
                kfree_sized(m_outline_buffer, m_capacity * sizeof(StorageType));
            m_outline_buffer = new_buffer;
            m_capacity = new_capacity;
            return {};
        }

        /**
         * @param new_size
         * @param keep_capacity
         */
        ErrorOr<void> try_resize(size_t new_size, bool keep_capacity = false) requires(!contains_reference)
        {
            if (new_size <= size()) {
                shrink(new_size, keep_capacity);
                return {};
            }

            TRY(try_ensure_capacity(new_size));

            for (size_t i = size(); i < new_size; ++i)
                new (slot(i)) StorageType {};
            m_size = new_size;
            return {};
        }

        /**
         * @param new_size
         * 
         */
        ErrorOr<void> try_resize_and_keep_capacity(size_t new_size) requires(!contains_reference)
        {
            return try_resize(new_size, true);
        }   

        /**
         * @param needed_capacity 
         */
        void grow_capacity(size_t needed_capacity)
        {
            MUST(try_grow_capacity(needed_capacity));
        }

        /**
         * @param needed_capacity 
         */
        void ensure_capacity(size_t needed_capacity)
        {
            MUST(try_ensure_capacity(needed_capacity));
        }

        /**
         * @param new_size 
         * @param keep_capacity 
         */
        void shrink(size_t new_size, bool keep_capacity = false)
        {
            VERIFY(new_size <= size());
            if (new_size == size())
                return;

            if (new_size == 0) {
                if (keep_capacity)
                    clear_with_capacity();
                else
                    clear();
                return;
            }

            for (size_t i = new_size; i < size(); ++i)
                at(i).~StorageType();
            m_size = new_size;
        }

        /**
         * @param new_size
         * @param keep_capacity
         */
        void resize(size_t new_size, bool keep_capacity = false) requires(!contains_reference)
        {
            MUST(try_resize(new_size, keep_capacity));
        }

        /**
         * @param new_size
         * 
         */
        void resize_and_keep_capacity(size_t new_size) requires(!contains_reference)
        {
            MUST(try_resize_and_keep_capacity(new_size));
        }

        using ConstIterator = SimpleIterator<Vector const, VisibleType const>;
        using Iterator = SimpleIterator<Vector, VisibleType>;

        ConstIterator begin() const 
        { 
            return ConstIterator::begin(*this); 
        }

        Iterator begin() 
        { 
            return Iterator::begin(*this); 
        }

        ConstIterator end() const 
        { 
            return ConstIterator::end(*this); 
        }

        Iterator end() 
        { 
            return Iterator::end(*this); 
        }

        /**
         * @tparam TUnaryPredicate 
         * @param finder 
         * @return ConstIterator 
         */
        template<typename TUnaryPredicate>
        ConstIterator find_if(TUnaryPredicate&& finder) const
        {
            return Mods::find_if(begin(), end(), forward<TUnaryPredicate>(finder));
        }

        /**
         * @tparam TUnaryPredicate 
         * @param finder 
         * @return Iterator 
         */
        template<typename TUnaryPredicate>
        Iterator find_if(TUnaryPredicate&& finder)
        {
            return Mods::find_if(begin(), end(), forward<TUnaryPredicate>(finder));
        }

        /**
         * @param value 
         * @return ConstIterator 
         */
        ConstIterator find(VisibleType const& value) const
        {
            return Mods::find(begin(), end(), value);
        }

        /**
         * @param value 
         * @return Iterator 
         */
        Iterator find(VisibleType const& value)
        {
            return Mods::find(begin(), end(), value);
        }

        /**
         * @param value 
         * @return Optional<size_t> 
         */
        Optional<size_t> find_first_index(VisibleType const& value) const
        {
            if (auto const index = Mods::find_index(begin(), end(), value);
                index < size()) {
                return index;
            }
            return {};
        }

        void reverse()
        {
            for (size_t i = 0; i < size() / 2; ++i)
                Mods::swap(at(i), at(size() - i - 1));
        }

    private:
        void reset_capacity()
        {
            m_capacity = inline_capacity;
        }

        /**
         * @param capacity 
         * @return size_t 
         */
        static size_t padded_capacity(size_t capacity)
        {
            return max(static_cast<size_t>(4), capacity + (capacity / 4) + 4);
        }

        /**
         * @param i 
         * @return StorageType* 
         */
        StorageType* slot(size_t i) 
        { 
            return &data()[i]; 
        }

        /**
         * @param i 
         * @return StorageType const* 
         */
        StorageType const* slot(size_t i) const 
        { 
            return &data()[i]; 
        }

        StorageType* inline_buffer()
        {
            static_assert(inline_capacity > 0);
            return reinterpret_cast<StorageType*>(m_inline_buffer_storage);
        }

        StorageType const* inline_buffer() const
        {
            static_assert(inline_capacity > 0);
            return reinterpret_cast<StorageType const*>(m_inline_buffer_storage);
        }

        StorageType& raw_last() 
        { 
            return raw_at(size() - 1); 
        }

        StorageType& raw_first() 
        { 
            return raw_at(0); 
        }

        /**
         * @param index 
         * @return StorageType& 
         */
        StorageType& raw_at(size_t index) 
        { 
            return *slot(index); 
        }

        size_t m_size { 0 };
        size_t m_capacity { 0 };

        alignas(StorageType) unsigned char m_inline_buffer_storage[sizeof(StorageType) * inline_capacity];

        StorageType* m_outline_buffer { nullptr };
    }; // class Vector

    template<class... Args>
    Vector(Args... args) -> Vector<CommonType<Args...>>;

} // namespace Mods

using Mods::Vector;