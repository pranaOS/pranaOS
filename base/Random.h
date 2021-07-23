/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Platform.h>
#include <base/Types.h>

#if defined(__pranaos__)
#    include <stdlib.h>
#endif

#if defined(__unix__)
#    include <unistd.h>
#endif

#if defined(__APPLE__)
#    include <sys/random.h>
#endif

namespace Base {

inline void fill_with_random([[maybe_unused]] void* buffer, [[maybe_unused]] size_t length)
{
#if defined(__pranaos__)
    arc4random_buf(buffer, length);
#elif defined(OSS_FUZZ)
#elif defined(__unix__) or defined(__APPLE__)
    [[maybe_unused]] int rc = getentropy(buffer, length);
#endif
}

}