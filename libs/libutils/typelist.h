/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/stdextras.h>

namespace Utils {

template <typename... Types>
struct TypeList;

template <unsigned Index, typename List>
struct TypeListElement;

template<unsigned Index, typename Head, typename... Tail>
struct TypeListElement<Index, TypeList<Head, Tail...>>
    : TypeListElement<Index - 1, TypeList<Tail...>> {
};

template <typename Head, typename... Tail>
struct TypeListElement<0, TypeList<Head, Tail...>> {
    using Type = Head;
};

template <typename... Types>
struct TypeList {
    static constexpr unsigned size = sizeof...(Types);

    template <unsigned N>
};

template <typename T>
struct TypeWrapper {
    using Type = T;
};


}