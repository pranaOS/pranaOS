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

    ALWAYS_INLINE ResultOr(JResult result) : _result{result}, _value{NONE} {}

    ALWAYS_INLINE ResultOr(T value) : _result{SUCCESS}, _value{std::move(value)} {}
};

}