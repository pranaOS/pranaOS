/**
 * @file pair.h
 * @author Krisna Pranav
 * @brief Pair
 * @version 6.0
 * @date 2023-06-19
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav 
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