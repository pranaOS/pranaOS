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
constexpr bool any_of(
    TIterator const& begin,
    TEndIterator const& end,
auto const& predicate)
{
return find_if(begin, end, predicate) != end;
}

}

using Utils::any_of;
