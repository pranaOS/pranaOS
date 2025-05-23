/**
 * @file pranaos.h
 * @author Krisna Pranav
 * @brief pranaos
 * @version 6.0
 * @date 2024-09-07
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/api/posix/sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PURGE_ALL_VOLATILE 0x1
#define PURGE_ALL_CLEAN_INODE 0x2

enum 
{
    PERF_EVENT_SAMPLE = 1,
    PERF_EVENT_MALLOC = 2,
    PERF_EVENT_FREE = 4,
    PERF_EVENT_MMAP = 8,
    PERF_EVENT_MUNMAP = 16,
    PERF_EVENT_PROCESS_CREATE = 32,
    PERF_EVENT_PROCESS_EXEC = 64,
    PERF_EVENT_PROCESS_EXIT = 128,
    PERF_EVENT_THREAD_CREATE = 256,
    PERF_EVENT_THREAD_EXIT = 512,
    PERF_EVENT_CONTEXT_SWITCH = 1024,
    PERF_EVENT_KMALLOC = 2048,
    PERF_EVENT_KFREE = 4096,
    PERF_EVENT_PAGE_FAULT = 8192,
    PERF_EVENT_SYSCALL = 16384,
    PERF_EVENT_SIGNPOST = 32768,
    PERF_EVENT_READ = 65536,
};

#define PERF_EVENT_MASK_ALL (~0ull)

#define THREAD_PRIORITY_MIN 1
#define THREAD_PRIORITY_LOW 10
#define THREAD_PRIORITY_NORMAL 30
#define THREAD_PRIORITY_HIGH 50
#define THREAD_PRIORITY_MAX 99

#ifdef __cplusplus
}
#endif
