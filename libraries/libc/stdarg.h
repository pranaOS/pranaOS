/**
 * @file stdarg.h
 * @author Krisna Pranav
 * @brief stdarg
 * @version 6.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#if defined(KERNEL)
#    define __BEGIN_DECLS
#    define __END_DECLS
#else
#    include <sys/cdefs.h>
#endif

__BEGIN_DECLS

typedef __builtin_va_list va_list;

#define va_start(v, l) __builtin_va_start(v, l)
#define va_end(v) __builtin_va_end(v)
#define va_arg(v, l) __builtin_va_arg(v, l)

__END_DECLS
