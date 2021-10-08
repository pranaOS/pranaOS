/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/platform.h>
#include <libutils/types.h>

#if defined(__pranaOS__)
#   include <stdlib.h>
#endif

#if defined(__unix__)
#   include <unistd.h>
#endif

#if defined(UTILS_OS_MACOS)
#   include <sys/random.h>
#endif

namespace Utils {


inline void fill_with_random([[maybe_unused]] void* buffer, [[maybe_unused]] size_t length)
{
    #if defined(__pranaOS__)
        arc4random_buf(buffer, length);
    #elif defined(OSS_FUZZ)
    #elif defined(__unix__) or defined(AK_OS_MACOS)
         [[maybe_unused]] int rc = getentropy(buffer, length);
    #endif
}

}

using Utils::fill_with_random;