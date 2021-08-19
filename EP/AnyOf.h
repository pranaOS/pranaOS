/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <EP/Concepts.h>
#include <EP/Find.h>
#include <EP/Iterator.h>

namespace EP {

template<typename TEndIterator, IteratorPairWith<TEndIterator> TIterator>
constexpr bool any_of(
    TIterator const& begin,
    TEndIterator const& end,
    auto const& predicate)
{
    return find_if(begin, end, predicate) != end;
}

template<IterableContainer Container>
constexpr bool any_of(Container&& container, auto const& predicate)
{
    return any_of(container.begin(), container.end(), predicate);
}

}

using EP::any_of;
