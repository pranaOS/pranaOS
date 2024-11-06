/**
 * @file currencycode.h
 * @author Krisna Pranav
 * @brief Currency Code
 * @version 6.0
 * @date 2024-11-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/hashmap.h>
#include <mods/optional.h>
#include <mods/span.h>
#include <mods/string_view.h>

namespace Unicode 
{
    struct CurrencyCode 
    {
        Optional<int> minor_unit {};
    }; // struct CurrencyCode

    /**
     * @brief Get the currency code object
     * 
     * @param currency 
     * @return Optional<CurrencyCode> 
     */
    Optional<CurrencyCode> get_currency_code(StringView currency);

    /**
     * @brief Get the available currencies object
     * 
     * @return Span<StringView const> 
     */
    Span<StringView const> get_available_currencies();
} // namespace Unicode