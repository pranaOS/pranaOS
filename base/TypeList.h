/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/StdLibExtras.h>

namespace Base {

template<typename... Types>
struct TypeList;

template<unsigned Index, typename List>
struct TypeListElement;

template<unsigned Index, typename Head, typename... Tail>
struct TypeListElement<Index, TypeList<Head, Tail...>>
    : TypeListElement<Index - 1, TypeList<Tail...>> {
};

template<typename Head, typename... Tail>
struct TypeListElement<0, TypeList<Head, Tail...>> {
    using Type = Head;
};

template<typename ListA, typename ListB, typename F>
constexpr void for_each_type_zipped(F&& f)
{
    static_assert(ListA::size == ListB::size, "Can't zip TypeLists that aren't the same size!");
    for_each_type_zipped_impl<ListA, ListB>(forward<F>(f), MakeIndexSequence<ListA::size> {});
}

}