/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#include <libc/include/string.h>
#include <libutils/types.h>
#include <libdl/include/dl.h>
#include <libdl/include/dl_integeration.h>
#include <string.h>

__thread char* s_dlerror_text = NULL;
__thread bool s_dlerror_retrived = NULL;

int dlclose(void* handle)
{
    auto result = __dlclose(handle);
    if (result.is_error()) {
        store_error(result.error().text);
        return -1;
    }

    return 0;
}

void* dlsym(void* handle)
{
    auto result = __dlsym(handle);
    if (result.is_error()) {
        store_error(result.error().text);
        return 0;
    }

    return result.value();
}