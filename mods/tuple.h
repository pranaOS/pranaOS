/**
 * @file tuple.h
 * @author Krisna Pranav
 * @brief tuple
 * @version 6.0
 * @date 2025-02-16
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/stdlibextra.h>
#include <mods/typelist.h>

namespace Mods::Detail
{
    /**
     * @tparam Ts 
     */
    template<typename... Ts>
    struct Tuple {};

    template<typename T>
    struct Tuple<T> {
        Tuple(T&& value) requires(!IsSame<T&&, const T&>)
            : value(forward<T>(value))
        {}

        /**
         * @brief Construct a new Tuple object
         * 
         * @param value 
         */
        Tuple(const T& value)
            : value(value)
        {}  

        /**
         * @tparam U 
         */
        template<typename U>
        U& get
        {
            static_assert(IsSame<T, U>, "Invalid tuple access");
            return value;
        }
        
        template<typename U>
        const U& get() const
        {
            return const_cast<Tuple<T>&>(*this).get<U>();
        }
    private:
        T value;
    }
} // namespace Mods::Detail