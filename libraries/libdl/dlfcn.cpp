/**
 * @file dlfcn.cpp
 * @author Krisna Pranav
 * @brief dlfcn
 * @version 6.0
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/string.h>
#include <mods/types.h>
#include <libdl/dlfcn.h>
#include <libdl/dlfcn_integration.h>
#include <string.h>

__thread char* s_dlerror_text = NULL;
__thread bool s_dlerror_retrieved = false;

/**
 * @param error 
 */
static void store_error(const String& error)
{
    free(s_dlerror_text);
    s_dlerror_text = strdup(error.characters());
    s_dlerror_retrieved = false;
}

/**
 * @param handle 
 * @return int 
 */
int dlclose(void* handle)
{
    auto result = __dlclose(handle);
    if (result.is_error()) {
        store_error(result.error().text);
        return -1;
    }
    return 0;
}

/**
 * @return char* 
 */
char* dlerror()
{
    if (s_dlerror_retrieved) {
        free(s_dlerror_text);
        s_dlerror_text = nullptr;
    }

    s_dlerror_retrieved = true;
    return const_cast<char*>(s_dlerror_text);
}

/**
 * @param filename 
 * @param flags 
 * @return void* 
 */
void* dlopen(const char* filename, int flags)
{
    auto result = __dlopen(filename, flags);
    if (result.is_error()) {
        store_error(result.error().text);
        return nullptr;
    }
    return result.value();
}

/**
 * @param handle 
 * @param symbol_name 
 * @return void* 
 */
void* dlsym(void* handle, const char* symbol_name)
{
    auto result = __dlsym(handle, symbol_name);
    if (result.is_error()) {
        store_error(result.error().text);
        return nullptr;
    }
    return result.value();
}


/**
 * @param addr 
 * @param info 
 * @return int 
 */
int dladdr(void* addr, Dl_info* info)
{
    auto result = __dladdr(addr, info);
    
    if (result.is_error()) {
        store_error(result.error().text);
        return 0;
    }
    return 1;
}
