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

#include "assertions.h"
#include "forward.h"
#include "iterator.h"
#include "optional.h"
#include "span.h"
#include "stdlibextra.h"
#include "traits.h"
#include "typedtransfer.h"
#include "kmalloc.h"

#ifndef PRANAOS_LIBC_BUILD
#    include <initializer_list>
#endif

#ifndef __pranaos__
#    include <new>
#endif

namespace Mods {

    template<typename T, size_t inline_capacity>
    class Vector {
    public:
        
        using value_type = T;

        Vector()
            : m_capacity(inline_capacity)
        {
        }

        ~Vector()
        {
            clear();
        }

    #ifndef PRANAOS_LIBC_BUILD
        Vector(std::initializer_list<T> list) {
            ensure_capacity(list.size());
            for (auto& item : list)
                unchecked_append(item);
        }
    #endif

        Vector(Vector&& other)
            : m_size(other.m_size)
            , m_capacity(other.m_capacity)
            , m_outline_buffer(other.m_outline_buffer) {
            if constexpr (inline_capacity > 0) {
                if (!m_outline_buffer) {
                    for (size_t i = 0; i < m_size; ++i) {
                        new (&inline_buffer()[i]) T(move(other.inline_buffer()[i]));
                        other.inline_buffer()[i].~T();
                    }
                }
            }
            other.m_outline_buffer = nullptr;
            other.m_size = 0;
            other.reset_capacity();
        }

        /**
         * @param other 
         */
        Vector(const Vector& other) {
            ensure_capacity(other.size());
            TypedTransfer<T>::copy(data(), other.data(), other.size());
            m_size = other.size();
        }

        /**
         * @tparam other_inline_capacity 
         * @param other 
         */
        template<size_t other_inline_capacity>
        Vector(const Vector<T, other_inline_capacity>& other) {
            ensure_capacity(other.size());
            TypedTransfer<T>::copy(data(), other.data(), other.size());
            m_size = other.size();
        }

        /**
         * @return Span<T> 
         */
        Span<T> span() { 
            return { data(), size() }; 
        }
        Span<const T> span() const { 
            return { data(), size() }; 
        }

        /**
         * @param other 
         * @return Vector& 
         */
        Vector& operator=(Vector&& other) {
            if (this != &other) {
                clear();
                m_size = other.m_size;
                m_capacity = other.m_capacity;
                m_outline_buffer = other.m_outline_buffer;
                if constexpr (inline_capacity > 0) {
                    if (!m_outline_buffer) {
                        for (size_t i = 0; i < m_size; ++i) {
                            new (&inline_buffer()[i]) T(move(other.inline_buffer()[i]));
                            other.inline_buffer()[i].~T();
                        }
                    }
                }
                other.m_outline_buffer = nullptr;
                other.m_size = 0;
                other.reset_capacity();
            }
            return *this;
        }

        void clear() {
            clear_with_capacity();
            if (m_outline_buffer) {
                kfree(m_outline_buffer);
                m_outline_buffer = nullptr;
            }
            reset_capacity();
        }

        void clear_with_capacity() {
            for (size_t i = 0; i < m_size; ++i)
                data()[i].~T();
            m_size = 0;
        }

        /**
         * @tparam V 
         * @param other 
         * @return true 
         * @return false 
         */
        template<typename V>
        bool operator==(const V& other) const {
            if (m_size != other.size())
                return false;
            return TypedTransfer<T>::compare(data(), other.data(), size());
        }

        operator Span<T>() { 
            return span(); 
        }

        operator Span<const T>() const { 
            return span(); 
        }

