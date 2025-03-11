/**
 * @file param.h
 * @author Krisna Pranav
 * @brief Para
 * @version 6.0
 * @date 2023-07-31
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <endian.h>
#include <limits.h>

#ifndef MIN
#    define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#    define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef howmany
#    define howmany(x, y) (((x) + ((y)-1)) / (y))
#endif
