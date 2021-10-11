/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <sys/cdefs.h>

__BEGIN_DECLS

/* defined RTLD */
#define RTLD_DEFAULT 0
#define RTLD_LAZY 2
#define RTLD_NOW 4
#define RTLD_GLOBAL 8
#define RTLD_LOCAL 16

/* dl info structure */
typedef struct __Dl_info {
   const char* dli_fname;
   void* dli_fbase;
   const char* dli_sname;
   void* dli_saddr;
} Dl_info;

int dlclose(void*);
char* dlerror();
void* dlopen(const char*, int);
void* dlsym(void*, const char*);
int dladdr(void*, Dl_info*);

__END_DECLS