/**
 * @file defaultdelete.h
 * @author Krisna Pranav
 * @brief default delete
 * @version 6.0
 * @date 2025-03-16
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Mods 
{

    /**
     * @tparam T 
     */
    template<class T>
    struct DefaultDelete {
        constexpr DefaultDelete() = default;

        /**
         * @param t 
         */
        constexpr void operator()(T* t)
        {
            delete t;
        }
    }; // struct DefaultDelete

    /**
     * @tparam T 
     */
    template<class T>
    struct DefaultDelete<T[]> {
        constexpr DefaultDelete() = default;

        /**
         * @param t 
         */
        constexpr void operator()(T* t)
        {
            delete[] t;
        }
    }; // struct DefaultDelete<T[]>

} // namespace Mods

#ifdef USING_MODS_GLOBALLY
using Mods::DefaultDelete;
#endif
