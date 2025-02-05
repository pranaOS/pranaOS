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
        static ErrorOr<FixedArray<T>> try_create(std::initializer_list<T> initializer)
        {
            auto array = TRY(try_create(initializer.size()));
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
        static ErrorOr<FixedArray<T>> try_create(size_t size)
        {
            if (size == 0)
                return FixedArray<T>();

            T* elements = static_cast<T*>(kmalloc_array(size, sizeof(T)));

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
            return MUST(try_create(size));
        }

        /**
         * @tparam N 
         * @return ErrorOr<FixedArray<T>> 
         */
        template<size_t N>
        static ErrorOr<FixedArray<T>> try_create(T (&&array)[N])
        {
            if (N == 0)
                return FixedArray<T>();

            T* elements = static_cast<T*>(kmalloc_array(N, sizeof(T)));

            if (!elements)
                return Error::from_errno(ENOMEM);

            for (size_t i = 0; i < N; ++i)
                new (&elements[i]) T(move(array[i]));
                
            return FixedArray<T>(N, elements);
        }

        /**
         * @tparam U 
         * @param span 
         * @return ErrorOr<FixedArray<T>> 
         */
        template<typename U>
        static ErrorOr<FixedArray<T>> try_create(Span<U> span)
        {
            if (span.size() == 0)
                return FixedArray<T>();

            T* elements = static_cast<T*>(kmalloc_array(span.size(), sizeof(T)));

            if (!elements)
                return Error::from_errno(ENOMEM);

            for (size_t i = 0; i < span.size(); ++i)
                new (&elements[i]) T(span[i]);

            return FixedArray<T>(span.size(), elements);
        }

        /**
         * @return ErrorOr<FixedArray<T>> 
         */
        ErrorOr<FixedArray<T>> try_clone() const
        {
            if (m_size == 0)
                return FixedArray<T>();

            T* elements = static_cast<T*>(kmalloc_array(m_size, sizeof(T)));

            if (!elements)
                return Error::from_errno(ENOMEM);

            for (size_t i = 0; i < m_size; ++i)
                new (&elements[i]) T(m_elements[i]);

            return FixedArray<T>(m_size, elements);
        }

        /**
         * @return FixedArray<T> 
         */
        FixedArray<T> must_clone_but_fixme_should_propagate_errors() const
        {
            return MUST(try_clone());
        }

        /**
         * @brief Construct a new Fixed Array object
         * 
         */
        FixedArray(FixedArray<T> const&) = delete;

        /**
         * @return FixedArray<T>& 
         */
        FixedArray<T>& operator=(FixedArray<T> const&) = delete;

        /**
         * @brief Construct a new Fixed Array object
         * 
         * @param other 
         */
        FixedArray(FixedArray<T>&& other)
            : m_size(other.m_size)
            , m_elements(other.m_elements)
        {
            other.m_size = 0;
            other.m_elements = nullptr;
        }
        
        FixedArray<T>& operator=(FixedArray<T>&&) = delete;

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

            kfree_sized(m_elements, sizeof(T) * m_size);
            
            m_size = 0;
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
            return m_size == 0; 
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
            ::swap(m_size, other.m_size);
            ::swap(m_elements, other.m_elements);
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
         * @return Span<T const> 
         */
        Span<T const> span() const 
        { 
            return { data(), size() }; 
        }

    private:
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

using Mods::FixedArray;