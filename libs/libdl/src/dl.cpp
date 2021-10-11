/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libc/include/string.h>
#include <libutils/types.h>
#include <libdl/include/dl.h>
#include <libdl/include/dl_integeration.h>
#include <string.h>

/* dlerror text + retrived thread*/
__thread char* s_dlerror_text = NULL;
__thread bool s_dlerror_retrieved = false;

static void store_error(const String& error)
{
    free(s_dlerror_text);
    s_dlerror_text = strdup(error.characters());
    s_dlerror_retrieved = false;
}

int dlclose(void* handle)
{
    auto result = __dlclose(handle);
    if (result.is_error()) {
        store_error(result.error().text);
        return -1;
    }
    return 0;
}

char* dlerror()
{
    if (s_dlerror_retrieved) {
        free(s_dlerror_text);
        s_dlerror_text = nullptr;
    }
    s_dlerror_retrieved = true;
    return const_cast<char*>(s_dlerror_text);
}

void* dlopen(const char* filename, int flags)
{
    auto result = __dlopen(filename, flags);
    if (result.is_error()) {
        store_error(result.error().text);
        return nullptr;
    }
    return result.value();
}

void* dlsym(void* handle, const char* symbol_name)
{
    auto result = __dlsym(handle, symbol_name);
    if (result.is_error()) {
        store_error(result.error().text);
        return nullptr;
    }
    return result.value();
}

int dladdr(void* addr, Dl_info* info)
{
    auto result = __dladdr(addr, info);
    if (result.is_error()) {
        store_error(result.error().text);
        return 0;
    }
    return 1;
}