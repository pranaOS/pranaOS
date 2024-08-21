/**
 * @file array.h
 * @author Krisna Pranav
 * @brief Array
 * @version 6.0
 * @date 2023-06-29
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "iterator.h"
#include "span.h"


namespace Mods {

    template<typename T, size_t Size>
    struct Array {
        /**
         * @return constexpr const T* 
         */
        constexpr const T* data() const { 
            return __data; 
        }

        /**
         * @return constexpr T* 
         */
        constexpr T* data() { 
            return __data; 
        }

        /**
         * @brief size
         * 
         * @return constexpr size_t 
         */
        constexpr size_t size() const { 
            return Size; 
        }

        /**
         * @brief span
         * 
         * @return constexpr Span<const T> 
         */
        constexpr Span<const T> span() const { 
            return { __data, Size }; 
        }

        /**
         * @return constexpr Span<T> 
         */
        constexpr Span<T> span() { 
            return { __data, Size }; 
        }

        /**
         * @brief at
         * 
         * @param index 
         * @return constexpr const T& 
         */
        constexpr const T& at(size_t index) const {
            ASSERT(index < size());
            return (*this)[index];
        }

        /**
         * @param index 
         * @return constexpr T& 
         */
        constexpr T& at(size_t index) {
            ASSERT(index < size());
            return (*this)[index];
        }

        /**
         * @brief front
         * 
         * @return constexpr const T& 
         */
        constexpr const T& front() const { 
            return at(0); 
        }

        /**
         * @return constexpr T& 
         */
        constexpr T& front() { 
            return at(0); 
        }

        /**
         * @brief back
         * 
         * @return constexpr const T& 
         */
        constexpr const T& back() const { 
            return at(max(1, size()) - 1); 
        }

        /**
         * @return constexpr T& 
         */
        constexpr T& back() { 
            return at(max(1, size()) - 1); 
        }

        /**
         * @brief is_empty
         * 
         * @return true 
         * @return false 
         */
        constexpr bool is_empty() const { 
            return size() == 0; 
        }

        /**
         * @param index 
         * @return constexpr const T& 
         */
        constexpr const T& operator[](size_t index) const { 
            return __data[index]; 
        }

        /**
         * @param index 
         * @return constexpr T& 
         */
        constexpr T& operator[](size_t index) { 
            return __data[index]; 
        }

        using ConstIterator = SimpleIterator<const Array, const T>;
        using Iterator = SimpleIterator<Array, T>;
        
        /**
         * @brief begin
         * 
         * @return constexpr ConstIterator 
         */
        constexpr ConstIterator begin() const { 
            return ConstIterator::begin(*this); 
        }
        
        /**
         * @return constexpr Iterator 
         */
        constexpr Iterator begin() { 
            return Iterator::begin(*this); 
        }

        /**
         * @brief end
         * 
         * @return constexpr ConstIterator 
         */
        constexpr ConstIterator end() const { 
            return ConstIterator::end(*this); 
        }

        /**
         * @return constexpr Iterator 
         */
        constexpr Iterator end() { 
            return Iterator::end(*this); 
        }

        /**
         * @brief Span
         * 
         * @return Span<const T> 
         */
        constexpr operator Span<const T>() const { 
            return span(); 
        }

        /**
         * @return Span<T> 
         */
        constexpr operator Span<T>() { 
            return span(); 
        }

        T __data[Size];
    };

    template<typename T, typename... Types>
    Array(T, Types...) -> Array<T, sizeof...(Types) + 1>;

}

using Mods::Array;
