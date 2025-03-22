/**
 * @file fixedarray.h
 * @author Krisna Pranav
 * @brief Fixed Array
 * @version 6.0
 * @date 2025-02-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/error.h>
#include <mods/iterator.h>
#include <mods/span.h>
#include <mods/kmalloc.h>
#include <initializer_list>

namespace Mods 
{

    /**
     * @tparam T 
     */
    template<typename T>
    class FixedArray 
    {
    public:
        /**
         * @brief Construct a new Fixed Array object
         * 
         */
        FixedArray() = default;

        /**
         * @param initializer 
         * @return ErrorOr<FixedArray<T>> 
         */
        static ErrorOr<FixedArray<T>> create(std::initializer_list<T> initializer)
        {
            auto array = TRY(create(initializer.size()));
            auto it = initializer.begin();
            for (size_t i = 0; i < array.size(); ++i) {
                array[i] = move(*it);
                ++it;
            }
            return array;
        }

        /**
         * @param size 
         * @return ErrorOr<FixedArray<T>> 
         */
        static ErrorOr<FixedArray<T>> create(size_t size)
        {
            if (size == 0)
                return FixedArray<T>();
            auto* elements = reinterpret_cast<T*>(kmalloc(storage_allocation_size(size)));
            if (!elements)
                return Error::from_errno(ENOMEM);
            for (size_t i = 0; i < size; ++i)
                new (&elements[i]) T();
            return FixedArray<T>(size, elements);
        }

        /**
         * @param size 
         * @return FixedArray<T> 
         */
        static FixedArray<T> must_create_but_fixme_should_propagate_errors(size_t size)
        {
            return MUST(create(size));
        }

        /**
         * @tparam N 
         * @return ErrorOr<FixedArray<T>> 
         */
        template<size_t N>
        static ErrorOr<FixedArray<T>> create(T (&&array)[N])
        {
            return create(Span(array, N));
        }

        /**
         * @tparam U 
         * @param span 
         * @return ErrorOr<FixedArray<T>> 
         */
        template<typename U>
        static ErrorOr<FixedArray<T>> create(Span<U> span)
        {
            if (span.size() == 0)
                return FixedArray<T>();
            auto* elements = reinterpret_cast<T*>(kmalloc(storage_allocation_size(span.size())));
            if (!elements)
                return Error::from_errno(ENOMEM);
            for (size_t i = 0; i < span.size(); ++i)
                new (&elements[i]) T(span[i]);
            return FixedArray<T>(span.size(), elements);
        }

        /**
         * @return ErrorOr<FixedArray<T>> 
         */
        ErrorOr<FixedArray<T>> clone() const
        {
            return create(span());
        }

        /**
         * @brief Construct a new Fixed Array object
         * 
         */
        FixedArray(FixedArray<T> const&) = delete;
        FixedArray<T>& operator=(FixedArray<T> const&) = delete;

        /**
         * @param other 
         */
        FixedArray(FixedArray<T>&& other)
            : m_size(exchange(other.m_size, 0))
            , m_elements(exchange(other.m_elements, nullptr))
        {
        }

        /**
         * @param other 
         * @return FixedArray<T>& 
         */
        FixedArray<T>& operator=(FixedArray<T>&& other)
        {
            if (this != &other) {
                this->~FixedArray();
                new (this) FixedArray<T>(move(other));
            }
            return *this;
        }

        /**
         * @brief Destroy the Fixed Array object
         * 
         */
        ~FixedArray()
        {
            if (!m_elements)
                return;
            for (size_t i = 0; i < m_size; ++i)
                m_elements[i].~T();
            kfree_sized(m_elements, storage_allocation_size(m_size));
            m_elements = nullptr;
        }

        /**
         * @return size_t 
         */
        size_t size() const 
        { 
            return m_size; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const 
        { 
            return size() == 0; 
        }

        /**
         * @return T* 
         */
        T* data() 
        { 
            return m_elements; 
        }

        /**
         * @return T const*
         */
        T const* data() const 
        { 
            return m_elements; 
        }

        /**
         * @param index 
         * @return T& 
         */
        T& at(size_t index)
        {
            VERIFY(index < m_size);
            return m_elements[index];
        }

        /**
         * @param index 
         * @return T& 
         */
        T& unchecked_at(size_t index)
        {
            return m_elements[index];
        }

        /**
         * @param index 
         * @return T const& 
         */
        T const& at(size_t index) const
        {
            VERIFY(index < m_size);
            return m_elements[index];
        }

        /**
         * @param index 
         * @return T& 
         */
        T& operator[](size_t index)
        {
            return at(index);
        }

        /**
         * @param index 
         * @return T const& 
         */
        T const& operator[](size_t index) const
        {
            return at(index);
        }

        /**
         * @param value 
         * @return true 
         * @return false 
         */
        bool contains_slow(T const& value) const
        {
            if (!m_elements)
                return false;
            for (size_t i = 0; i < m_size; ++i) {
                if (m_elements[i] == value)
                    return true;
            }
            return false;
        }

        /**
         * @param other 
         */
        void swap(FixedArray<T>& other)
        {
            Mods::swap(m_size, other.m_size);
            Mods::swap(m_elements, other.m_elements);
        }

        /**
         * @param value 
         */
        void fill_with(T const& value)
        {
            if (!m_elements)
                return;
            for (size_t i = 0; i < m_size; ++i)
                m_elements[i] = value;
        }

        using Iterator = SimpleIterator<FixedArray, T>;
        using ConstIterator = SimpleIterator<FixedArray const, T const>;

        /**
         * @return Iterator 
         */
        Iterator begin() 
        { 
            return Iterator::begin(*this); 
        }

        /**
         * @return ConstIterator 
         */
        ConstIterator begin() const 
        { 
            return ConstIterator::begin(*this); 
        }

        /**
         * @return Iterator 
         */
        Iterator end() 
        { 
            return Iterator::end(*this); 
        }

        /**
         * @return ConstIterator 
         */
        ConstIterator end() const 
        { 
            return ConstIterator::end(*this); 
        }

        /**
         * @return Span<T> 
         */
        Span<T> span() 
        { 
            return { data(), size() }; 
        }

        /**
         * @return ReadonlySpan<T> 
         */
        ReadonlySpan<T> span() const 
        { 
            return { data(), size() }; 
        }

    private:
        /**
         * @param size 
         * @return size_t 
         */
        static size_t storage_allocation_size(size_t size)
        {
            return size * sizeof(T);
        }

        /**
         * @brief Construct a new Fixed Array object
         * 
         * @param size 
         * @param elements 
         */
        FixedArray(size_t size, T* elements)
            : m_size(size)
            , m_elements(elements)
        {
        }

        size_t m_size { 0 };
        T* m_elements { nullptr };
    }; // class FixedArray

} // namespace Mods

#if USING_MODS_GLOBALLY
using Mods::FixedArray;
#endif
