/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/assertions.h>
#include <libutils/optional.h>

namespace Utils {

template <typename ValueT, typename ErrorT>
class [[nodiscard]] Result {

public:
    using ValueType = ValueT;
    using ErrorType = ErrorT;

    Result(const ValueType& res)
        : m_result(res)
    {
    }

    Result(ValueType&& res)
        : m_result(move(res))
    {
    }

    Result(const ErrorType& error)
        : m_error(error)
    {
    }

    Result(ErrorType&& error)
        : m_error(move(error))
    {
    }

    Result(Result&& other) = default;
    Result(const Result&& other) = default;
    ~Result() = default;

};

}