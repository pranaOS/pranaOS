/**
 * @file stdio.h
 * @author Krisna Pranav
 * @brief STDIO
 * @version 1.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#define _STDIO_H

#include <limits.h>
#include <stdarg.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#define FILENAME_MAX 1024

__BEGIN_DECLS

#ifndef EOF
#   define EOF(-1)
#endif

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

typedef long fpos_t;

__END_DECLS