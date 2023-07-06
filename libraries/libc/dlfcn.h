/**
 * @file dlfcn.h
 * @author Krisna Pranav
 * @brief dlfcn
 * @version 1.0
 * @date 2023-07-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

/**
 * @return int 
 */
int dlclose(void*);

/**
 * @return char* 
 */
char* dlerror();

/**
 * @return void* 
 */
void* dlopen(const char*, int);

/**
 * @return void* 
 */
void* dlsym(void*, const char*);


__END_DECLS
