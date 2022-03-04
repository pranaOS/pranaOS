/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdint.h>

/**
 * @brief ALIGN DOWN 
 * 
 */
#define ALIGN_DOWN(base, size) ((base) & -((__typeof__(base))(size)))

/**
 * @brief align up
 * 
 */
#define ALIGN_UP(base, size) ALIGN_DOWN((base) + (size)-1, (size))

/**
 * @brief ptr align down
 * 
 */
#define PTR_ALIGN_DOWN(base, size) \
	((__typeof__(base))ALIGN_DOWN((uintptr_t)(base), (size)))

/**
 * @brief ptr align up
 * 
 */
#define PTR_ALIGN_UP(base, size) \
	((__typeof__(base))ALIGN_UP((uintptr_t)(base), (size)))

/**
 * @brief log2
 * 
 */
#define log2(X) ((unsigned)(8 * sizeof(unsigned long long) - __builtin_clzll((X)) - 1))

/**
 * @brief min
 * 
 */
#define min(x, y) ({ \
	typeof(x) _x = (x);	\
	typeof(y) _y = (y);	\
	(void) (&_x == &_y);		\
	_x < _y ? _x : _y; })

/**
 * @brief max
 * 
 */
#define max(x, y) ({ \
	typeof(x) _x = (x);	\
	typeof(y) _y = (y);	\
	(void) (&_x == &_y);		\
	_x > _y ? _x : _y; })

/**
 * @brief min_t
 * 
 */
#define min_t(type, x, y) \
	({ type __x = (x); type __y = (y); __x < __y ? __x: __y; })

/**
 * @brief max_t 
 * 
 */
#define max_t(type, x, y) \
	({ type __x = (x); type __y = (y); __x > __y ? __x: __y; })

/**
 * @brief div ceil
 * 
 */
#define div_ceil(x, y) ({__auto_type _x = (x); __auto_type _y = (y); _x / _y + (_x % _y > 0); })

/**
 * @brief round
 * 
 */
#define round(x, y) ({__auto_type _x = (x); __auto_type _y = (y); (_x / _y) * _y ; })

/**
 * @brief abs
 * 
 */
#define abs(x) ({__auto_type _x = (x); _x >=0 ? _x : -_x; })

/**
 * @brief rand 
 * 
 * @return uint32_t 
 */
uint32_t rand();

/**
 * @brief srand
 * 
 * @param seed 
 * @return uint32_t 
 */
uint32_t srand(uint32_t seed);