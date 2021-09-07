/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libabi/result.h>
#include <assert.h>
#include <libutils/optional.h>
#include <libutils/std.h>

namespace Utils
{

template <typename T>
struct ResultOr
{
private:
    JResult _result = SUCCESS;
    Optional<T> _value;

public:
    ALWAYS_INLINE bool success() { return _result == SUCCESS; }

    ALWAYS_INLINE T &unwrap()
    {
        assert(success());

        return _value.unwrap();
    }

    ALWAYS_INLINE const T &unwrap() const
    {
        assert(success());
        return _value.unwrap();
    }

    ALWAYS_INLINE T unwrap_or(T default_)
    {
        if (success())
        {
            return _value.unwrap();
        }
        else
        {
            return default_;
        }
    }

    ALWAYS_INLINE JResult result() const { return _result; }

    ALWAYS_INLINE const char *description()
    {
        return get_result_description(_result);
    }
};

}