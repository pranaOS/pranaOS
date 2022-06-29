/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdint.h>

/**
 * @brief name maximum
 * 
 */
#define NAME_MAX 255

/**
 * @brief path maxmimum
 * 
 */
#define PATH_MAX 4096

/**
 * @brief check maximum path len + path max
 * 
 */
#if !defined MAXPATHLEN && defined PATH_MAX
#define MAXPATHLEN PATH_MAX
#endif

/**
 * @brief int max and minimum rule
 * 
 */
#define INT_MAX ((int)(~0U >> 1))
#define INT_MIN (-INT_MAX - 1)

/**
 * @brief uint max rule 
 * 
 */
#define UINT_MAX (~0U)

/**
 * @brief [long] maximum and minimum
 * 
 */
#define LONG_MAX ((long)(~0UL >> 1))
#define LONG_MIN (-LONG_MAX - 1)

/**
 * @brief ulong max
 * 
 */
#define ULONG_MAX (~0UL)

/**
 * @brief ushrt max
 * 
 */
#define USHRT_MAX 65535

/**
 * @brief shrt max
 * 
 */
#define SHRT_MAX 32767

/**
 * @brief ullong max
 * 
 */
#define ULLONG_MAX (~0ULL)