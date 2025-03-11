/**
 * @file numberformat.h
 * @author Krisna Pranav
 * @brief Number Format
 * @version 6.0
 * @date 2024-11-06
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/optional.h>
#include <mods/string.h>
#include <mods/string_view.h>
#include <mods/vector.h>
#include <libunicode/forward.h>

namespace Unicode 
{
    struct NumberGroupings 
    {
        u8 minimum_grouping_digits { 0 };
        u8 primary_grouping_size { 0 };
        u8 secondary_grouping_size { 0 };
    }; // struct NumberGroupings 

    enum class StandardNumberFormatType : u8 
    {
        Decimal,
        Currency,
        Accounting,
        Percent,
        Scientific,
    }; // enum class StandardNumberFormatType : u8 

    enum class CompactNumberFormatType : u8 
    {
        DecimalLong,
        DecimalShort,
        CurrencyUnit,
        CurrencyShort,
    }; // enum class CompactNumberFormatType : u8 

    struct NumberFormat 
    {
        enum class Plurality : u8 
        {
            Other,
            Zero,
            Single,
            One,
            Two,
            Few,
            Many,
        }; // enum class Plurality : u8 

        u8 magnitude { 0 };
        u8 exponent { 0 };
        Plurality plurality { Plurality::Other };
        StringView zero_format {};
        StringView positive_format {};
        StringView negative_format {};
        Vector<StringView> identifiers {};
    }; // struct NumberFormat 

    enum class NumericSymbol : u8 
    {
        Decimal,
        Exponential,
        Group,
        Infinity,
        MinusSign,
        NaN,
        PercentSign,
        PlusSign,
    }; // enum class NumericSymbol : u8 

    /**
     * @brief Get the default number system object
     * 
     * @param locale 
     * @return Optional<StringView> 
     */
    Optional<StringView> get_default_number_system(StringView locale);

    /**
     * @brief Get the number system symbol object
     * 
     * @param locale 
     * @param system 
     * @param symbol 
     * @return Optional<StringView> 
     */
    Optional<StringView> get_number_system_symbol(StringView locale, StringView system, NumericSymbol symbol);

    /**
     * @brief Get the number system groupings object
     * 
     * @param locale 
     * @param system 
     * @return Optional<NumberGroupings> 
     */
    Optional<NumberGroupings> get_number_system_groupings(StringView locale, StringView system);

    /**
     * @brief Get the digits for number system object
     * 
     * @param system 
     * @return Optional<Span<u32 const>> 
     */
    Optional<Span<u32 const>> get_digits_for_number_system(StringView system);

    /**
     * @param system 
     * @param number 
     * @return String 
     */
    String replace_digits_for_number_system(StringView system, StringView number);

    /**
     * @brief Get the standard number system format object
     * 
     * @param locale 
     * @param system 
     * @param type 
     * @return Optional<NumberFormat> 
     */
    Optional<NumberFormat> get_standard_number_system_format(StringView locale, StringView system, StandardNumberFormatType type);

    /**
     * @brief Get the compact number system formats object
     * 
     * @param locale 
     * @param system 
     * @param type 
     * @return Vector<NumberFormat> 
     */
    Vector<NumberFormat> get_compact_number_system_formats(StringView locale, StringView system, CompactNumberFormatType type);

    /**
     * @brief Get the unit formats object
     * 
     * @param locale 
     * @param unit 
     * @param style 
     * @return Vector<NumberFormat> 
     */
    Vector<NumberFormat> get_unit_formats(StringView locale, StringView unit, Style style);

    /**
     * @param currency_display 
     * @param base_pattern 
     * @return Optional<String> 
     */
    Optional<String> augment_currency_format_pattern(StringView currency_display, StringView base_pattern);

    /**
     * @tparam FormatType 
     * @param formats 
     * @param number 
     * @return Optional<FormatType> 
     */
    template<typename FormatType>
    Optional<FormatType> select_pattern_with_plurality(Vector<FormatType> const& formats, double number)
    {
        auto find_plurality = [&](auto plurality) -> Optional<FormatType> {
            if (auto it = formats.find_if([&](auto& patterns) { return patterns.plurality == plurality; }); it != formats.end())
                return *it;
            return {};
        };

        if (number == 0) {
            if (auto patterns = find_plurality(FormatType::Plurality::Zero); patterns.has_value())
                return patterns;
        } else if (number == 1) {
            if (auto patterns = find_plurality(FormatType::Plurality::One); patterns.has_value())
                return patterns;
        } else if (number == 2) {
            if (auto patterns = find_plurality(FormatType::Plurality::Two); patterns.has_value())
                return patterns;
        } else if (number > 2) {
            if (auto patterns = find_plurality(FormatType::Plurality::Many); patterns.has_value())
                return patterns;
        }

        return find_plurality(FormatType::Plurality::Other);
    }
} // namespace Unicode
