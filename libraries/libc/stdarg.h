/**
 * @file stdarg.h
 * @author Krisna Pranav
 * @brief stdarg
 * @version 1.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once 


#if defined(KERNEL)
#   define __BEGIN_DECLS
#   define __END_DECLS
#else   
#   include <sys/cdefs.h>
#endif

__BEGIN_DECLS

typedef __builtin_va_list va_list;

__END_DECLS
