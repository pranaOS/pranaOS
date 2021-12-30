extern "C"
{
    #define AT_EXIT_ENTRIES_MAX 1024

    typedef void(*atexit_function_t)(void*);

    struct atexit_entry
    {
        atexit_function_t func;
        void* obj_ptr;
        void* dso_handle;
    };
    
    static atexit_entry atexit_entries[AT_EXIT_ENTRIES_MAX] = {};
    static int atexit_entry_count = 0;

    void* __dso_handle;

    int __cxa_atexit(atexit_function_t func, void* obj_ptr, void* dso_handle)
    {
        if(atexit_entry_count >= AT_EXIT_ENTRIES_MAX) return -1;

        atexit_entries[atexit_entry_count] = { func, obj_ptr, dso_handle };
        atexit_entry_count++;

        return 0;
    }

    void __cxa_finalize(void* dso_handle)
    {
        int i = atexit_entry_count;

        if(dso_handle == nullptr)
        {
            while(i--)
            {
                if(atexit_entries[i].func)
                {
                    (*atexit_entries[i].func)(atexit_entries[i].obj_ptr);
                }
            }

            return;
        }

        while(i--)
        {
            if(atexit_entries[i].func == dso_handle)
            {
                (*atexit_entries[i].func)(atexit_entries[i].obj_ptr);
                atexit_entries[i].func = 0;
            }
        }
    }
}
