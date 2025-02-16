/**
 * @file typelist.h
 * @author Krisna Pranav
 * @brief Type List
 * @version 6.0
 * @date 2025-02-16
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/stdlibextra.h>

namespace Mods
{
    /**
     * @tparam Types 
     */
    template<typename... Types>
    struct TypeList;

    /**
     * @tparam Index 
     * @tparam List 
     */
    template<unsigned Index, typename List>
    struct TypeListElement;

    /**
     * @tparam Index 
     * @tparam Head 
     * @tparam Tail 
     */
    template<unsigned Index, typename Head, typename... Tail>
    struct TypeListElement<Index, TypeList<Head, Tail...>>
        : TypeListElement<Index - 1, TypeList<Tail...>> {
    };

    /**
     * @tparam Types 
     */
    template<typename... Types>
    struct TypeList
    {
        static constexpr unsigned size = sizeof...(Types);
        
        template<unsigned N>
        using Type = typename TypeListElement<N>;
    }; // struct TypeList

    template<typename T>
    struct TypeWrapper
    {
        using Type = T;
    }; 
} // namespace Mods