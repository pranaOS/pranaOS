/**
 * @file locale.h
 * @author Krisna Pranav
 * @brief locale
 * @version 1.0
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

enum {
    LC_ALL,
    LC_NUMERIC,
    LC_CTYPE,
    LC_COLLATE,
    LC_TIME,
    LC_MONETARY
};

struct lconv {
    char* decimal_point;
    char* thousands_sep;
    char* grouping;
    char* int_curr_symbol;
    char* currency_symbol;
    char mon_decimal_point;
    char* mon_thousands_sept;
    char* mon_grouping;
};

/**
 * @return struct lconv* 
 */
struct lconv* localeconv();

/**
 * @param category 
 * @param locale 
 * @return char* 
 */
char* setlocale(int category, const char* locale);

__END_DECLS