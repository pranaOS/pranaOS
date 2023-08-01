/**
 * @file endian.h
 * @author Krisna Pranav
 * @brief endian
 * @version 1.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN 4321
#define __PDP_ENDIAN 3412

#if defined(__GNUC__) && defined(__BYTE_ORDER__)
#   define __BYTE_ORDER__ __BYTE_ORDER__
#else
#   include <bits/endian.h>
#endif


__END_DECLS