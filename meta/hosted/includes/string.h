/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include_next <string.h>

size_t strlcpy(char *dst, const char *src, size_t maxlen);

void strrvs(char *str);
