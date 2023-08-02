/**
 * @file limits.h
 * @author Krisna Pranav
 * @brief limits
 * @version 1.0
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <bits/stdint.h>

#ifndef PAGE_SIZE
#    define PAGE_SIZE 4096
#endif

#define PATH_MAX 4096
#if !defined MAXPATHLEN && defined PATH_MAX
#    define MAXPATHLEN PATH_MAX
#endif

#define NAME_MAX 255

#define PIPE_BUF 4096

#define INT_MAX INT32_MAX
#define INT_MIN INT32_MIN

#define UINT_MAX UINT32_MAX
#define UINT_MIN UINT32_MIN

#define CHAR_BIT 8
#define SCHAR_MIN (-128)
#define SCHAR_MAX 127
#define UCHAR_MAX 255

#define LONG_MAX 2147483647L
#define LONG_MIN (-LONG_MAX - 1L)

#define ULONG_MAX 4294967295UL

#define LONG_LONG_MAX 9223372036854775807LL
#define LONG_LONG_MIN (-LONG_LONG_MAX - 1LL)

#define ULONG_LONG_MAX 18446744073709551615ULL

#define CHAR_MIN SCHAR_MIN
#define CHAR_MAX SCHAR_MAX

#define MB_LEN_MAX 16

#define ARG_MAX 65536

#define PTHREAD_STACK_MIN 65536

#define SSIZE_MAX 2147483647

#ifdef __USE_POSIX
#    include <bits/posix1_lim.h>
#endif
