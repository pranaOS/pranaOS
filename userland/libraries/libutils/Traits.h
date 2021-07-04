/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
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



}