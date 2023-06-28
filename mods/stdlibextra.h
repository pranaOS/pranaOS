/**
 * @file stdlibextras.h
 * @author Krisna Pranav
 * @brief StdLIBExtra.h
 * @version 1.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#define UNUSED_PARAM(x) (void)x

/**
 * @brief round_up_to_power_of_two
 * 
 * @param value 
 * @param power_of_two 
 * @return constexpr unsigned 
 */
constexpr unsigned round_up_to_power_of_two(unsigned value, unsigned power_of_two) {
    return ((value - 1) & ~(power_of_two - 1)) + power_of_two;
}

namespace Mods {
    template<typename T>
    auto declval() -> T;

    template<typename T, typename SizeType = decltype(sizeof(T)), SizeType N>
    constexpr SizeType array_size(T (&)[N]) {
        return N;
    }
}