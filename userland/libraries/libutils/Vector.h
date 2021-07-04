/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <string.h>

#include <libmath/MinMax.h>
#include <libutils/Iteration.h>
#include <libutils/RefPtr.h>
#include <libutils/Std.h>

namespace Utils
{

template <typename T>
void typed_copy(T *destination, T *source, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        new (&destination[i]) T(source[i]);
    }
}

template <typename T>
void typed_move(T *destination, T *source, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        if (destination <= source)
        {
            new (&destination[i]) T(std::move(source[i]));
        }
        else
        {
            new (&destination[count - i - 1]) T(std::move(source[count - i - 1]));
        }
    }
}


}