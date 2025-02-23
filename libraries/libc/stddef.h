/**
 * @file stddef.h
 * @author Krisna Pranav
 * @brief stddef
 * @version 6.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#define offsetof(type, member) __builtin_offsetof(type, member)

#ifndef KERNEL

#    include <sys/cdefs.h>

#    ifdef __cplusplus
#        define NULL nullptr
#    else
#        define NULL ((void*)0)
#    endif

typedef __PTRDIFF_TYPE__ ptrdiff_t;
typedef __SIZE_TYPE__ size_t;

#endif
