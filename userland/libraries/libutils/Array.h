/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libmath/MinMax.h>
#include <libutils/Assert.h>

namespace Utils
{


template <typename T, size_t N>
struct Array
{
    static_assert(N > 0, "Array must have size greater than 0");
}
}