/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/platform.h>
#include <libutils/types.h>

#if defined(__pranaOS__)
#    include <stdlib.h>
#endif

#if defined(__unix__)
#    include <unistd.h>
#endif

#if defined(UTILS_OS_MACOS)
#    include <sys/random.h>
#endif

namespace Utils {

inline void fill_with_random([[maybe_unused]] void* buffer, [[maybe_unused]] size_t length)
{
#if defined(__serenity__)
    arc4random_buf(buffer, length);
#elif defined(OSS_FUZZ)
#elif defined(__unix__) or defined(UTILS_OS_MACOS)
    [[maybe_unused]] int rc = getentropy(buffer, length);
#endif
}

template<typename T>
inline T get_random()
{
    T t;
    fill_with_random(&t, sizeof(T));
    return t;
}

u32 get_random_uniform(u32 max_bounds);

}

using Utils::fill_with_random;
using Utils::get_random;
using Utils::get_random_uniform;