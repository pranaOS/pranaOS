/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

#include <libutils/Prelude.h>

namespace Utils
{

template <bool B, typename T = void>
struct EnableIf
{
};

template <typename T>
struct EnableIf<true, T>
{
    using Type = T;
};

template <typename T>
struct AddConst
{
    using Type = const T;
};

template <typename T>
struct RemoveConst
{
    using Type = T;
};

template <typename T>
struct RemoveConst<const T>
{
    using Type = T;
};

template <typename T>
struct RemoveVolatile
{
    using Type = T;
};

template <typename T>
struct RemoveVolatile<volatile T>
{
    using Type = T;
};

template <typename T>
struct RemoveConstVolatile
{
    typedef typename RemoveVolatile<typename RemoveConst<T>::Type>::Type Type;
};

}