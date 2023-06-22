/**
* @file kliballoc.h
* @author Krisna Pranav
* @brief KLIBalloc
* @version 1.0
*
* @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
*
 */

#pragma once

#include "../kstd/types.h"

typedef uint32_t uintptr_t;

#define PREFIX(func) k ## func

#ifdef __cplusplus
extern "C" {
#endif

extern void *PREFIX(malloc)(size_t);

#ifdef __cplusplus
}

#endif