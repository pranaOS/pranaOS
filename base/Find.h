/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Traits.h>
#include <base/Types.h>

namespace Base {

template<typename TIterator, typename TUnaryPredicate>
constexpr TIterator find_if(TIterator first, TIterator last, TUnaryPredicate&& pred)
{
    for (; first != last; ++first) {
        if (pred(*first)) {
            return first;
        }
    }
    return last;
}

}