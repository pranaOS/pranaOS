/**
 * @file array.h
 * @author Krisna Pranav
 * @brief Array
 * @version 6.0
 * @date 2023-06-29
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/iterator.h>
#include <mods/span.h>

namespace Mods
{
    /**
     * @tparam T 
     * @tparam Size 
     */
    template<typename T, size_t Size>
    struct Array 
    {
        using ValueType = T;

        /**
         * @return constexpr T const* 
         */
        [[nodiscard]] constexpr T const* data() const 
        { 
            return __data; 
        }

        /**
         * @return constexpr T* 
         */
        [[nodiscard]] constexpr T* data() 
        { 
            return __data; 
        }

        /**
         * @return constexpr size_t 
         */
        [[nodiscard]] constexpr size_t size() const 
        { 
            return Size; 
        }

        /**
         * @return constexpr Span<T const> 
         */
        [[nodiscard]] constexpr Span<T const> span() const 
        { 
            return { __data, Size }; 
        }

        /**
         * @return constexpr Span<T> 
         */
        [[nodiscard]] constexpr Span<T> span() 
        { 
            return { __data, Size }; 
        }

        /**
         * @param index 
         * @return constexpr T const& 
         */
        [[nodiscard]] constexpr T const& at(size_t index) const
        {
            VERIFY(index < size());
            return __data[index];
        }

        /**
         * @param index 
         * @return constexpr T& 
         */
        [[nodiscard]] constexpr T& at(size_t index)
        {
            VERIFY(index < size());
            return __data[index];
        }

        /**
         * @return constexpr T const& 
         */
        [[nodiscard]] constexpr T const& first() const 
        { 
            return at(0); 
        }

        /**
         * @return constexpr T& 
         */
        [[nodiscard]] constexpr T& first() 
        { 
            return at(0); 
        }

        [[nodiscard]] constexpr T const& last() const requires(Size > 0) 
        { 
            return at(Size - 1); 
        }
        
        [[nodiscard]] constexpr T& last() requires(Size > 0) 
        { 
            return at(Size - 1); 
        }

        [[nodiscard]] constexpr bool is_empty() const 
        { 
            return size() == 0; 
        }

        /**
         * @param index 
         * @return constexpr T const& 
         */
        [[nodiscard]] constexpr T const& operator[](size_t index) const 
        { 
            return at(index); 
        }

        /**
         * @param index 
         * @return constexpr T& 
         */
        [[nodiscard]] constexpr T& operator[](size_t index) 
        { 
            return at(index); 
        }

        /**
         * @tparam T2 
         * @tparam Size2 
         * @param other 
         * @return true 
         * @return false 
         */
        template<typename T2, size_t Size2>
        [[nodiscard]] constexpr bool operator==(Array<T2, Size2> const& other) const 
        { 
            return span() == other.span(); 
        }

        using ConstIterator = SimpleIterator<Array const, T const>;
        using Iterator = SimpleIterator<Array, T>;

        /**
         * @return constexpr ConstIterator 
         */
        [[nodiscard]] constexpr ConstIterator begin() const 
        { 
            return ConstIterator::begin(*this); 
        }
            
        /**
         * @return constexpr Iterator 
         */
        [[nodiscard]] constexpr Iterator begin() 
        { 
            return Iterator::begin(*this); 
        }

        /**
         * @return constexpr ConstIterator 
         */
        [[nodiscard]] constexpr ConstIterator end() const 
        { 
            return ConstIterator::end(*this); 
        }

        /**
         * @return constexpr Iterator 
         */
        [[nodiscard]] constexpr Iterator end() 
        { 
            return Iterator::end(*this); 
        }

        /**
         * @return Span<T const> 
         */
        [[nodiscard]] constexpr operator Span<T const>() const 
        { 
            return span(); 
        }

        /**
         * @return Span<T> 
         */
        [[nodiscard]] constexpr operator Span<T>() 
        { 
            return span(); 
        }

        /**
         * @param value 
         * @return constexpr size_t 
         */
        constexpr size_t fill(T const& value)
        {
            for (size_t idx = 0; idx < Size; ++idx) 
                __data[idx] = value;

            return Size;
        }

        [[nodiscard]] constexpr T max() const requires(requires(T x, T y) { x < y; })
        {
            static_assert(Size > 0, "No values to max() over");

            T value = __data[0];
            for (size_t i = 1; i < Size; ++i)
                value = Mods::max(__data[i], value);
            return value;
        }
    
        [[nodiscard]] constexpr T min() const requires(requires(T x, T y) { x > y; })
        {
            static_assert(Size > 0, "No values to min() over");

            T value = __data[0];
            for (size_t i = 1; i < Size; ++i)
                value = Mods::min(__data[i], value);
            return value;
        }

        T __data[Size];
    };

    /**
     * @tparam T 
     * @tparam Types 
     */
    template<typename T, typename... Types>
    Array(T, Types...) -> Array<T, sizeof...(Types) + 1>;

    namespace Detail 
    {
        /**
         * @tparam T 
         * @tparam Is 
         * @param offset 
         * @return Array<T, sizeof...(Is)> 
         */
        template<typename T, size_t... Is>
        constexpr auto integer_sequence_generate_array([[maybe_unused]] T const offset, IntegerSequence<T, Is...>) -> Array<T, sizeof...(Is)>
        {
            return { { (offset + Is)... } };
        }
    } // namespace Detail   

    /**
     * @tparam T 
     * @tparam N 
     * @param offset 
     * @return constexpr auto 
     */
    template<typename T, T N>
    constexpr static auto iota_array(T const offset = {})
    {
        static_assert(N >= T {}, "Negative sizes not allowed in iota_array()");
        return Detail::integer_sequence_generate_array<T>(offset, MakeIntegerSequence<T, N>());
    }

} // namespace Mods

using Mods::Array;
using Mods::iota_array;