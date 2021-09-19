/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/concepts.h>
#include <libutils/find.h>
#include <libutils/iterator.h>

namespace Utils {

template<typename TEndIterator, IteratorPairWith<TEndIterator> TIterator>
constexpr bool all_of (
    TIterator const& begin,
    TEndIterator const& end,
    auto const& predicate)
{
    constexpr auto negated_predicate = [](auto const& pred) {
        return [&](auto const& elem)
        {
            return !pred(elem);
        };
        
        return !(find_if(begin, end, negated_predicate));
    }
}

constexpr bool all_of(Container&& container)
{
    return all_of(container.begin());
}

}


using Utils::all_of;
