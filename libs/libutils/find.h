#pragma once

// includes
#include <libutils/concepts.h>
#include <libutils/traits.h>
#include <libutils/types.h>

namespace Utils {

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

}