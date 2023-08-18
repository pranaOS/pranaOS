/**
 * @file all.h
 * @author Krisna Pranav
 * @brief All Iter
 * @version 6.0
 * @date 2023-06-27
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Mods {

    /**
     * @brief allOf
     * 
     * @param begin 
     * @param end 
     * @param predicate 
     * @return true 
     * @return false 
     */
    constexpr bool allof(const auto& begin, const auto& end, const auto& predicate) {
        for (auto iter = begin; iter != end; ++iter) {
            if (!predicate(*iter)) {
                return false;
            }
        }
        return true;
    }

}