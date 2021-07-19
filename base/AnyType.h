/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <Base/Find.h>
#include <Base/Iterator.h>

namespace Base {

template<typename Container, typename ValueType>
constexpr bool any_type(
    const SimpleIterator<Container, ValueType>& begin,
    const SimpleIterator<Container, ValueType>& end,
    const auto& predicate)
{
    return find_if(begin, end, predicate) != end;
}

}

using Base::any_type;