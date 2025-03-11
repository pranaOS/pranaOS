/**
 * @file cdefs.h
 * @author Krisna Pranav
 * @brief cdefs
 * @version 6.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#define _POSIX_VERSION 200809L

#ifdef __cplusplus
#    ifndef __BEGIN_DECLS
#        define __BEGIN_DECLS extern "C" {
#        define __END_DECLS }
#    endif
#else
#    ifndef __BEGIN_DECLS
#        define __BEGIN_DECLS
#        define __END_DECLS
#    endif
#endif

#undef __P
#define __P(a) a