/**
 * @file types.h
 * @author Krisna Pranav
 * @brief types
 * @version 0.1
 * @date 2023-05-25
 * 
 * @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "cdefs.h"
#include "stdint.h"

__DECL_BEGIN

typedef uint32_t size_t;
typedef int32_t ssize_t;
typedef int pid_t;
typedef int tid_t;
typedef unsigned long ino_t;
typedef short dev_t;
typedef uint32_t mode_t;
typedef unsigned short nlink_t;
typedef unsigned short uid_t;
typedef unsigned short gid_t;

typedef long suseconds_t;
typedef unsigned long useconds_t;

__DECL_END