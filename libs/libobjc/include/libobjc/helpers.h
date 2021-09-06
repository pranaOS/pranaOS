/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdio.h>

#define OBJC_EXPORT extern "C"

#define OBJC_DEBUG

#ifdef OBJC_DEBUG
#define OBJC_DEBUGPRINT(...) \
    printf(__VA_ARGS__);     \
    fflush(stdout)
#else
#define OBJC_DEBUGPRINT(...) (sizeof(int))
#endif