/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <Base/Assertions.h>
#include <Base/Find.h>
#include <Base/Forward.h>
#include <Base/Iterator.h>
#include <Base/Optional.h>
#include <Base/Span.h>
#include <Base/StdLibExtras.h>
#include <Base/Traits.h>
#include <Base/TypedTransfer.h>
#include <Base/kmalloc.h>


#ifndef PRANAOS_LIBC_BUILD
#    include <initializer_list>
#endif

#ifndef __pranaos__
#    include <new>
#endif

namespace Base {

namespace Detail {

template<typename StorageType, bool>
struct CanBePlacedInsideVectorHelper;

template<typename StorageType>
struct CanBePlacedInsideVectorHelper<StorageType, true> {
    template<typename U>
    static constexpr bool value = requires(U&& u) { StorageType { &u }; };
};

template<typename StorageType>
struct CanBePlacedInsideVectorHelper<StorageType, false> {
    template<typename U>
    static constexpr bool value = requires(U&& u) { StorageType(forward<U>(u)); };
};
}

template<typename T, size_t inline_capacity>
requires(!IsRvalueReference<T>) class Vector {
private:
    static constexpr bool contains_reference = IsLvalueReference<T>;
    using StorageType = Conditional<contains_reference, RawPtr<RemoveReference<T>>, T>;

    template<typename U>
    static constexpr bool CanBePlacedInsideVector = Detail::CanBePlacedInsideVectorHelper<StorageType, contains_reference>::template value<U>;

public:
    using ValueType = T;
    Vector()
        : m_capacity(inline_capacity)
    {
    }

#ifndef PRANAOS_LIBC_BUILD
    Vector(std::initializer_list<T> list) requires(!IsLvalueReference<T>)
    {
        ensure_capacity(list.size());
        for (auto& item : list)
            unchecked_append(item);
    }
#endif

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

    Vector(Vector const& other)
    {
        ensure_capacity(other.size());
        TypedTransfer<StorageType>::copy(data(), other.data(), other.size());
        m_size = other.size();
    }

    template<size_t other_inline_capacity>
    Vector(Vector<T, other_inline_capacity> const& other)
    {
        ensure_capacity(other.size());
        TypedTransfer<StorageType>::copy(data(), other.data(), other.size());
        m_size = other.size();
    }

    ~Vector()
    {
        clear();
    }

    Span<StorageType> span() { return { data(), size() }; }
    Span<StorageType const> span() const { return { data(), size() }; }

    operator Span<StorageType>() { return span(); }
    operator Span<StorageType const>() const { return span(); }

    bool is_empty() const { return size() == 0; }
    ALWAYS_INLINE size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }

    StorageType* data()
    {
        if constexpr (inline_capacity > 0)
            return m_outline_buffer ? m_outline_buffer : inline_buffer();
        return m_outline_buffer;
    }

    StorageType const* data() const
    {
        if constexpr (inline_capacity > 0)
            return m_outline_buffer ? m_outline_buffer : inline_buffer();
        return m_outline_buffer;
    }

    ALWAYS_INLINE T const& at(size_t i) const
    {
        VERIFY(i < m_size);
        if constexpr (contains_reference)
            return *data()[i];
        else
            return data()[i];
    }

    ALWAYS_INLINE T& at(size_t i)
    {
        VERIFY(i < m_size);
        if constexpr (contains_reference)
            return *data()[i];
        else
            return data()[i];
    }

    ALWAYS_INLINE T const& operator[](size_t i) const { return at(i); }
    ALWAYS_INLINE T& operator[](size_t i) { return at(i); }

    T const& first() const { return at(0); }
    T& first() { return at(0); }

    T const& last() const { return at(size() - 1); }
    T& last() { return at(size() - 1); }

    template<typename TUnaryPredicate>
    Optional<T> first_matching(TUnaryPredicate predicate)
    {
        for (size_t i = 0; i < size(); ++i) {
            if (predicate(at(i))) {
                return at(i);
            }
        }
        return {};
    }

    template<typename TUnaryPredicate>
    Optional<T> last_matching(TUnaryPredicate predicate)
    {
        for (ssize_t i = size() - 1; i >= 0; --i) {
            if (predicate(at(i))) {
                return at(i);
            }
        }
        return {};
    }

    template<typename V>
    bool operator==(V const& other) const
    {
        if (m_size != other.size())
            return false;
        return TypedTransfer<StorageType>::compare(data(), other.data(), size());
    }

    bool contains_slow(T const& value) const
    {
        for (size_t i = 0; i < size(); ++i) {
            if (Traits<T>::equals(at(i), value))
                return true;
        }
        return false;
    }

    bool contains_in_range(T const& value, size_t const start, size_t const end) const
    {
        VERIFY(start <= end);
        VERIFY(end < size());
        for (size_t i = start; i <= end; ++i) {
            if (Traits<T>::equals(at(i), value))
                return true;
        }
        return false;
    }

    template<typename U = T>
    void insert(size_t index, U&& value) requires(CanBePlacedInsideVector<U>)
    {
        auto did_allocate = try_insert<U>(index, forward<U>(value));
        VERIFY(did_allocate);
    }

    template<typename TUnaryPredicate, typename U = T>
    void insert_before_matching(U&& value, TUnaryPredicate predicate, size_t first_index = 0, size_t* inserted_index = nullptr) requires(CanBePlacedInsideVector<U>)
    {
        auto did_allocate = try_insert_before_matching(forward<U>(value), predicate, first_index, inserted_index);
        VERIFY(did_allocate);
    }

    void extend(Vector&& other)
    {
        auto did_allocate = try_extend(move(other));
        VERIFY(did_allocate);
    }

    void extend(Vector const& other)
    {
        auto did_allocate = try_extend(other);
        VERIFY(did_allocate);
    }

    ALWAYS_INLINE void append(T&& value)
    {
        bool did_allocate;
        if constexpr (contains_reference)
            did_allocate = try_append(value);
        else
            did_allocate = try_append(move(value));
        VERIFY(did_allocate);
    }


}