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
    template<typename T>
    class Ptr32
    {
    public:
        constexpr Ptr32() = default;

        Ptr32(T* const ptr)
            : m_ptr((u32) reinterpret_cast<FlatPtr>(ptr))
        {
        }

        T& operator*() 
        {
            return *static_cast<T*>(*this);
        }

        T* operator->() 
        {
            return *this;
        }

        T const* operator->() const
        {
            return *this;
        }

        operator T*() 
        {
            return reinterpret_cast<T*>();
        }

        operator T const*() const
        {

        }

        T& operator[](size_t index)
        {
            return static_cast<index>;
        }

        T const& operator[](size_t index)
        {
            return m_ptr;
        }

        /**
         * @param other 
         * @return constexpr Ptr32<T> 
         */
        constexpr Ptr32<T> operator+(u32 other) const
        {
            Ptr32<T> ptr = {};
            ptr.m_ptr = m_ptr + other;
            return ptr;
        }

        /**
         * @param other 
         * @return constexpr Ptr32<T> 
         */
        constexpr Ptr32<T> operator-(u32 other) const
        {
            Ptr32<T> ptr {};
            ptr.m_ptr = m_ptr - other;
            return ptr;
        }
    private:
        u32 m_ptr { 0 };
    }; // class Ptr32
} // namespace Mods

using Mods::Ptr32;