/**
 * @file pair.h
 * @author Krisna Pranav
 * @brief Pair
 * @version 1.0
 * @date 2023-06-19
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav 
 * 
 */

#pragma once

namespace kstd {
    template <typename A, typename B>
    class pair {
    public:
        A first;
        B second;
    };
}