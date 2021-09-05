/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#if defined(__cplusplus)
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#else
#define __BEGIN_DECLS
#define __END_DECLS
#endif

/* Define __use_instead macro for some functions so the user can be warned
   about better/faster functions. */
#ifndef __use_instead
#ifdef __clang__
#define __use_instead(F) __attribute__((diagnose_if(1, "use " F " instead", \
    "warning")))
#elif defined(__GNUC__)
#define __use_instead(F) __attribute__((warning("use " F " instead")))
#endif
#endif
