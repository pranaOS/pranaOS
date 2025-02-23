/**
 * @file locale.cpp
 * @author Krisna Pranav
 * @brief locale
 * @version 6.0
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>

extern "C" {

static char default_decimal_point[] = ".";
static char default_thousands_sep[] = ",";
static char default_grouping[] = "\x03\x03";

static char default_empty_string[] = "";
static char default_empty_value = 127;

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
 * @return char* 
 */
char* setlocale(int, char const*)
{
    static char locale[2];
    memcpy(locale, "C", 2);
    return locale;
}

/**
 * @return struct lconv* 
 */
struct lconv* localeconv()
{
    return &default_locale;
}
}
