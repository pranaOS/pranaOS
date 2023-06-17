/**
 * @file integer.hpp
 * @author Krisna Pranav
 * @brief Integer
 * @version 0.1
 * @date 2023-06-16
 * 
 * @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <cstdint>

namespace Utils::integer {

    /**
     * @brief BitsInByte
     * 
     */
    constexpr inline auto BitsInByte = 8U;

    /**
     * @brief TypeHolder
     * 
     * @tparam T 
     */
    template <typename T>
    struct TypeHolder {
        using type = T;
    };


    /**
     * @brief Get the Integer Type object
     * 
     * @tparam Bits 
     * @return auto 
     */
    template <unsigned int Bits>
    auto getIntegerType() {
        static_assert(Bits <= 64);

        /**
         * @brief TypeHolder
         * 
         */
        if constexpr (Bits <= 8) {
            return TypeHolder<std::uint8_t>();
        }
        else if constexpr (Bits <= 16) {
            return TypeHolder<std::uint16_t>();
        }
        else if constexpr (Bits <= 32) {
            return TypeHolder<std::uint32_t>();
        }
        else {
            return TypeHolder<std::uint64_t>();
        }
    }

} 