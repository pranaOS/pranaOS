/**
 * @file neverdestroyed.h
 * @author Krisna Pranav
 * @brief neverdestroyed
 * @version 6.0
 * @date 2023-07-11
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/noncopyable.h>
#include <mods/types.h>

namespace Mods
{
    /**
     * @tparam T 
     */
    template <typename T>
    class NeverDestroyed
    {
        MOD_MAKE_NONCOPYABLE(NeverDestroyed);
        MOD_MAKE_NONMOVABLE(NeverDestroyed);

    public:
        /**
         * @brief Construct a new Never Destroyed object
         * 
         * @tparam Args 
         * @param args 
         */
        template <typename... Args>
        NeverDestroyed(Args&&... args)
        {
            new(storage) T(forward<Args>(args)...);
        }   

        /**
         * @brief Destroy the Never Destroyed object
         * 
         */
        ~NeverDestroyed() = default;

        /**
         * @return T* 
         */
        T* operator->()
        {
            return &get();
        }

        /**
         * @return const T* 
         */
        const T* operator->() const
        {
            return &get();
        }

        /**
         * @return T& 
         */
        T& operator*()
        {
            return get();
        }

        /**
         * @return const T& 
         */
        const T& operator*() const
        {
            return get();
        }

        /**
         * @return T& 
         */
        T& get()
        {
            return reinterpret_cast<T&>(storage);
        }

        /**
         * @return const T& 
         */
        const T& get() const
        {
            return reinterpret_cast<T&>(storage);
        }

    private:
        alignas(T) u8 storage[sizeof(T)];
    }; // class NeverDestroyed
} // namespace Mods

using Mods::NeverDestroyed;
