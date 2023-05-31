/**
 * @file utility.h
 * @author Krisna Pranav
 * @brief KSTD Utility
 * @version 0.1
 * @date 2023-05-31
 * 
 * @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "type_traits.h"

namespace kstd {
    template <typename T>
	typename remove_reference<T>::type&& move(T&& arg) {
		return static_cast<typename remove_reference<T>::type&&>(arg);
	}

    template <typename T> void swap(T& t1, T& t2) {
        T temp = kstd::move(t1);
        t1 = kstd::move(t2);
        t2 = kstd::move(temp);
    }

    template<typename A, typename B>
    constexpr A ceil_div(A a, B b) {
        return (a + (b - 1)) / b;
    }

    template<typename T, typename U>
    inline constexpr bool is_base_of = __is_base_of(T, U);
}