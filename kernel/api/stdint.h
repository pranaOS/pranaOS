/**
 * @file stdint.h
 * @author Krisna Pranav
 * @brief stdint
 * @version 0.1
 * @date 2023-05-25
 * 
 * @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "cdefs.h"

__DECL_BEGIN 

#if defined(__GNUC__) && \
  ( (__GNUC__ >= 4) || \
	( (__GNUC__ >= 3) && defined(__GNUC_MINOR__) && (__GNUC_MINOR__ > 2) ) )
#define __STDINT_EXP(x) __##x##__
#else
#define __STDINT_EXP(x) x
#include <limits.h>
#endif

#if __STDINT_EXP(SCHAR_MAX) == 0x7f
typedef signed char int8_t;
typedef unsigned char uint8_t;
#define __int8_t_defined 1
#endif 

__DECL_END 