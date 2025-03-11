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
#include <mods/optional.h>
#include <mods/span.h>
#include <mods/stdlibextra.h>
#include <mods/typedtransfer.h>

namespace Mods 
{

    namespace Detail 
    {
        /**
         * @tparam T 
         */
        template<typename T>
        struct EmptyArrayStorage {
            T& operator[](size_t) const { VERIFY_NOT_REACHED(); }
            constexpr operator T*() const { return nullptr; }
        };
    } // namespace Detail 

    /**
     * @tparam T 
     * @tparam Size 
     */
    template<typename T, size_t Size>
    struct Array {
        using ValueType = T;

        /**
         * @param span 
         * @return Array 
         */
        static Array from_span(ReadonlySpan<T> span)
        {
            Array array;
            VERIFY(span.size() == Size);
            TypedTransfer<T>::copy(array.data(), span.data(), Size);
            return array;
        }

        /**
         * @param value 
         * @return constexpr Array 
         */
        static constexpr Array from_repeated_value(T const& value)
        {
            Array array;
            array.fill(value);
            return array;
        }

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
         * @return constexpr ReadonlySpan<T> 
         */
        [[nodiscard]] constexpr ReadonlySpan<T> span() const 
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

        [[nodiscard]] constexpr T const& last() const
        requires(Size > 0)
        {
            return at(Size - 1);
        }
        [[nodiscard]] constexpr T& last()
        requires(Size > 0)
        {
            return at(Size - 1);
        }

        /**
         * @return true 
         * @return false 
         */
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
        [[nodiscard]] constexpr bool operator==(Array<T2, Size2> const& other) const { return span() == other.span(); }

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
         * @return ReadonlySpan<T> 
         */
        [[nodiscard]] constexpr operator ReadonlySpan<T>() const 
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

        [[nodiscard]] constexpr T max() const
        requires(requires(T x, T y) { x < y; })
        {
            static_assert(Size > 0, "No values to max() over");

            T value = __data[0];
            for (size_t i = 1; i < Size; ++i)
                value = Mods::max(__data[i], value);
            return value;
        }

        [[nodiscard]] constexpr T min() const
        requires(requires(T x, T y) { x > y; })
        {
            static_assert(Size > 0, "No values to min() over");

            T value = __data[0];
            for (size_t i = 1; i < Size; ++i)
                value = Mods::min(__data[i], value);
            return value;
        }

        /**
         * @param value 
         * @return true 
         * @return false 
         */
        bool contains_slow(T const& value) const
        {
            return first_index_of(value).has_value();
        }

        /**
         * @param value 
         * @return Optional<size_t> 
         */
        Optional<size_t> first_index_of(T const& value) const
        {
            for (size_t i = 0; i < Size; ++i) {
                if (__data[i] == value)
                    return i;
            }
            return {};
        }

        Conditional<Size == 0, Detail::EmptyArrayStorage<T>, T[Size]> __data;
    }; // struct Array

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
    constexpr auto iota_array(T const offset = {})
    {
        static_assert(N >= T {}, "Negative sizes not allowed in iota_array()");
        return Detail::integer_sequence_generate_array<T>(offset, MakeIntegerSequence<T, N>());
    }

    namespace Detail 
    {
        template<typename T, size_t N, size_t... Is>
        constexpr auto to_array_impl(T (&&a)[N], IndexSequence<Is...>) -> Array<T, sizeof...(Is)>
        {
            return { { a[Is]... } };
        }
    } // namespace Detail 

    /**
     * @tparam T 
     * @tparam N 
     * @return constexpr auto 
     */
    template<typename T, size_t N>
    constexpr auto to_array(T (&&a)[N])
    {
        return Detail::to_array_impl(move(a), MakeIndexSequence<N>());
    }

} // namespace Mods

#if USING_MODS_GLOBALLY
using Mods::Array;
using Mods::iota_array;
using Mods::to_array;
#endif
