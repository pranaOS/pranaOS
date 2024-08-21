/**
 * @file result.h
 * @author Krisna Pranav
 * @brief RESULT
 * @version 6.0
 * @date 2023-07-29
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "assertions.h"
#include "optional.h"

namespace Mods
{

    template<typename ValueType, typename ErrorType>
    class [[nodiscard]] Result
    {
    public:
        Result(const ValueType& res)
            : m_result(res)
        {}

        Result(ValueType && res)
            : m_result(move(res))
        {}

        Result(const ErrorType& error)
            : m_error(error)
        {}

        Result(ErrorType && error)
            : m_error(move(error))
        {}

        Result(const ValueType& res, const ErrorType& error)
            : m_result(res)
            , m_error(error)
        {}

    private:
        Optional<ValueType> m_result;
        Optional<ErrorType> m_error;
    }; // class Result

} // namespace Mods

using Mods::Result;