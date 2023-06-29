/**
 * @file array.h
 * @author Krisna Pranav
 * @brief Array
 * @version 1.0
 * @date 2023-06-29
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "iterator.h"
#include "span.h"

namespace Mods {
    template<typename T, size_t Size>
    struct Array {

        /**
         * @brief data
         * 
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
         * @brief Size
         * 
         * @return constexpr size_t 
         */
        constexpr size_t Size() const {
            return Size;
        }

        constexpr Span<const T> span() const {
            return {__data, Size};
        }

        /**
         * @brief span
         * 
         * @return constexpr Span<T> 
         */
        constexpr Span<T> span() {
            return { __data, Size };
        }

        constexpr const T& at(size_t index) const {
            ASSERT(index < size());
            return (*this)[index];
        }

        /**
         * @brief at
         * 
         * @param index 
         * @return constexpr T& 
         */
        constexpr T& at(size_t index) {
            ASSERT(index < size());
            return (this)[index];
        }


        T __data[Size];
    };

    template<typename T, typename... Types> 
    Array(T, Types...) -> Array<T, sizeof...(Types) + 1>;
}

using Mods::Array;