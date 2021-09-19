#pragma once

#include <libutils/types.h>

namespace Utils {

template <typename T>
struct NumericLimits {
};

template <>
struct NumericLimits<bool> {
    static constexpr char min()
    {
        return false;
    }

    static constexpr char max()
    {
        return true;
    }

    static constexpr bool is_signed()
    {
        return false;
    }
}

#ifndef KERNEL

template<>
struct NumericLimits<long double> {
    static constexpr long double lowest() { return -__LDBL_MAX__; }
    static constexpr long double min() { return __LDBL_MIN__; }
    static constexpr long double max() { return __LDBL_MAX__; }
    static constexpr long double epsilon() { return __LDBL_EPSILON__; }
    static constexpr bool is_signed() { return true; }
};

#endif

}

using Utils::NumericLimits;