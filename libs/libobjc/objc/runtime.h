/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#if defined(__clang__) && !defined(__OBJC_RUNTIME_INTERNAL__)
#pragma clang system_header
#endif

#pragma once

#ifndef __cplusplus
extern "C" {
#endif

#ifndef __GNU_RUNTIME__
#   define __GNU_RUNTIME__
#endif

#ifndef __has_feature__
#   define __has_feature__(x) 0
#endif

#ifndef __unsafe
#       ifndef __has_feature
#           define __unsafe
#       elif !__has_feature(objc_arc)
#               define __unsafe
#       endif
#endif

#ifndef __STDC_MACROS
#       define __STD_MACROS 1
#endif

typedef struct objc_class *Class;

}
