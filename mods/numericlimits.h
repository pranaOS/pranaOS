/**
 * @file numericlimits.h
 * @author Krisna Pranav
 * @brief numericlimits
 * @version 1.0
 * @date 2023-06-29
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "types.h"

namespace Mods {
    template<typename T>
    struct NumericLimits {};

    template<>
    struct NumericLimits<bool> {
        static constexpr char min() {
            return false;
        }

        static constexpr char max() {
            return true;
        }

        static constexpr bool is_signed() {
            return false;
        }
    };

    template<>
    struct NumericLimits<signed char> {
        static constexpr signed char min() {
            return -__SCHAR_MAX__ - 1;
        }

        static constexpr signed char max() {
            return __SCHAR_MAX__;
        }

        static constexpr bool is_signed() {
            return true;
        }
    };

    template<>
    struct NumericLimits<char> {
        static constexpr char min() {
            return -__SCHAR_MAX__ -1;
        }

        static constexpr char max() {
            return __SCHAR_MAX__;
        }

        static constexpr bool is_signed() {
            return true;
        }
    };

    #ifndef KERNEL 

    /**
     * @brief NumericLimits(float)
     * 
     * @tparam  
     */
    template<>
    struct NumericLimits<float> {
        static constexpr unsigned long long min() {
            return 0;
        }

        static constexpr unsigned long long max() {
            return __LONG_LONG_MAX__ * 2ull + 1;
        }

        static constexpr bool is_signed() {
            return false;
        }
    };
    #endif 
}