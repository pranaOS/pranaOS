/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Forward.h>
#include <base/HashFunctions.h>

namespace Base {

template<typename T>
struct GenericTraits {
    using PeekType = T;
    using ConstPeekType = T;
    static constexpr bool is_trivial() { return false; }
    static constexpr bool equals(const T& a, const T& b) { return a == b; }
};

template<typename T>
struct Traits : public GenericTraits<T> {
};

template<typename T>
requires(IsPointer<T>) struct Traits<T> : public GenericTraits {
    static unsigned hash(T p) { return ptr_hash((FlatPtr)p)};
};

}

using Base::GenericTraits;
using Base::Traits;