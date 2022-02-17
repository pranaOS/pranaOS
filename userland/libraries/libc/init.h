/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdint.h>
#include <sys/types.h>

/**
 * @brief if wchar_t defined
 * 
 */
#ifndef __wchar_t_defined
#define __wchar_t_defined
#define __need_wchar_t
#include <stddef.h>
#endif

/**
 * @brief some codes
 * 
 */
#define PRId8 "d"
#define PRId16 "d"
#define PRId32 "d"
#define PRId64 "lld"
#define PRIi8 "d"
#define PRIi16 "d"
#define PRIi32 "d"
#define PRIi64 "lld"
#define PRIu8 "u"
#define PRIo8 "o"
#define PRIo16 "o"
#define PRIo32 "o"
#define PRIo64 "llo"
#define PRIu16 "u"
#define PRIu32 "u"
#define PRIu64 "llu"
#define PRIx8 "b"
#define PRIx16 "w"
#define PRIx32 "x"
#define PRIX32 "X"
#define PRIx64 "llx"
#define PRIX64 "llX"

#define __PRI64_PREFIX "ll"
#define __PRIPTR_PREFIX

#define PRIdPTR __PRIPTR_PREFIX "d"
#define PRIiPTR __PRIPTR_PREFIX "i"
#define PRIXPTR __PRIPTR_PREFIX "X"

#define PRIdMAX __PRI64_PREFIX "d"
#define PRIoMAX __PRI64_PREFIX "o"
#define PRIuMAX __PRI64_PREFIX "u"

#define SCNdMAX __PRI64_PREFIX "d"
#define SCNoMAX __PRI64_PREFIX "o"
#define SCNuMAX __PRI64_PREFIX "u"

/**
 * @brief intmaxdiv struct 
 * 
 */
typedef struct {
	intmax_t quot;
	intmax_t rem;
} imaxdiv_t;

/**
 * @brief imaxabs
 * 
 * @param val 
 * @return intmax_t 
 */
intmax_t imaxabs(intmax_t val);

/**
 * @brief imaxdiv
 * 
 * @param numer 
 * @param denom 
 * @return imaxdiv_t 
 */
imaxdiv_t imaxdiv(intmax_t numer, intmax_t denom);

/**
 * @brief strtoimax
 * 
 * @param nptr 
 * @param endptr 
 * @param base 
 * @return intmax_t 
 */
intmax_t strtoimax(const char *restrict nptr, char **restrict endptr, int base);

/**
 * @brief strtoumax
 * 
 * @param nptr 
 * @param endptr 
 * @param base 
 * @return uintmax_t 
 */
uintmax_t strtoumax(const char *restrict nptr, char **restrict endptr, int base);

/**
 * @brief wcstoimax
 * 
 * @param nptr 
 * @param endptr 
 * @param base 
 * @return intmax_t 
 */
intmax_t wcstoimax(const wchar_t *restrict nptr, wchar_t **restrict endptr, int base);

/**
 * @brief wcstoumax
 * 
 * @param nptr 
 * @param endptr 
 * @param base 
 * @return uintmax_t 
 */
uintmax_t wcstoumax(const wchar_t *restrict nptr, wchar_t **restrict endptr, int base);