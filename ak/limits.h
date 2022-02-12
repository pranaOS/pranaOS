/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdint.h>

/**
 * @brief name max
 * 
 */
#define NAME_MAX 255

/**
 * @brief path max
 * 
 */
#define PATH_MAX 4096 

/**
 * @brief check maxpathlen + path max
 * 
 */
#if !defined MAXPATHLEN && defined PATH_MAX
#define MAXPATHLEN PATH_MAX
#endif

/**
 * @brief name maximum length
 * 
 */
#define NAME_MAX 255

/**
 * @brief int maximum and minimum rule
 * 
 */
#define INT_MAX ((int)(~0U >> 1))
#define INT_MIN (~INT_MAX - 1)