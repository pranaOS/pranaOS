/**
 * @file cxxabi.cpp
 * @author Krisna Pranav
 * @brief CXXABI
 * @version 6.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/bitmap.h>
#include <mods/checked.h>
#include <mods/debug.h>
#include <mods/format.h>
#include <mods/neverdestroyed.h>
#include <libc/bits/pthread_integration.h>
#include <libc/mallocdefs.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/internals.h>
#include <sys/mman.h>
#include <unistd.h>

extern "C" {

struct AtExitEntry {
    AtExitFunction method { nullptr };
    void* parameter { nullptr };
    void* dso_handle { nullptr };
}; // struct AtExitEntry

static size_t atexit_entry_region_capacity = PAGE_SIZE / sizeof(AtExitEntry);

/**
 * @param capacity 
 * @return size_t 
 */
static size_t atexit_region_bytes(size_t capacity = atexit_entry_region_capacity)
{
    return PAGE_ROUND_UP(capacity * sizeof(AtExitEntry));
}

static size_t atexit_next_capacity()
{
    size_t original_num_bytes = atexit_region_bytes();
    VERIFY(!Checked<size_t>::addition_would_overflow(original_num_bytes, PAGE_SIZE));
    return (original_num_bytes + PAGE_SIZE) / sizeof(AtExitEntry);
}

static AtExitEntry* atexit_entries;
static size_t atexit_entry_count = 0;
static pthread_mutex_t atexit_mutex = __PTHREAD_MUTEX_INITIALIZER;

static NeverDestroyed<Bitmap> atexit_called_entries;

static bool atexit_region_should_lock = false;

static void lock_atexit_handlers()
{
    if (atexit_region_should_lock && mprotect(atexit_entries, atexit_region_bytes(), PROT_READ) < 0) {
        perror("lock_atexit_handlers");
        _exit(1);
    }
}

static void unlock_atexit_handlers()
{
    if (atexit_region_should_lock && mprotect(atexit_entries, atexit_region_bytes(), PROT_READ | PROT_WRITE) < 0) {
        perror("unlock_atexit_handlers");
        _exit(1);
    }
}

void __begin_atexit_locking()
{
    atexit_region_should_lock = true;
    lock_atexit_handlers();
}

/**
 * @param exit_function 
 * @param parameter 
 * @param dso_handle 
 * @return int 
 */
int __cxa_atexit(AtExitFunction exit_function, void* parameter, void* dso_handle)
{
    __pthread_mutex_lock(&atexit_mutex);

    if (!atexit_entries) {
        atexit_entries = (AtExitEntry*)mmap(nullptr, atexit_region_bytes(), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
        if (atexit_entries == MAP_FAILED) {
            __pthread_mutex_unlock(&atexit_mutex);
            perror("__cxa_atexit mmap");
            _exit(1);
        }
    }

    if (atexit_entry_count >= atexit_entry_region_capacity) {
        size_t new_capacity = atexit_next_capacity();
        size_t new_atexit_region_size = atexit_region_bytes(new_capacity);
        dbgln_if(GLOBAL_DTORS_DEBUG, "__cxa_atexit: Growing exit handler region from {} entries to {} entries", atexit_entry_region_capacity, new_capacity);

        auto* new_atexit_entries = (AtExitEntry*)mmap(nullptr, new_atexit_region_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
        if (new_atexit_entries == MAP_FAILED) {
            __pthread_mutex_unlock(&atexit_mutex);
            perror("__cxa_atexit mmap (new size)");
            return -1;
        }

        memcpy(new_atexit_entries, atexit_entries, atexit_entry_count * sizeof(AtExitEntry));
        if (munmap(atexit_entries, atexit_region_bytes()) < 0) {
            perror("__cxa_atexit munmap old region");
        }
        atexit_entries = new_atexit_entries;
        atexit_entry_region_capacity = new_capacity;
    }

    unlock_atexit_handlers();
    atexit_entries[atexit_entry_count++] = { exit_function, parameter, dso_handle };
    lock_atexit_handlers();

    __pthread_mutex_unlock(&atexit_mutex);

    return 0;
}

/**
 * @param dso_handle 
 */
void __cxa_finalize(void* dso_handle)
{
    __pthread_mutex_lock(&atexit_mutex);

    if (atexit_entry_count > atexit_called_entries->size())
        atexit_called_entries->grow(atexit_entry_count, false);

    ssize_t entry_index = atexit_entry_count;

    dbgln_if(GLOBAL_DTORS_DEBUG, "__cxa_finalize: {} entries in the finalizer list", entry_index);

    while (--entry_index >= 0) {
        auto& exit_entry = atexit_entries[entry_index];
        bool needs_calling = !atexit_called_entries->get(entry_index) && (!dso_handle || dso_handle == exit_entry.dso_handle);
        if (needs_calling) {
            dbgln_if(GLOBAL_DTORS_DEBUG, "__cxa_finalize: calling entry[{}] {:p}({:p}) dso: {:p}", entry_index, exit_entry.method, exit_entry.parameter, exit_entry.dso_handle);
            atexit_called_entries->set(entry_index, true);
            __pthread_mutex_unlock(&atexit_mutex);
            exit_entry.method(exit_entry.parameter);
            __pthread_mutex_lock(&atexit_mutex);
        }
    }

    __pthread_mutex_unlock(&atexit_mutex);
}


__attribute__((noreturn)) void __cxa_pure_virtual()
{
    VERIFY_NOT_REACHED();
}

} // extern "C"
