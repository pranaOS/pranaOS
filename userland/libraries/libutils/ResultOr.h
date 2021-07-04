/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <libabi/Result.h>
#include <assert.h>
#include <libutils/Optional.h>
#include <libutils/Std.h>


namespace Utils
{


template <typename T>
{
private:
    JResult _result = SUCCESS;
    Optional<T> _value;
}
}