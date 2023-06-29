/**
 * @file span.h
 * @author Krisna Pranav
 * @brief span
 * @version 1.0
 * @date 2023-06-29
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "assertions.h"
#include "iterator.h"
#include "typedtransfer.h"
#include "types.h"

namespace Detail {
    template<typename T>
    class Span {
    public:
        ALWAYS_INLINE constexpr Span() = default;

        ALWAYS_INLINE constexpr Span(T* values, size_t size)
            : m_values(values)
            , m_size(size)
        {}

    protected:
        T* m_values { nullptr };
        size_t m_size { 0 };
    };

    template<>
    class Span<u8> {
    public:
        ALWAYS_INLINE constexpr Span() = default;

        ALWAYS_INLINE constexpr Span(u8* values, size_t size)
            : m_values(values)
            , m_size(size)
        {
        }

    protected:
        u8* m_values { nullptr };
        size_t m_size { 0 };
    };

    template<typename T>
    class Span: public Detail::Span<T> {
    public:
        using Detail::Span<T>::Span;

        ALWAYS_INLINE constexpr Span(std::nullptr_t) 
            : Span()
        {}  

        /**
         * @brief Span
         * 
         * @param other 
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr Span(const Span& other)
            : Span(other.m_values, other.m_size)
        {}

        /**
         * @brief T*
         * 
         * @return ALWAYS_INLINE constexpr const* 
         */
        ALWAYS_INLINE constexpr const T* data() const {
            return this->m_values;
        }

        /**
         * @brief data
         * 
         * @return ALWAYS_INLINE constexpr* 
         */
        ALWAYS_INLINE constexpr T* data() {
            return this->m_values;
        }

    };

}

using Mods::Bytes;
using Mods::ReadonlyBytes;