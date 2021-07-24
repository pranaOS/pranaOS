/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/StdLibExtras.h>
#include <base/TypeList.h>

namespace AK::Detail {

template<typename... Ts>
struct Tuple {
};

template<typename T>
struct Tuple<T> {
    Tuple(T&& value) requires(!IsSame<T&&, const T&>)
        : value(forward<T>(value))
    {
    }

    Tuple(const T& value)
        : value(value)
    {
    }

    template<typename U>
    U& get()
    {
        static_assert(IsSame<T, U>, "Invalid tuple access");
        return value;
    }

    template<typename U>
    const U& get() const
    {
        return const_cast<Tuple<T>&>(*this).get<U>();
    }

    template<typename U, unsigned index>
    U& get_with_index()
    {
        static_assert(IsSame<T, U> && index == 0, "Invalid tuple access");
        return value;
    }

    template<typename U, unsigned index>
    const U& get_with_index() const
    {
        return const_cast<Tuple<T>&>(*this).get_with_index<U, index>();
    }

private:
    T value;
};

template<typename T, typename... TRest>
struct Tuple<T, TRest...> : Tuple<TRest...> {
    Tuple(T&& first, TRest&&... rest)
        : Tuple<TRest...>(forward<TRest>(rest)...)
        , value(forward<T>(first))
    {
    }

    Tuple(const T& first, const TRest&... rest)
        : Tuple<TRest...>(rest...)
        , value(first)
    {
    }

    template<typename U>
    U& get()
    {
        if constexpr (IsSame<T, U>)
            return value;
        else
            return Tuple<TRest...>::template get<U>();
    }

    template<typename U>
    const U& get() const
    {
        return const_cast<Tuple<T, TRest...>&>(*this).get<U>();
    }

    template<typename U, unsigned index>
    U& get_with_index()
    {
        if constexpr (IsSame<T, U> && index == 0)
            return value;
        else
            return Tuple<TRest...>::template get_with_index<U, index - 1>();
    }

    template<typename U, unsigned index>
    const U& get_with_index() const
    {
        return const_cast<Tuple<T, TRest...>&>(*this).get_with_index<U, index>();
    }

private:
    T value;
};

}