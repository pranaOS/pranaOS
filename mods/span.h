/**
 * @file span.h
 * @author Krisna Pranav
 * @brief span
 * @version 6.0
 * @date 2023-06-29
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "assertions.h"
#include "iterator.h"
#include "typedtransfer.h"
#include "types.h"

namespace Mods 
{
    namespace Detail 
    {

        template<typename T>
        class Span 
        {
        public:
            /**
             * @return ALWAYS_INLINE constexpr 
             */
            ALWAYS_INLINE constexpr Span() = default;

            /**
             * @param values 
             * @param size 
             * @return ALWAYS_INLINE constexpr 
             */
            ALWAYS_INLINE constexpr Span(T* values, size_t size)
                : m_values(values)
                , m_size(size)
            { }

            /**
             * @tparam size 
             * @return ALWAYS_INLINE constexpr 
             */
            template<size_t size>
            ALWAYS_INLINE constexpr Span(T (&values)[size])
                : m_values(values)
                , m_size(size)
            { }

        protected:
            T* m_values { nullptr };
            size_t m_size { 0 };
        }; // class Span


        template<>
        class Span<u8> 
        {
        public:
            /**
             * @return ALWAYS_INLINE constexpr 
             */
            ALWAYS_INLINE constexpr Span() = default;

            /**
             * @param values 
             * @param size 
             * @return ALWAYS_INLINE constexpr 
             */
            ALWAYS_INLINE constexpr Span(u8* values, size_t size)
                : m_values(values)
                , m_size(size)
            { }

            /**
             * @param values 
             * @param size 
             * @return ALWAYS_INLINE 
             */
            ALWAYS_INLINE Span(void* values, size_t size)
                : m_values(reinterpret_cast<u8*>(values))
                , m_size(size)
            { }

        protected:
            u8* m_values { nullptr };
            size_t m_size { 0 };
        }; // class Span 

        template<>
        class Span<u8 const> 
        {
        public:
            ALWAYS_INLINE constexpr Span() = default;

            /**
             * @param values 
             * @param size 
             * @return ALWAYS_INLINE constexpr 
             */
            ALWAYS_INLINE constexpr Span(u8 const* values, size_t size)
                : m_values(values)
                , m_size(size)
            { }

            /**
             * @param values 
             * @param size 
             * @return ALWAYS_INLINE 
             */
            ALWAYS_INLINE Span(void const* values, size_t size)
                : m_values(reinterpret_cast<u8 const*>(values))
                , m_size(size)
            { }

            /**
             * @param values 
             * @param size 
             * @return ALWAYS_INLINE 
             */
            ALWAYS_INLINE Span(char const* values, size_t size)
                : m_values(reinterpret_cast<u8 const*>(values))
                , m_size(size)
            { }

        protected:
            u8 const* m_values { nullptr };
            size_t m_size { 0 };
        }; // class Span<u8>
    } // namespace Detail

    /**
     * @tparam T 
     */
    template<typename T>
    class Span : public Detail::Span<T> 
    {
    public:
        using Detail::Span<T>::Span;

        constexpr Span() = default;

        /**
         * @return ALWAYS_INLINE constexpr const* 
         */
        [[nodiscard]] ALWAYS_INLINE constexpr T const* data() const 
        { 
            return this->m_values; 
        }

        /**
         * @return ALWAYS_INLINE constexpr* 
         */
        [[nodiscard]] ALWAYS_INLINE constexpr T* data() 
        { 
            return this->m_values; 
        }

        /**
         * @param offset 
         * @return ALWAYS_INLINE constexpr const* 
         */
        [[nodiscard]] ALWAYS_INLINE constexpr T const* offset_pointer(size_t offset) const 
        { 
            return this->m_values + offset; 
        }

        /**
         * @param offset 
         * @return ALWAYS_INLINE constexpr* 
         */
        [[nodiscard]] ALWAYS_INLINE constexpr T* offset_pointer(size_t offset) 
        { 
            return this->m_values + offset; 
        }

        using ConstIterator = SimpleIterator<Span const, T const>;
        using Iterator = SimpleIterator<Span, T>;

        /**
         * @return constexpr ConstIterator 
         */
        constexpr ConstIterator begin() const 
        { 
            return ConstIterator::begin(*this); 
        }

        /**
         * @return constexpr Iterator 
         */
        constexpr Iterator begin() 
        { 
            return Iterator::begin(*this); 
        }

        /**
         * @return constexpr ConstIterator 
         */
        constexpr ConstIterator end() const 
        { 
            return ConstIterator::end(*this); 
        }

        /**
         * @return constexpr Iterator 
         */
        constexpr Iterator end() 
        { 
            return Iterator::end(*this); 
        }

        /**
         * @return ALWAYS_INLINE constexpr 
         */
        [[nodiscard]] ALWAYS_INLINE constexpr size_t size() const { return this->m_size; }

        /**
         * @return ALWAYS_INLINE constexpr 
         */
        [[nodiscard]] ALWAYS_INLINE constexpr bool is_null() const 
        { 
            return this->m_values == nullptr; 
        }

        /**
         * @return * ALWAYS_INLINE constexpr 
         */
        [[nodiscard]] ALWAYS_INLINE constexpr bool is_empty() const 
        { 
            return this->m_size == 0;
        }

        /**
         * @param start 
         * @param length 
         * @return ALWAYS_INLINE constexpr 
         */
        [[nodiscard]] ALWAYS_INLINE constexpr Span slice(size_t start, size_t length) const
        {
            VERIFY(start + length <= size());
            return { this->m_values + start, length };
        }

        /**
         * @param start 
         * @return ALWAYS_INLINE constexpr 
         */
        [[nodiscard]] ALWAYS_INLINE constexpr Span slice(size_t start) const
        {
            VERIFY(start <= size());
            return { this->m_values + start, size() - start };
        }

        /**
         * @param count 
         * @return ALWAYS_INLINE constexpr 
         */
        [[nodiscard]] ALWAYS_INLINE constexpr Span slice_from_end(size_t count) const
        {
            VERIFY(count <= size());
            return { this->m_values + size() - count, count };
        }

        /**
         * @param length 
         * @return ALWAYS_INLINE constexpr 
         */
        [[nodiscard]] ALWAYS_INLINE constexpr Span trim(size_t length) const
        {
            return 
            { 
                this->m_values, min(size(), length) 
            };
        }

        /**
         * @param start 
         * @return * ALWAYS_INLINE constexpr* 
         */
        [[nodiscard]] ALWAYS_INLINE constexpr T* offset(size_t start) const
        {
            VERIFY(start < this->m_size);
            return this->m_values + start;
        }

        /**
         * @param offset 
         * @param data 
         * @param data_size 
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr void overwrite(size_t offset, void const* data, size_t data_size)
        {
            VERIFY(offset + data_size <= size());
            __builtin_memmove(this->data() + offset, data, data_size);
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr size_t copy_to(Span<RemoveConst<T>> other) const
        {
            VERIFY(other.size() >= size());
            return TypedTransfer<RemoveConst<T>>::copy(other.data(), data(), size());
        }

        /**
         * @param other 
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr size_t copy_trimmed_to(Span<RemoveConst<T>> other) const
        {
            auto const count = min(size(), other.size());
            return TypedTransfer<RemoveConst<T>>::copy(other.data(), data(), count);
        }

        /**
         * @param value 
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr size_t fill(T const& value)
        {
            for (size_t idx = 0; idx < size(); ++idx)
                data()[idx] = value;

            return size();
        }
        
        /**
         * @param value 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool constexpr contains_slow(T const& value) const
        {
            for (size_t i = 0; i < size(); ++i) {
                if (at(i) == value)
                    return true;
            }
            return false;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool constexpr starts_with(Span<T const> other) const
        {
            if (size() < other.size())
                return false;

            return TypedTransfer<T>::compare(data(), other.data(), other.size());
        }

        /**
         * @param index 
         * @return ALWAYS_INLINE constexpr const& 
         */
        [[nodiscard]] ALWAYS_INLINE constexpr T const& at(size_t index) const
        {
            VERIFY(index < this->m_size);
            return this->m_values[index];
        }


        /**
         * @param index 
         * @return ALWAYS_INLINE constexpr& 
         */
        [[nodiscard]] ALWAYS_INLINE constexpr T& at(size_t index)
        {
            VERIFY(index < this->m_size);
            return this->m_values[index];
        }

        /**
         * @param index 
         * @return ALWAYS_INLINE constexpr const& 
         */
        [[nodiscard]] ALWAYS_INLINE constexpr T const& operator[](size_t index) const
        {
            return at(index);
        }

        /**
         * @param index 
         * @return ALWAYS_INLINE constexpr& 
         */
        [[nodiscard]] ALWAYS_INLINE constexpr T& operator[](size_t index)
        {
            return at(index);
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator==(Span const& other) const
        {
            if (size() != other.size())
                return false;

            return TypedTransfer<T>::compare(data(), other.data(), size());
        }

        /**
         * @return Span<T const> 
         */
        ALWAYS_INLINE constexpr operator Span<T const>() const
        {
            return 
            { 
                data(), size() 
            };
      }
    }; // class Span
    using ReadonlyBytes = Span<u8 const>;
    using Bytes = Span<u8>;
} // namespace Mods

using Mods::Bytes;
using Mods::ReadonlyBytes;
using Mods::Span;