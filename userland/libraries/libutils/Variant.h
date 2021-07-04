/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/Assert.h>
#include <libutils/Std.h>
#include <libutils/Traits.h>

namespace Utils
{

template <typename... TStack>
struct Dispatch;

template <>
struct Dispatch<>
{
    static void func(int, auto, auto, &)
    {
        Assert::unreachable();
    }
};

template <typename TFirst, typename... TStack>
struct Dispatch<TFirst, TStack...>
{
    static void func(int index, auto storage, auto &visitor)
    {
        if (index == 0)
        {
            using ConsT = 
        }
    }
}

}