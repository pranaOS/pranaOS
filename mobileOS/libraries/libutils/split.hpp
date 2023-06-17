/**
 * @file split.hpp
 * @author Krisna Pranav
 * @brief Split functionality
 * @version 0.1
 * @date 2023-06-16
 * 
 * @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <list>
#include <string>
#include <type_traits>

namespace Utils {

    /**
     * @brief Split
     * 
     * @tparam Container 
     * @tparam T 
     * @tparam Allocator 
     * @param strv 
     * @param delims 
     * @return auto 
     */
    template <template <class, class> class Container, class T, class Allocator = std::allocator<T>>
    auto Split(T strv, T delims = " ") {
        static_assert(std::is_same<T, std::string>::value || std::is_same<T, std::string_view>::value,
                      "std::string or std::string_view expected");
        Container<T, Allocator> output;
        size_t first = 0;

        while (first < strv.size()) {
            const auto second = strv.find_first_of(delims, first);

            if (first != second)
                output.emplace_back(strv.substr(first, second - first));

            if (second == std::string_view::npos)
                break;

            first = second + 1;
        }

        return output;
    }
} 