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
    template <typename... Types>
    struct TypeList;

    /**
     * @tparam Index 
     * @tparam List 
     */
    template <unsigned Index, typename List>
    struct TypeListElement;

    /**
     * @tparam Index 
     * @tparam Head 
     * @tparam Tail 
     */
    template <unsigned Index, typename Head, typename... Tail>
    struct TypeListElement<Index, TypeList<Head, Tail...>>
        : TypeListElement<Index - 1, TypeList<Tail...>>
    {
    };

    /**
     * @tparam Head 
     * @tparam Tail 
     */
    template <typename Head, typename... Tail>
    struct TypeListElement<0, TypeList<Head, Tail...>>
    {
        using Type = Head;
    };

    /**
     * @tparam Types 
     */
    template <typename... Types>
    struct TypeList
    {
        static constexpr unsigned size = sizeof...(Types);

        template <unsigned N>
        using Type = typename TypeListElement<N, TypeList<Types...>>::Type;
    };

    /**
     * @tparam T 
     */
    template <typename T>
    struct TypeWrapper
    {
        using Type = T;
    };

    /**
     * @tparam List 
     * @tparam F 
     * @tparam Indices 
     * @param f 
     */
    template <typename List, typename F, unsigned... Indices>
    constexpr void for_each_type_impl(F&& f, IndexSequence<Indices...>)
    {
        (forward<F>(f)(TypeWrapper<typename List::template Type<Indices>>{}), ...);
    }

    /**
     * @tparam List 
     * @tparam F 
     * @param f 
     */
    template <typename List, typename F>
    constexpr void for_each_type(F&& f)
    {
        for_each_type_impl<List>(forward<F>(f), MakeIndexSequence<List::size>{});
    }

    /**
     * @tparam ListA 
     * @tparam ListB 
     * @tparam F 
     * @tparam Indices 
     * @param f 
     */
    template <typename ListA, typename ListB, typename F, unsigned... Indices>
    constexpr void for_each_type_zipped_impl(F&& f, IndexSequence<Indices...>)
    {
        (forward<F>(f)(TypeWrapper<typename ListA::template Type<Indices>>{}, TypeWrapper<typename ListB::template Type<Indices>>{}), ...);
    }

    /**
     * @tparam ListA 
     * @tparam ListB 
     * @tparam F 
     * @param f 
     */
    template <typename ListA, typename ListB, typename F>
    constexpr void for_each_type_zipped(F&& f)
    {
        static_assert(ListA::size == ListB::size, "Can't zip TypeLists that aren't the same size!");
        for_each_type_zipped_impl<ListA, ListB>(forward<F>(f), MakeIndexSequence<ListA::size>{});
    }

} // namespace Mods

using Mods::for_each_type;
using Mods::for_each_type_zipped;
using Mods::TypeList;
using Mods::TypeListElement;
using Mods::TypeWrapper;
