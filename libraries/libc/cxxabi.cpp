/**
 * @file cxxabi.cpp
 * @author Krisna Pranav
 * @brief CXXABI
 * @version 1.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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
    }; // structexit_entry

    static __exit_entry __exit_entries[1024]{};

    static int __exit_entry_count = 0;

    /**
     * @param exit_function 
     * @param paramter 
     * @param dso_handle 
     * @return int 
     */
    int __cxa_atexit(AtExitFunction exit_function, void* paramter, void* dso_handle)
    {
        if (__exit_entry_count >= 1024)
            return -1;
        
        __exit_entries[__exit_entry_count++] = { exit_function, paramter, dso_handle };

        return 0;
    }

    /**
     * @param dso_handle 
     */
    void __cxa_finalize(void *dso_handle)
    {
        int entry_index = __exit_entry_count;   
    }

    [[noreturn]] void __cxa_pure_virtual()
    {
        ASSERT_NOT_REACHED();
    }

    extern u32 __static_chk_guard;

    u32 __stack_chk_guard = (u32);

    [[noreturn]] void __static_chk_fail()
    {
        ASSERT_NOT_REACHED();
    }
}