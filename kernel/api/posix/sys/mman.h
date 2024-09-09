/**
 * @file mman.h
 * @author Krisna Pranav
 * @brief mman
 * @version 6.0
 * @date 2024-09-09
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/sys/types.h>
#include <kernel/api/posix/time.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAP_FILE 0x00
#define MAP_SHARED 0x01
#define MAP_PRIVATE 0x02
#define MAP_FIXED 0x10
#define MAP_ANONYMOUS 0x20
#define MAP_ANON MAP_ANONYMOUS
#define MAP_STACK 0x40
#define MAP_NORESERVE 0x80
#define MAP_RANDOMIZED 0x100
#define MAP_PURGEABLE 0x200
#define MAP_FIXED_NOREPLACE 0x400

#define PROT_READ 0x1
#define PROT_WRITE 0x2
#define PROT_EXEC 0x4
#define PROT_NONE 0x0

#define MAP_FAILED ((void*)-1)

#define MADV_NORMAL 0x0
#define MADV_SET_VOLATILE 0x1
#define MADV_SET_NONVOLATILE 0x2
#define MADV_DONTNEED 0x3

#define POSIX_MADV_NORMAL MADV_NORMAL
#define POSIX_MADV_DONTNEED MADV_DONTNEED

#define MS_SYNC 1
#define MS_ASYNC 2
#define MS_INVALIDATE 4

#ifdef __cplusplus
}
#endif
