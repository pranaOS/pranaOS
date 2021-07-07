/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

extern "C"
{
    typedef void (*AtExitFunction)(void *);

    struct __exit_entry
    {
        AtExitFunction method;
        void *parameter;
        void *dso_handle;
        bool has_been_called;
    };

    static __exit_entry __exit_entries[1024]{};
    static int __exit_entry_count = 0;

    int __cxa_atexit(AtExitFunction exit_function, void *parameter, void *dso_handle)
    {
        if (__exit_entry_count >= 1024)
        {
            return -1;
        }

        __exit_entries[__exit_entry_count++] = {exit_function, parameter, dso_handle, false};

        return 0;
    }

    void __cxa_finalize(void *dso_handle)
    {

        int entry_index = __exit_entry_count;

        while (--entry_index >= 0)
        {
            auto &exit_entry = __exit_entries[entry_index];
            bool needs_calling = !exit_entry.has_been_called && (!dso_handle || dso_handle == exit_entry.dso_handle);
            if (needs_calling)
            {
                exit_entry.method(exit_entry.parameter);
                exit_entry.has_been_called = true;
            }
        }
    }

}  