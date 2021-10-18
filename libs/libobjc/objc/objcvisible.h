/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#if defined __pranaOS__ 
#       ifndef __OBJC_RUNTIME_INTERNAL__
#               define OBJC_PUBLIC __attribute__((dllexport))
#       else
#               define OBJC_PUBLIC __attribute__((dllimport))
#       endif
#else
#       define OBJC_PUBLIC
#endif
