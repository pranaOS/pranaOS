/**
 * @file genericshorthands.h
 * @author Krisna Pranav
 * @brief generic short hands
 * @version 6.0
 * @date 2025-07-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/platform.h>

namespace Mods 
{

    template<typename T, typename... Ts>
    [[nodiscard]] constexpr bool first_is_one_of(T const to_compare, Ts const... valid_values)
    {
        return (... || (to_compare == valid_values));
    }

    template<typename T, typename... Ts>
    [[nodiscard]] constexpr bool first_is_smaller_than_one_of(T const to_compare, Ts const... valid_values)
    {
        return (... || (to_compare < valid_values));
    }

    template<typename T, typename... Ts>
    [[nodiscard]] constexpr bool first_is_smaller_or_equal_than_one_of(T const to_compare, Ts const... valid_values)
    {
        return (... || (to_compare <= valid_values));
    }

    template<typename T, typename... Ts>
    [[nodiscard]] constexpr bool first_is_larger_than_one_of(T const to_compare, Ts const... valid_values)
    {
        return (... || (to_compare > valid_values));
    }

    template<typename T, typename... Ts>
    [[nodiscard]] constexpr bool first_is_larger_or_equal_than_one_of(T const to_compare, Ts const... valid_values)
    {
        return (... || (to_compare >= valid_values));
    }

    template<typename T, typename... Ts>
    [[nodiscard]] constexpr bool first_is_smaller_than_all_of(T const to_compare, Ts const... valid_values)
    {
        return (... && (to_compare < valid_values));
    }

    template<typename T, typename... Ts>
    [[nodiscard]] constexpr bool first_is_smaller_or_equal_than_all_of(T const to_compare, Ts const... valid_values)
    {
        return (... && (to_compare <= valid_values));
    }

    template<typename T, typename... Ts>
    [[nodiscard]] constexpr bool first_is_larger_than_all_of(T const to_compare, Ts const... valid_values)
    {
        return (... && (to_compare > valid_values));
    }

    template<typename T, typename... Ts>
    [[nodiscard]] constexpr bool first_is_larger_or_equal_than_all_of(T const to_compare, Ts const... valid_values)
    {
        return (... && (to_compare >= valid_values));
    }

} // namespace Mods

#if USING_MODS_GLOBALLY
using Mods::first_is_larger_or_equal_than_all_of;
using Mods::first_is_larger_or_equal_than_one_of;
using Mods::first_is_larger_than_all_of;
using Mods::first_is_larger_than_one_of;
using Mods::first_is_one_of;
using Mods::first_is_smaller_or_equal_than_all_of;
using Mods::first_is_smaller_or_equal_than_one_of;
using Mods::first_is_smaller_than_all_of;
using Mods::first_is_smaller_than_one_of;
#endif
