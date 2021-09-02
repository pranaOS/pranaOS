/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _LIBC_ARPA_INET_H
#define _LIBC_ARPA_INET_H 1

// includes
#include <stdint.h>

char *inet_ntop(uint32_t src, char *dst, uint16_t len);

#endif