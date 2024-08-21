/**
 * @file byteswap.h
 * @author Krisna Pranav
 * @brief byteswp
 * @version 6.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <sys/cdefs.h>

#pragma once

__BEGIN_DECLS

/// @brief: bswap[16, 32, 64]
#define bswap_16(x) (__builtin_bswap16(x))
#define bswap_32(x) (__builtin_bswap32(x))
#define bswap_64(x) (__builtin_bswap64(x))

__END_DECLS
