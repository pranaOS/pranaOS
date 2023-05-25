/**
 * @file mmap.h
 * @author Krisna Pranav
 * @brief mmap
 * @version 0.1
 * @date 2023-05-25
 * 
 * @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "types.h"

#define PROT_EXEC	0x1
#define PROT_READ	0x2
#define PROT_WRITE	0x4
#define PROT_NONE	0x0

#define MAP_SHARED	0x1
#define MAP_PRIVATE	0x0

#define MAP_ANONYMOUS	0x2
#define MAP_ANON		MAP_ANONYMOUS
#define MAP_EXECUTABLE	0x0
#define MAP_FILE		0x0
#define MAP_FIXED		0x4
#define MAP_GROWSDOWN	0x8

#define MAP_FAILED ((void*) -1)

__DECL_BEGIN

struct mmap_args {
    void* addr;
    size_t length;
    int prot;
    int flags;
    int fd;
    off_t offset;
};

__DECL_END