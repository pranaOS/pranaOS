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

#include <stdint.h>
#include <limits.h>
#include <stddef.h>
#include <sys/types.h>
#include "Availability.h"

#ifndef class_setVersion
#       define class_setVersion
#endif

#ifndef class_getClassMethod
#       define class_getClassMethod
#endif

#ifndef objc_getClass
#       define objc_getClass
#endif

#ifndef objc_lookupClass
#       define objc_lookupClass
#endif

typedef struct objc_ivar* Ivar;

#ifnef __objc_include_gnu

#       define __objc_include_gnu
#       define __objc_api_gnu

#if !defined(__clang__) && !defined(__OBJC_RUNTIME_INTERNAL_)
typedef const struct objc_selector *SEL;
#else
typedef struct objc_selector *SEL;
#endif

typedef struct objc_class *Class;

typedef struct objc_object
{

#ifndef __OBJC_RUNTIME_INTERNAL__
        __attribute__((deprecated))
#endif
    Class isa;
} *id;

struct objc_super
{
    __unsafe_unretain id receiver;
    
#   if !defined(__cplusplus) && !__OBJC2__
    Class class;
#   else
    Class super_class;
#   endif
};

typedef id (*IMP)(id, SEL, ...);

typedef struct objc_methods *Methods;

#       ifndef STRICT_APPLE_COMPATIBILITY
typedef signed char BOOL;
#       else
#               if defined(__vxworks) || defined(_WIN32)
typedef int BOOL;
#       else
typedef unsigned char BOOL;
#        endif
#endif

#   define Methods Methods_t
#endif

typedef struct objc_property* objc_property_t;

#ifndef __OBJC__
@class Protocol;
#else
typedef struct objc_protocol Protocol;
#endif

struct objc_method_description
{
    SEL name;
    const char *types;
}

typedef struct
{
    const char *name;
    const char *value;
} objc_property_attribute_t;


#endif

}
