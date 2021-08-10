/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifdef __i386__
#include <platform/x86/tasking/signal_impl.h>
#elif __arm__
#include <platform/aarch32/tasking/signal_impl.h>
#endif