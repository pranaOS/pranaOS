/**
 * @file platform.h
 * @author Krisna Pranav
 * @brief Platform
 * @version 6.0
 * @date 2023-06-27
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#ifdef __i386__
#    define MOD_ARCH_I386 1
#endif

#ifdef __x86_64__
#    define MOD_ARCH_X86_64 1
#endif

#define ARCH(arch) (defined(MOD_ARCH_##arch) && MOD_ARCH_##arch)

#ifdef ALWAYS_INLINE
#    undef ALWAYS_INLINE
#endif
#define ALWAYS_INLINE [[gnu::always_inline]] inline

#ifdef NEVER_INLINE
#    undef NEVER_INLINE
#endif
#define NEVER_INLINE [[gnu::noinline]]

#ifdef FLATTEN
#    undef FLATTEN
#endif
#define FLATTEN [[gnu::flatten]]

#ifndef __pranaos__
#    define PAGE_SIZE sysconf(_SC_PAGESIZE)

#    include <errno.h>
#    include <fcntl.h>
#    include <stdlib.h>
#    include <string.h>
inline int open_with_path_length(const char* path, size_t path_length, int options, mode_t mode) {
    auto* tmp = (char*)malloc(path_length + 1);
    memcpy(tmp, path, path_length);
    tmp[path_length] = '\0';
    int fd = open(tmp, options, mode);
    int saved_errno = errno;
    free(tmp);
    errno = saved_errno;
    return fd;
}
#endif

ALWAYS_INLINE int count_trailing_zeroes_32(unsigned int val) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_ctz(val);
#else
    for (u8 i = 0; i < 32; ++i) {
        if ((val >> i) & 1) {
            return i;
        }
    }
    return 0;
#endif
}
