/**
* @file utils.h
* @author Krisna Pranav
* @brief Utils
* @version 1.0
* @date 2023-07-07
*
* @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
*
*/

#pragma once

#include <cstddef>

namespace Utils
{
    template<typename T, size_t N>
    constexpr size_t size(T (&)[N])
    {
        return N;
    }
}