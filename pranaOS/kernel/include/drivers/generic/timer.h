/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifdef __i386__
#include <drivers/x86/pit.h>
#elif __arm__
#include <drivers/aarch32/sp804.h>
#endif