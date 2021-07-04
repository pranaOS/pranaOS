/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <assert.h>

namespace Utils
{

template <typename T>
struct RefCounted
{
private:
    int _refcount = 1;
    NONCOPYABLE(RefCounted);
    NONMOVABLE(RefCounted);

};

}