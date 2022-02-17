/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <init.h>

/**
 * @brief imaxdiv object class
 * 
 * @param numer 
 * @param denom 
 * @return imaxdiv_t 
 */
imaxdiv_t imaxdiv(intmax_t numer, intmax_t denom) {
    imaxdiv_t div;
    div.quot = numer / denom;
    div.rem = numer % denom;
    return div;
}