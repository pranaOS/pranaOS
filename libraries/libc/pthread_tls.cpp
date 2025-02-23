/**
 * @file pthread_tls.cpp
 * @author Krisna Pranav
 * @brief pthrea tls
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/atomic.h>
#include <libpthread/pthread.h>
#include <errno.h>
#include <unistd.h>

#ifndef _DYNAMIC_LOADER
extern "C" {

static constexpr int max_keys = PTHREAD_KEYS_MAX;

struct KeyTable {
    KeyDestructor destructors[max_keys] { nullptr };
    int next { 0 };
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
}; // struct KeyTable

struct SpecificTable {
    void* values[max_keys] { nullptr };
}; // struct SpecificTable

static KeyTable s_keys;

__thread SpecificTable t_specifics;

/**
 * @param key 
 * @param destructor 
 * @return int 
 */
int __pthread_key_create(pthread_key_t* key, KeyDestructor destructor)
{
    int ret = 0;
    __pthread_mutex_lock(&s_keys.mutex);
    if (s_keys.next >= max_keys) {
        ret = EAGAIN;
    } else {
        *key = s_keys.next++;
        s_keys.destructors[*key] = destructor;
        ret = 0;
    }
    __pthread_mutex_unlock(&s_keys.mutex);
    return ret;
}

int pthread_key_create(pthread_key_t*, KeyDestructor) __attribute__((weak, alias("__pthread_key_create")));

/**
 * @param key 
 * @return int 
 */
int __pthread_key_delete(pthread_key_t key)
{
    if (key < 0 || key >= max_keys)
        return EINVAL;
    __pthread_mutex_lock(&s_keys.mutex);
    s_keys.destructors[key] = nullptr;
    __pthread_mutex_unlock(&s_keys.mutex);
    return 0;
}

int pthread_key_delete(pthread_key_t) __attribute__((weak, alias("__pthread_key_delete")));

/**
 * @param key 
 * @return void* 
 */
void* __pthread_getspecific(pthread_key_t key)
{
    if (key < 0)
        return nullptr;
    if (key >= max_keys)
        return nullptr;
    return t_specifics.values[key];
}

void* pthread_getspecific(pthread_key_t) __attribute__((weak, alias("__pthread_getspecific")));

/**
 * @param key 
 * @param value 
 * @return int 
 */
int __pthread_setspecific(pthread_key_t key, void const* value)
{
    if (key < 0)
        return EINVAL;
    if (key >= max_keys)
        return EINVAL;

    t_specifics.values[key] = const_cast<void*>(value);
    return 0;
}

int pthread_setspecific(pthread_key_t, void const*) __attribute__((weak, alias("__pthread_setspecific")));

void __pthread_key_destroy_for_current_thread()
{
    __pthread_mutex_lock(&s_keys.mutex);
    size_t num_used_keys = s_keys.next;

    for (size_t destruct_iteration = 0; destruct_iteration < PTHREAD_DESTRUCTOR_ITERATIONS; ++destruct_iteration) {
        bool any_nonnull_destructors = false;
        for (size_t key_index = 0; key_index < num_used_keys; ++key_index) {
            void* value = exchange(t_specifics.values[key_index], nullptr);

            if (value && s_keys.destructors[key_index]) {
                any_nonnull_destructors = true;
                (*s_keys.destructors[key_index])(value);
            }
        }
        if (!any_nonnull_destructors)
            break;
    }
    __pthread_mutex_unlock(&s_keys.mutex);
}
}
#endif
