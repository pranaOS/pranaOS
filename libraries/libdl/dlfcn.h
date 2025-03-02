/**
 * @file dlfcn.h
 * @author Krisna Pranav
 * @brief dlfcn
 * @version 6.0
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

#define RTLD_DEFAULT 0
#define RTLD_LAZY 2
#define RTLD_NOW 4
#define RTLD_GLOBAL 8
#define RTLD_LOCAL 16

typedef struct __Dl_info 
{
    const char* dli_fname;
    void* dli_fbase;
    const char* dli_sname;
    void* dli_saddr;
} Dl_info;

/**
 * @return int 
 */
int dlclose(void*);

/**
 * @return char* 
 */
char* dlerror(void);

/**
 * @return void* 
 */
void* dlopen(const char*, int);

/**
 * @return void* 
 */
void* dlsym(void*, const char*);

/**
 * @return int 
 */
int dladdr(void*, Dl_info*);

__END_DECLS
