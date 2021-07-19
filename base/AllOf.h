/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Iteration.h>

namespace Base {

template<typename Container, typename ValueType>
constexpr bool all_of(
    const SimpleIterator<Container, ValueType>& begin,
    const SimpleIterator<Container, ValueType>& end,
    const auto& predicate)
{
    for (auto iter = begin; iter != end; ++iter) {
        if (!predicate(*iter)) {
            return false;
        }
    }
    return true;
}

}

using Base::all_of;