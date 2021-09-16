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
    ALWAYS_INLINE bool success()
    {
        return _result == SUCCESS;
    }

};

#define TRY(__stuff)                                                \
    ({                                                              \
        auto __eval__ = __stuff;                                    \
                                                                    \
        if (::Utils::__extract_result(__eval__) != SUCCESS)         \
        {                                                           \
            return ::Utils::__extract_result(__eval__);             \
        }                                                            \   
        ::Utils::__extract_value(__eval__);                          \ 
    })                                                               \

}