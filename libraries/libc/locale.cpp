/**
 * @file locale.cpp
 * @author Krisna Pranav
 * @brief locale
 * @version 6.0
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/logstream.h>
#include <assert.h>
#include <locale.h>
#include <stdio.h>

extern "C" {

    static char default_decimal_point[] = ".";
    static char default_thousands_sep[] = ",";
    static char default_grouping[] = "\x03\x03";

    /// @brief  empty values
    static char default_empty_string[] = "";
    static char default_empty_value = 127;

    /// @brief default_locale
    static struct lconv default_locale = {
        default_decimal_point,
        default_thousands_sep,
        default_grouping,
        default_empty_string,
        default_empty_string,
        default_empty_string,
        default_empty_string,
        default_empty_string,
        default_empty_string,
        default_empty_string,
        default_empty_value,
        default_empty_value,
        default_empty_value,
        default_empty_value,
        default_empty_value,
        default_empty_value,
        default_empty_value,
        default_empty_value,
        default_empty_value,
        default_empty_value,
        default_empty_value,
        default_empty_value,
        default_empty_value,
        default_empty_value
    };

    /**
     * @param category 
     * @param locale 
     * @return char* 
     */
    char* setlocale(int category, const char* locale) {
        dbgln("FIXME(LibC): setlocale({}, '{}')", category, locale);
        return nullptr;
    }

    /**
     * @return struct lconv* 
     */
    struct lconv* localeconv() {
        return &default_locale;
    }
}