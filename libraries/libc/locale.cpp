/**
 * @file locale.cpp
 * @author Krisna Pranav
 * @brief locale
 * @version 1.0
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

    static struct lconv default_locale = {
        default_decimal_point
    };
}

char* setlocale(int category, const char* locale) {
    dbgln("LIBC: setlocale({}, '{}')", category, locale);
}

struct lconv* localeconv() {
    return &default_locale;
}