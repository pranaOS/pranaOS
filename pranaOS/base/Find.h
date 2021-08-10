/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <base/Concepts.h>
#include <base/Traits.h>
#include <base/Types.h>

namespace Base {

template<typename TEndIterator, IteratorPairWith<TEndIterator> TIterator, typename TUnaryPredicate>
constexpr TIterator find_if(TIterator first, TEndIterator last, TUnaryPredicate&& pred)
{
    for (; first != last; ++first) {
        if (pred(*first)) {
            return first;
        }
    }
    return last;
}

template<typename TEndIterator, IteratorPairWith<TEndIterator> TIterator, typename T>
constexpr TIterator find(TIterator first, TEndIterator last, T const& value)
{
    return find_if(first, last, [&](auto const& v) { return Traits<T>::equals(value, v); });
}

template<typename TEndIterator, IteratorPairWith<TEndIterator> TIterator, typename T>
constexpr size_t find_index(TIterator first, TEndIterator last, T const& value) requires(requires(TIterator it) { it.index(); })
{
    return find_if(first, last, [&](auto const& v) { return Traits<T>::equals(value, v); }).index();
}

}
