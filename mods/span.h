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

namespace Mods {
    namespace Detail { // Start 

        template<typename T>
        class Span {
        public:
            ALWAYS_INLINE constexpr Span() = default;

            ALWAYS_INLINE constexpr Span(T* values, size_t size)
                : m_values(values)
                , m_size(size)
            {
            }

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
            
            ALWAYS_INLINE Span(void* values, size_t size)
                : m_values(reinterpret_cast<u8*>(values))
                , m_size(size)
            {
            }

        protected:
            u8* m_values { nullptr };
            size_t m_size { 0 };
        };

        template<>
        class Span<const u8> {
        public:
            ALWAYS_INLINE constexpr Span() = default;

            ALWAYS_INLINE constexpr Span(const u8* values, size_t size)
                : m_values(values)
                , m_size(size)
            {
            }

            ALWAYS_INLINE Span(const void* values, size_t size)
                : m_values(reinterpret_cast<const u8*>(values))
                , m_size(size)
            {
            }

            ALWAYS_INLINE Span(const char* values, size_t size)
                : m_values(reinterpret_cast<const u8*>(values))
                , m_size(size)
            {
            }

        protected:
            const u8* m_values { nullptr };
            size_t m_size { 0 };
        };

    } // End

    template<typename T>
    class Span : public Detail::Span<T> {
    public:
        using Detail::Span<T>::Span;

        ALWAYS_INLINE constexpr Span(std::nullptr_t)
            : Span()
        {
        }

        ALWAYS_INLINE constexpr Span(const Span& other)
            : Span(other.m_values, other.m_size)
        {
        }
        
        /**
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

        using ConstIterator = SimpleIterator<const Span, const T>;
        using Iterator = SimpleIterator<Span, T>;

        /**
         * @brief begin
         * 
         * @return constexpr ConstIterator 
         */
        constexpr ConstIterator begin() const { 
            return ConstIterator::begin(*this); 
        }

        /**
         * @brief begin
         * 
         * @return constexpr Iterator 
         */
        constexpr Iterator begin() { 
            return Iterator::begin(*this); 
        }

        /**
         * @return constexpr ConstIterator 
         */
        constexpr ConstIterator end() const { 
            return ConstIterator::end(*this); 
        }

        /**
         * @brief end
         * 
         * @return constexpr Iterator 
         */
        constexpr Iterator end() { 
            return Iterator::end(*this); 
        }

        /**
         * @brief size
         * 
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr size_t size() const { 
            return this->m_size; 
        }

        /**
         * @brief is_empty
         * 
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr bool is_empty() const { 
            return this->m_size == 0; 
        }

        /**
         * @brief slice
         * 
         * @param start 
         * @param length 
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr Span slice(size_t start, size_t length) const {
            ASSERT(start + length <= size());
            return { this->m_values + start, length };
        }

        /**
         * @brief slice
         * 
         * @param start 
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr Span slice(size_t start) const {
            ASSERT(start <= size());
            return { this->m_values + start, size() - start };
        }

        /**
         * @brief trim
         * 
         * @param length 
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr Span trim(size_t length) const {
            return { this->m_values, min(size(), length) };
        }

        /**
         * @brief offset
         * 
         * @param start 
         * @return ALWAYS_INLINE constexpr* 
         */
        ALWAYS_INLINE constexpr T* offset(size_t start) const {
            ASSERT(start < this->m_size);
            return this->m_values + start;
        }

        /**
         * @brief copy_to
         * 
         * @param other 
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr size_t copy_to(Span<typename RemoveConst<T>::Type> other) const {
            ASSERT(other.size() >= size());
            return TypedTransfer<typename RemoveConst<T>::Type>::copy(other.data(), data(), size());
        }

        /**
         * @brief copy_trimmed_to
         * 
         * @param other 
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr size_t copy_trimmed_to(Span<typename RemoveConst<T>::Type> other) const {
            const auto count = min(size(), other.size());
            return TypedTransfer<typename RemoveConst<T>::Type>::copy(other.data(), data(), count);
        }

        /**
         * @brief fill
         * 
         * @param value 
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr size_t fill(const T& value) {
            for (size_t idx = 0; idx < size(); ++idx)
                data()[idx] = value;

            return size();
        }

        /**
         * @brief contains_slow
         * 
         * @param value 
         * @return true 
         * @return false 
         */
        bool constexpr contains_slow(const T& value) const {
            for (size_t i = 0; i < size(); ++i) {
                if (at(i) == value)
                    return true;
            }
            return false;
        }

        /**
         * @param index 
         * @return ALWAYS_INLINE constexpr const& 
         */
        ALWAYS_INLINE constexpr const T& at(size_t index) const {
            ASSERT(index < this->m_size);
            return this->m_values[index];
        }

        /**
         * @brief at
         * 
         * @param index 
         * @return ALWAYS_INLINE constexpr& 
         */
        ALWAYS_INLINE constexpr T& at(size_t index) {
            ASSERT(index < this->m_size);
            return this->m_values[index];
        }   

        /**
         * @param index 
         * @return ALWAYS_INLINE constexpr& 
         */
        ALWAYS_INLINE constexpr T& operator[](size_t index) const {
            return this->m_values[index];
        }

        /**
         * @param index 
         * @return ALWAYS_INLINE constexpr& 
         */
        ALWAYS_INLINE constexpr T& operator[](size_t index) {
            return this->m_values[index];
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE constexpr& 
         */
        ALWAYS_INLINE constexpr Span& operator=(const Span<T>& other) {
            this->m_size = other.m_size;
            this->m_values = other.m_values;
            return *this;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator==(Span<const T> other) const {
            if (size() != other.size())
                return false;

            return TypedTransfer<T>::compare(data(), other.data(), size());
        }

        /**
         * @brief Span
         * 
         * @return Span<const T> 
         */
        ALWAYS_INLINE constexpr operator Span<const T>() const {
            return { data(), size() };
        }
    };

    using ReadonlyBytes = Span<const u8>;
    using Bytes = Span<u8>;

}

using Mods::Bytes;
using Mods::ReadonlyBytes;
using Mods::Span;