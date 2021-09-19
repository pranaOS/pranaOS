#pragma once

#include <libutils/concepts.h>
#include <libutils/forward.h>
#include <libutils/hashfunctions.h>

namespace Utils {

template <typename T>
struct GenericTraits {
    using PeekType = T;
    using ConstPeekType = T;
    static constexpr bool is_trivial()
    {
        return false;
    }

    static constexpr bool equals(const T& a, const T& b)
    {
        return a == b;
    }
};

template <typename T>
struct Traits : public GenericTraits<T> {
};



}

using Utils::Traits;