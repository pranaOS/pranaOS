/**
 * @file pthread_integration.cpp
 * @author Krisna Pranav
 * @brief pthread integration
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/atomic.h>
#include <mods/neverdestroyed.h>
#include <mods/types.h>
#include <mods/vector.h>
#include <bits/pthread_integration.h>
#include <errno.h>
#include <sched.h>
#include <pranaos.h>
#include <unistd.h>

namespace 
{

    static Atomic<bool> g_did_touch_atfork { false };
    static pthread_mutex_t g_atfork_list_mutex __PTHREAD_MUTEX_INITIALIZER;
    static NeverDestroyed<Vector<void (*)(void), 4>> g_atfork_prepare_list;
    static NeverDestroyed<Vector<void (*)(void), 4>> g_atfork_child_list;
    static NeverDestroyed<Vector<void (*)(void), 4>> g_atfork_parent_list;

} // namespace

extern "C" {

void __pthread_fork_prepare(void)
{
    if (!g_did_touch_atfork.load())
        return;

    __pthread_mutex_lock(&g_atfork_list_mutex);
    for (auto entry : g_atfork_prepare_list.get())
        entry();
    __pthread_mutex_unlock(&g_atfork_list_mutex);
}

void __pthread_fork_child(void)
{
    if (!g_did_touch_atfork.load())
        return;

    __pthread_mutex_lock(&g_atfork_list_mutex);
    for (auto entry : g_atfork_child_list.get())
        entry();
    __pthread_mutex_unlock(&g_atfork_list_mutex);
}

void __pthread_fork_parent(void)
{
    if (!g_did_touch_atfork.load())
        return;

    __pthread_mutex_lock(&g_atfork_list_mutex);
    for (auto entry : g_atfork_parent_list.get())
        entry();
    __pthread_mutex_unlock(&g_atfork_list_mutex);
}

void __pthread_fork_atfork_register_prepare(void (*func)(void))
{
    g_did_touch_atfork.store(true);

    __pthread_mutex_lock(&g_atfork_list_mutex);
    g_atfork_prepare_list->append(func);
    __pthread_mutex_unlock(&g_atfork_list_mutex);
}

void __pthread_fork_atfork_register_parent(void (*func)(void))
{
    g_did_touch_atfork.store(true);

    __pthread_mutex_lock(&g_atfork_list_mutex);
    g_atfork_parent_list->append(func);
    __pthread_mutex_unlock(&g_atfork_list_mutex);
}

void __pthread_fork_atfork_register_child(void (*func)(void))
{
    g_did_touch_atfork.store(true);

    __pthread_mutex_lock(&g_atfork_list_mutex);
    g_atfork_child_list->append(func);
    __pthread_mutex_unlock(&g_atfork_list_mutex);
}

int __pthread_self()
{
    return gettid();
}

int pthread_self() __attribute__((weak, alias("__pthread_self")));

static constexpr u32 MUTEX_UNLOCKED = 0;
static constexpr u32 MUTEX_LOCKED_NO_NEED_TO_WAKE = 1;
static constexpr u32 MUTEX_LOCKED_NEED_TO_WAKE = 2;

/**
 * @param mutex 
 * @param attributes 
 * @return int 
 */
int __pthread_mutex_init(pthread_mutex_t* mutex, pthread_mutexattr_t const* attributes)
{
    mutex->lock = 0;
    mutex->owner = 0;
    mutex->level = 0;
    mutex->type = attributes ? attributes->type : __PTHREAD_MUTEX_NORMAL;
    return 0;
}

int pthread_mutex_init(pthread_mutex_t*, pthread_mutexattr_t const*) __attribute__((weak, alias("__pthread_mutex_init")));

/**
 * @param mutex 
 * @return int 
 */
