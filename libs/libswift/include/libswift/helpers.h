/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <stdio.h>

#define SWIFT_EXPORT extern "C"

#define SWIFT_DEBUG

#ifdef SWIFT_DEBUG
#define SWIFT_DEBUGPRINT(...) \
    printf(__VA_ARGS__);     \
    fflush(stdout)
#else
#define SWIFT_DEBUGPRINT(...) (sizeof(int))
#endif