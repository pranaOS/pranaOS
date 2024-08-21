/**
 * @file cxxabi.cpp
 * @author Krisna Pranav
 * @brief CXXABI
 * @version 6.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/internals.h>
#include <mods/types.h>

extern "C" 
{

    struct __exit_entry 
    {
        AtExitFunction method;
        void* parameter;
        void* dso_handle;
        bool has_been_called;
    }; // struct __exit_entry

    /// @brief: exit_entries
    static __exit_entry __exit_entries[1024] {};

    /// @brief: __exit_entry_count setted as 0
    static int __exit_entry_count = 0;

    /**
     * @param exit_function 
     * @param parameter 
     * @param dso_handle 
     * @return int 
     */
    int __cxa_atexit(AtExitFunction exit_function, void* parameter, void* dso_handle)
    {
        if (__exit_entry_count >= 1024)
            return -1;

        __exit_entries[__exit_entry_count++] = { exit_function, parameter, dso_handle, false };

        return 0;
    }

    /**
     * @param dso_handle 
     */
    void __cxa_finalize(void* dso_handle)
    {
        int entry_index = __exit_entry_count;

    #ifdef GLOBAL_DTORS_DEBUG
        dbgprintf("__cxa_finalize: %d entries in the finalizer list\n", entry_index);
    #endif

        while (--entry_index >= 0) {
            auto& exit_entry = __exit_entries[entry_index];
            bool needs_calling = !exit_entry.has_been_called && (!dso_handle || dso_handle == exit_entry.dso_handle);
            if (needs_calling) {
    #ifdef GLOBAL_DTORS_DEBUG
                dbgprintf("__cxa_finalize: calling entry[%d] %p(%p) dso: %p\n", entry_index, exit_entry.method, exit_entry.parameter, exit_entry.dso_handle);
    #endif
                exit_entry.method(exit_entry.parameter);
                exit_entry.has_been_called = true;
            }
        }
    }

    [[noreturn]] void __cxa_pure_virtual()
    {
        ASSERT_NOT_REACHED();
    }

    extern u32 __stack_chk_guard;

    u32 __stack_chk_guard = (u32)0xc6c7c8c9;

    [[noreturn]] void __stack_chk_fail()
    {
        ASSERT_NOT_REACHED();
    }
}
