/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Array.h>
#include <base/BitCast.h>
#include <base/StdLibExtras.h>
#include <base/TypeList.h>

namespace Base::Detail {

template<typename T, typename IndexType, IndexType InitialIndex, typename... InTypes>
struct VariantIndexOf {
    static_assert(DependentFalse<T, IndexType, InTypes...>, "Invalid VariantIndex instantiated");
};

template<typename T, typename IndexType, IndexType InitialIndex, typename InType, typename... RestOfInTypes>
struct VariantIndexOf<T, IndexType, InitialIndex, InType, RestOfInTypes...> {
    consteval IndexType operator()()
    {
        if constexpr (IsSame<T, InType>)
            return InitialIndex;
        else
            return VariantIndexOf<T, IndexType, InitialIndex + 1, RestOfInTypes...> {}();
    }
};

template<typename T, typename IndexType, IndexType InitialIndex>
struct VariantIndexOf<T, IndexType, InitialIndex> {
    consteval IndexType operator()() { return InitialIndex; }
};

template<typename T, typename IndexType, typename... Ts>
consteval IndexType index_of()
{
    return VariantIndexOf<T, IndexType, 0, Ts...> {}();
}


}