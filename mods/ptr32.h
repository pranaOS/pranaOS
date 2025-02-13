/**
 * @file ptr32.h
 * @author Krisna Pranav
 * @brief ptr 32
 * @version 6.0
 * @date 2025-02-12
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/assertions.h>
#include <mods/types.h>

namespace Mods
{
    /**
     * @tparam T 
     */
    template <typename T>
    class Ptr32
    {
    public:
        constexpr Ptr32() = default;

        /**
         * @brief Construct a new Ptr32 object
         * 
         * @param ptr 
         */
        Ptr32(T* const ptr)
            : m_ptr((u32) reinterpret_cast<FlatPtr>(ptr))
        {
            VERIFY((reinterpret_cast<FlatPtr>(ptr) & 0xFFFFFFFFULL) == static_cast<FlatPtr>(m_ptr));
        }

        /**
         * @return T& 
         */
        T& operator*()
        {
            return *static_cast<T*>(*this);
        }

        /**
         * @return T const& 
         */
        T const& operator*() const
        {
            return *static_cast<T const*>(*this);
        }

        /**
         * @return T* 
         */
        T* operator->()
        {
            return *this;
        }

        /**
         * @return T const* 
         */
        T const* operator->() const
        {
            return *this;
        }

        /**
         * @return T* 
         */
        operator T*()
        {
            return reinterpret_cast<T*>(static_cast<FlatPtr>(m_ptr));
        }

        /**
         * @return T const* 
         */
        operator T const*() const
        {
            return reinterpret_cast<T const*>(static_cast<FlatPtr>(m_ptr));
        }

        /**
         * @param index 
         * @return T& 
         */
        T& operator[](size_t index)
        {
            return static_cast<T*>(*this)[index];
        }

        /**
         * @param index 
         * @return T const& 
         */
        T const& operator[](size_t index) const
        {
            return static_cast<T const*>(*this)[index];
        }

        /**
         * @return true 
         * @return false 
         */
        constexpr explicit operator bool()
        {
            return m_ptr;
        }

        /**
         * @tparam U 
         * @param other 
         * @return true 
         * @return false 
         */
        template <typename U>
        constexpr bool operator==(Ptr32<U> other)
        {
            return m_ptr == other.m_ptr;
        }

        /**
         * @param other 
         * @return constexpr Ptr32<T> 
         */
        constexpr Ptr32<T> operator+(u32 other) const
        {
            Ptr32<T> ptr{};
            ptr.m_ptr = m_ptr + other;
            return ptr;
        }

        /**
         * @param other 
         * @return constexpr Ptr32<T> 
         */
        constexpr Ptr32<T> operator-(u32 other) const
        {
            Ptr32<T> ptr{};
            ptr.m_ptr = m_ptr - other;
            return ptr;
        }

    private:
        u32 m_ptr{0};
    }; // class Ptr32
} // namespace Mods

using Mods::Ptr32;