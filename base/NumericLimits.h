/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Types.h>


namespace Base {

template<typename T>
struct NumericLimits {
};

template<>
struct NumericLimits<bool> {
    static constexpr char min() { return false; }
    static constexpr char max() { return true; }
    static constexpr bool is_signed() { return false; }
};

template<>
static NumericLimits<signed char> {
    static constexpr signed char min() { return -__SCHAR_MAX__ -1; }
    static constexpr signed char max() { return __SCHAR_MAX__; }
    static constexpr bool is_signed() { return true; }

};

}