        /**
         * @param value 
         * @return true 
         * @return false 
         */
        bool contains_slow(const T& value) const {
            for (size_t i = 0; i < size(); ++i) {
                if (Traits<T>::equals(at(i), value))
                    return true;
            }
            return false;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_null() const { 
            return false; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const { 
            return size() == 0; 
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE size_t size() const { 
            return m_size; 
        }

        /**
         * @return size_t 
         */
        size_t capacity() const { 
            return m_capacity; 
        }

        /**
         * @return T* 
         */
        T* data() {
            if constexpr (inline_capacity > 0)
                return m_outline_buffer ? m_outline_buffer : inline_buffer();
            return m_outline_buffer;
        }

        /**
         * @return const T* 
         */
        const T* data() const {
            if constexpr (inline_capacity > 0)
                return m_outline_buffer ? m_outline_buffer : inline_buffer();
            return m_outline_buffer;
        }

        /**
         * @param i 
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE const T& at(size_t i) const {
            ASSERT(i < m_size);
            return data()[i];
        }

        /**
         * @param i 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE T& at(size_t i) {
            ASSERT(i < m_size);
            return data()[i];
        }

        /**
         * @param i 
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE const T& operator[](size_t i) const { 
            return at(i); 
        }

        /**
         * @param i 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE T& operator[](size_t i) { 
            return at(i); 
        }

        /**
         * @return const T& 
         */
        const T& first() const { 
            return at(0); 
        }

        /**
         * @return T& 
         */
        T& first() { 
            return at(0); 
        }

        /**
         * @return const T& 
         */
        const T& last() const { 
            return at(size() - 1); 
        }

        /**
         * @return T& 
         */
        T& last() { 
            return at(size() - 1); 
        }

        /**
         * @return T 
         */
        T take_last() {
            ASSERT(!is_empty());
            T value = move(last());
            last().~T();
            --m_size;
            return value;
        }

        /**
         * @return T 
         */
        T take_first() {
            ASSERT(!is_empty());
            T value = move(first());
            remove(0);
            return value;
        }

        /**
         * @param index 
         * @return T 
         */
        T take(size_t index) {
            T value = move(at(index));
            remove(index);
            return value;
        }

        /**
         * @param index 
         * @return T 
         */
        T unstable_take(size_t index) {
            ASSERT(index < m_size);
            swap(at(index), at(m_size - 1));
            return take_last();
        }

        /**
         * @param index 
         */
        void remove(size_t index) {
            ASSERT(index < m_size);

            if constexpr (Traits<T>::is_trivial()) {
                TypedTransfer<T>::copy(slot(index), slot(index + 1), m_size - index - 1);
            } else {
                at(index).~T();
                for (size_t i = index + 1; i < m_size; ++i) {
                    new (slot(i - 1)) T(move(at(i)));
                    at(i).~T();
                }
            }

            --m_size;
        }

        /**
         * @param index 
         * @param value 
         */
        void insert(size_t index, T&& value) {
            ASSERT(index <= size());
            if (index == size())
                return append(move(value));
            grow_capacity(size() + 1);
            ++m_size;
            if constexpr (Traits<T>::is_trivial()) {
                TypedTransfer<T>::move(slot(index + 1), slot(index), m_size - index - 1);
            } else {
                for (size_t i = size() - 1; i > index; --i) {
                    new (slot(i)) T(move(at(i - 1)));
                    at(i - 1).~T();
                }
            }
            new (slot(index)) T(move(value));
        }

        /**
         * @param index 
         * @param value 
         */
        void insert(size_t index, const T& value) {
            insert(index, T(value));
        }

        /**
         * @tparam C 
         * @param value 
         * @param callback 
         * @param first_index 
         * @param inserted_index 
         */
        template<typename C>
        void insert_before_matching(T&& value, C callback, size_t first_index = 0, size_t* inserted_index = nullptr) {
            for (size_t i = first_index; i < size(); ++i) {
                if (callback(at(i))) {
                    insert(i, move(value));
                    if (inserted_index)
                        *inserted_index = i;
                    return;
                }
            }
            append(move(value));
            if (inserted_index)
                *inserted_index = size() - 1;
        }

        /**
         * @param other 
         * @return Vector& 
         */
        Vector& operator=(const Vector& other) {
            if (this != &other) {
                clear();
                ensure_capacity(other.size());
                TypedTransfer<T>::copy(data(), other.data(), other.size());
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
        Vector& operator=(const Vector<T, other_inline_capacity>& other) {
            clear();
            ensure_capacity(other.size());
            TypedTransfer<T>::copy(data(), other.data(), other.size());
            m_size = other.size();
            return *this;
        }

        /**
         * @param other 
         */
        void append(Vector&& other) {
            if (is_empty()) {
                *this = move(other);
                return;
            }
            auto other_size = other.size();
            Vector tmp = move(other);
            grow_capacity(size() + other_size);
            TypedTransfer<T>::move(data() + m_size, tmp.data(), other_size);
            m_size += other_size;
        }

        /**
         * @param other 
         */
        void append(const Vector& other) {
            grow_capacity(size() + other.size());
            TypedTransfer<T>::copy(data() + m_size, other.data(), other.size());
            m_size += other.m_size;
        }

        /**
         * @tparam Callback 
         * @param callback 
         * @return Optional<T> 
         */
        template<typename Callback>
        Optional<T> first_matching(Callback callback) {
            for (size_t i = 0; i < size(); ++i) {
                if (callback(at(i))) {
                    return at(i);
                }
            }
            return {};
        }

        /**
         * @tparam Callback 
         * @param callback 
         * @return Optional<T> 
         */
        template<typename Callback>
        Optional<T> last_matching(Callback callback) {
            for (ssize_t i = size() - 1; i >= 0; --i) {
                if (callback(at(i))) {
                    return at(i);
                }
            }
            return {};
        }

        /**
         * @tparam Callback 
         * @param callback 
         * @return true 
         * @return false 
         */
        template<typename Callback>
        bool remove_first_matching(Callback callback) {
            for (size_t i = 0; i < size(); ++i) {
                if (callback(at(i))) {
                    remove(i);
                    return true;
                }
            }
            return false;
        }

        /**
         * @tparam Callback 
         * @param callback 
         */
        template<typename Callback>
        void remove_all_matching(Callback callback) {
            for (size_t i = 0; i < size();) {
                if (callback(at(i))) {
                    remove(i);
                } else {
                    ++i;
                }
            }
        }

        /**
         * @param value 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void unchecked_append(T&& value) {
            ASSERT((size() + 1) <= capacity());
            new (slot(m_size)) T(move(value));
            ++m_size;
        }

        /**
         * @param value 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void unchecked_append(const T& value) {
            unchecked_append(T(value));
        }

        /**
         * @tparam Args 
         * @param args 
         */
        template<class... Args>
        void empend(Args&&... args) {
            grow_capacity(m_size + 1);
            new (slot(m_size)) T(forward<Args>(args)...);
            ++m_size;
        }

        /**
         * @param value 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void append(T&& value) {
            grow_capacity(size() + 1);
            new (slot(m_size)) T(move(value));
            ++m_size;
        }

        /**
         * @param value 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void append(const T& value) {
            append(T(value));
        }

        /**
         * @param value 
         */
        void prepend(T&& value) {
            insert(0, move(value));
        }

        /**
         * @param value 
         */
        void prepend(const T& value) {
            insert(0, value);
        }
        
        /**
         * @param other 
         */
        void prepend(Vector&& other) {
            if (other.is_empty())
                return;

            if (is_empty()) {
                *this = move(other);
                return;
            }

            auto other_size = other.size();
            grow_capacity(size() + other_size);

            for (size_t i = size() + other_size - 1; i >= other.size(); --i) {
                new (slot(i)) T(move(at(i - other_size)));
                at(i - other_size).~T();
            }

            Vector tmp = move(other);
            TypedTransfer<T>::move(slot(0), tmp.data(), tmp.size());
            m_size += other_size;
        }

        /**
         * @param values 
         * @param count 
         */
        void prepend(const T* values, size_t count) {
            if (!count)
                return;
            grow_capacity(size() + count);
            TypedTransfer<T>::move(slot(count), slot(0), m_size);
            TypedTransfer<T>::copy(slot(0), values, count);
            m_size += count;
        }

        /**
         * @param values 
         * @param count 
         */
        void append(const T* values, size_t count) {
            if (!count)
                return;
            grow_capacity(size() + count);
            TypedTransfer<T>::copy(slot(m_size), values, count);
            m_size += count;
        }

        /**
         * @param needed_capacity 
         */
        void grow_capacity(size_t needed_capacity) {
            if (m_capacity >= needed_capacity)
                return;
            ensure_capacity(padded_capacity(needed_capacity));
        }

        /**
         * @param needed_capacity 
         */
        void ensure_capacity(size_t needed_capacity) {
            if (m_capacity >= needed_capacity)
                return;
            size_t new_capacity = needed_capacity;
            auto* new_buffer = (T*)kmalloc(new_capacity * sizeof(T));

            if constexpr (Traits<T>::is_trivial()) {
                TypedTransfer<T>::copy(new_buffer, data(), m_size);
            } else {
                for (size_t i = 0; i < m_size; ++i) {
                    new (&new_buffer[i]) T(move(at(i)));
                    at(i).~T();
                }
            }
            if (m_outline_buffer)
                kfree(m_outline_buffer);
            m_outline_buffer = new_buffer;
            m_capacity = new_capacity;
        }

        /**
         * @param new_size 
         * @param keep_capacity 
         */
        void shrink(size_t new_size, bool keep_capacity = false) {
            ASSERT(new_size <= size());
            if (new_size == size())
                return;

            if (!new_size) {
                if (keep_capacity)
                    clear_with_capacity();
                else
                    clear();
                return;
            }

            for (size_t i = new_size; i < size(); ++i)
                at(i).~T();
            m_size = new_size;
        }
        
        /**
         * @param new_size 
         * @param keep_capacity 
         */
        void resize(size_t new_size, bool keep_capacity = false) {
            if (new_size <= size())
                return shrink(new_size, keep_capacity);

            ensure_capacity(new_size);
            for (size_t i = size(); i < new_size; ++i)
                new (slot(i)) T;
            m_size = new_size;
        }

        /**
         * @param new_size 
         */
        void resize_and_keep_capacity(size_t new_size) {
            return resize(new_size, true);
        }

        using ConstIterator = SimpleIterator<const Vector, const T>;
        using Iterator = SimpleIterator<Vector, T>;
            
        /**
         * @return ConstIterator 
         */
        ConstIterator begin() const { 
            return ConstIterator::begin(*this); 
        }

        /**
         * @return Iterator 
         */
        Iterator begin() { 
            return Iterator::begin(*this); 
        }

        /**
         * @return ConstIterator 
         */
        ConstIterator end() const { 
            return ConstIterator::end(*this); 
        }   

        /**
         * @return Iterator 
         */
        Iterator end() { 
            return Iterator::end(*this); 
        }

        /**
         * @tparam Finder 
         * @param finder 
         * @return ConstIterator 
         */
        template<typename Finder>
        ConstIterator find(Finder finder) const {
            for (size_t i = 0; i < m_size; ++i) {
                if (finder(at(i)))
                    return ConstIterator(*this, i);
            }
            return end();
        }

        /**
         * @tparam Finder 
         * @param finder 
         * @return Iterator 
         */
        template<typename Finder>
        Iterator find(Finder finder) {
            for (size_t i = 0; i < m_size; ++i) {
                if (finder(at(i)))
                    return Iterator { *this, i };
            }
            return end();
        }

        /**
         * @param value 
         * @return ConstIterator 
         */
        ConstIterator find(const T& value) const {
            return find([&](auto& other) { return Traits<T>::equals(value, other); });
        }

        /**
         * @param value 
         * @return Iterator 
         */
        Iterator find(const T& value) {
            return find([&](auto& other) { return Traits<T>::equals(value, other); });
        }

        /**
         * @param value 
         * @return Optional<size_t> 
         */
        Optional<size_t> find_first_index(const T& value) {
            for (size_t i = 0; i < m_size; ++i) {
                if (Traits<T>::equals(value, at(i)))
                    return i;
            }
            return {};
        }

    private:
        void reset_capacity() {
            m_capacity = inline_capacity;
        }

        /**
         * @param capacity 
         * @return size_t 
         */
        static size_t padded_capacity(size_t capacity) {
            return max(static_cast<size_t>(4), capacity + (capacity / 4) + 4);
        }

        /**
         * @param i 
         * @return T* 
         */
        T* slot(size_t i) { 
            return &data()[i]; 
        }

        /**
         * @param i 
         * @return const T* 
         */
        const T* slot(size_t i) const { 
            return &data()[i]; 
        }

        /**
         * @return T* 
         */
        T* inline_buffer() {
            static_assert(inline_capacity > 0);
            return reinterpret_cast<T*>(m_inline_buffer_storage);
        }   

        /**
         * @return const T* 
         */
        const T* inline_buffer() const {
            static_assert(inline_capacity > 0);
            return reinterpret_cast<const T*>(m_inline_buffer_storage);
        }

        size_t m_size { 0 };
        size_t m_capacity { 0 };

        alignas(T) unsigned char m_inline_buffer_storage[sizeof(T) * inline_capacity];
        T* m_outline_buffer { nullptr };
    };

}

using Mods::Vector;
