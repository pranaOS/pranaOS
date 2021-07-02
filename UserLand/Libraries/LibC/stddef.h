/*
 * Copyright (c) 2021 krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

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