int __pthread_mutex_trylock(pthread_mutex_t* mutex)
{
    u32 expected = MUTEX_UNLOCKED;
    bool exchanged = Mods::atomic_compare_exchange_strong(&mutex->lock, expected, MUTEX_LOCKED_NO_NEED_TO_WAKE, Mods::memory_order_acquire);

    if (exchanged) [[likely]] {
        if (mutex->type == __PTHREAD_MUTEX_RECURSIVE)
            Mods::atomic_store(&mutex->owner, __pthread_self(), Mods::memory_order_relaxed);
        mutex->level = 0;
        return 0;
    } else if (mutex->type == __PTHREAD_MUTEX_RECURSIVE) {
        pthread_t owner = Mods::atomic_load(&mutex->owner, Mods::memory_order_relaxed);
        if (owner == __pthread_self()) {
            // We already own the mutex!
            mutex->level++;
            return 0;
        }
    }
    return EBUSY;
}

int pthread_mutex_trylock(pthread_mutex_t* mutex) __attribute__((weak, alias("__pthread_mutex_trylock")));

/**
 * @param mutex 
 * @return int 
 */
int __pthread_mutex_lock(pthread_mutex_t* mutex)
{
    u32 value = MUTEX_UNLOCKED;
    bool exchanged = Mods::atomic_compare_exchange_strong(&mutex->lock, value, MUTEX_LOCKED_NO_NEED_TO_WAKE, Mods::memory_order_acquire);
    if (exchanged) [[likely]] {
        if (mutex->type == __PTHREAD_MUTEX_RECURSIVE)
            Mods::atomic_store(&mutex->owner, __pthread_self(), Mods::memory_order_relaxed);
        mutex->level = 0;
        return 0;
    } else if (mutex->type == __PTHREAD_MUTEX_RECURSIVE) {
        pthread_t owner = Mods::atomic_load(&mutex->owner, Mods::memory_order_relaxed);
        if (owner == __pthread_self()) {
            mutex->level++;
            return 0;
        }
    }

    if (value != MUTEX_LOCKED_NEED_TO_WAKE)
        value = Mods::atomic_exchange(&mutex->lock, MUTEX_LOCKED_NEED_TO_WAKE, Mods::memory_order_acquire);

    while (value != MUTEX_UNLOCKED) {
        futex_wait(&mutex->lock, value, nullptr, 0);
        value = Mods::atomic_exchange(&mutex->lock, MUTEX_LOCKED_NEED_TO_WAKE, Mods::memory_order_acquire);
    }

    if (mutex->type == __PTHREAD_MUTEX_RECURSIVE)
        Mods::atomic_store(&mutex->owner, __pthread_self(), Mods::memory_order_relaxed);
    mutex->level = 0;
    return 0;
}

int pthread_mutex_lock(pthread_mutex_t*) __attribute__((weak, alias("__pthread_mutex_lock")));

/**
 * @param mutex 
 * @return int 
 */
int __pthread_mutex_lock_pessimistic_np(pthread_mutex_t* mutex)
{
    u32 value = Mods::atomic_exchange(&mutex->lock, MUTEX_LOCKED_NEED_TO_WAKE, Mods::memory_order_acquire);
    while (value != MUTEX_UNLOCKED) {
        futex_wait(&mutex->lock, value, nullptr, 0);
        value = Mods::atomic_exchange(&mutex->lock, MUTEX_LOCKED_NEED_TO_WAKE, Mods::memory_order_acquire);
    }

    if (mutex->type == __PTHREAD_MUTEX_RECURSIVE)
        Mods::atomic_store(&mutex->owner, __pthread_self(), Mods::memory_order_relaxed);
    mutex->level = 0;
    return 0;
}

/**
 * @param mutex 
 * @return int 
 */
int __pthread_mutex_unlock(pthread_mutex_t* mutex)
{
    if (mutex->type == __PTHREAD_MUTEX_RECURSIVE && mutex->level > 0) {
        mutex->level--;
        return 0;
    }

    if (mutex->type == __PTHREAD_MUTEX_RECURSIVE)
        Mods::atomic_store(&mutex->owner, 0, Mods::memory_order_relaxed);

    u32 value = Mods::atomic_exchange(&mutex->lock, MUTEX_UNLOCKED, Mods::memory_order_release);
    if (value == MUTEX_LOCKED_NEED_TO_WAKE) [[unlikely]] {
        int rc = futex_wake(&mutex->lock, 1);
        VERIFY(rc >= 0);
    }

    return 0;
}

int pthread_mutex_unlock(pthread_mutex_t*) __attribute__((weak, alias("__pthread_mutex_unlock")));
}
