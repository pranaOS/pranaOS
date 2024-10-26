/**
 * @file pthread_cond.cpp
 * @author Krisna Pranav
 * @brief Pthread Cond
 * @version 6.0
 * @date 2024-10-26
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/assertions.h>
#include <mods/atomic.h>
#include <mods/types.h>
#include <errno.h>
#include <pthread.h>
#include <pranaos.h>
#include <sys/types.h>
#include <time.h>

/**
 * @param attr 
 * @return int 
 */
int pthread_condattr_init(pthread_condattr_t* attr)
{
    attr->clockid = CLOCK_MONOTONIC_COARSE;
    return 0;
}

/**
 * @return int 
 */
int pthread_condattr_destroy(pthread_condattr_t*)
{
    return 0;
}

/**
 * @param attr 
 * @param clock 
 * @return int 
 */
int pthread_condattr_getclock(pthread_condattr_t* attr, clockid_t* clock)
{
    *clock = attr->clockid;
    return 0;
}

/**
 * @param attr 
 * @param clock 
 * @return int 
 */
int pthread_condattr_setclock(pthread_condattr_t* attr, clockid_t clock)
{
    switch (clock) {
    case CLOCK_REALTIME:
    case CLOCK_REALTIME_COARSE:
    case CLOCK_MONOTONIC:
    case CLOCK_MONOTONIC_COARSE:
    case CLOCK_MONOTONIC_RAW:
        attr->clockid = clock;
        return 0;
    default:
        return EINVAL;
    }
}

static constexpr u32 NEED_TO_WAKE_ONE = 1;
static constexpr u32 NEED_TO_WAKE_ALL = 2;
static constexpr u32 INCREMENT = 4;

/**
 * @param cond 
 * @param attr 
 * @return int 
 */
int pthread_cond_init(pthread_cond_t* cond, const pthread_condattr_t* attr)
{
    cond->mutex = nullptr;
    cond->value = 0;
    cond->clockid = attr ? attr->clockid : CLOCK_REALTIME_COARSE;
    return 0;
}

/**
 * @return int 
 */
int pthread_cond_destroy(pthread_cond_t*)
{
    return 0;
}

/**
 * @param cond 
 * @param mutex 
 * @return int 
 */
int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex)
{
    return pthread_cond_timedwait(cond, mutex, nullptr);
}

/**
 * @param cond 
 * @param mutex 
 * @param abstime 
 * @return int 
 */
int pthread_cond_timedwait(pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* abstime)
{
    pthread_mutex_t* old_mutex = Mods::atomic_exchange(&cond->mutex, mutex, Mods::memory_order_relaxed);

    if (old_mutex && old_mutex != mutex)
        TODO();

    u32 value = Mods::atomic_fetch_or(&cond->value, NEED_TO_WAKE_ONE | NEED_TO_WAKE_ALL, Mods::memory_order_release) | NEED_TO_WAKE_ONE | NEED_TO_WAKE_ALL;
    pthread_mutex_unlock(mutex);
    int rc = futex_wait(&cond->value, value, abstime, cond->clockid);
    if (rc < 0 && errno != EAGAIN)
        return errno;

    __pthread_mutex_lock_pessimistic_np(mutex);
    return 0;
}

/**
 * @param cond 
 * @return int 
 */
int pthread_cond_signal(pthread_cond_t* cond)
{
    u32 value = Mods::atomic_fetch_add(&cond->value, INCREMENT, Mods::memory_order_relaxed);

    if (!(value & NEED_TO_WAKE_ONE)) [[likely]]
        return 0;

    value = Mods::atomic_fetch_and(&cond->value, ~NEED_TO_WAKE_ONE, Mods::memory_order_relaxed);

    if (!(value & NEED_TO_WAKE_ONE)) [[likely]]
        return 0;

    int rc = futex_wake(&cond->value, 1);
    VERIFY(rc >= 0);

    if (rc > 0)
        Mods::atomic_fetch_or(&cond->value, NEED_TO_WAKE_ONE, Mods::memory_order_relaxed);

    return 0;
}

/**
 * @param cond 
 * @return int 
 */
int pthread_cond_broadcast(pthread_cond_t* cond)
{
    u32 value = Mods::atomic_fetch_add(&cond->value, INCREMENT, Mods::memory_order_relaxed);
    
    if (!(value & NEED_TO_WAKE_ALL)) [[likely]]
        return 0;

    Mods::atomic_fetch_and(&cond->value, ~(NEED_TO_WAKE_ONE | NEED_TO_WAKE_ALL), Mods::memory_order_acquire);

    pthread_mutex_t* mutex = Mods::atomic_load(&cond->mutex, Mods::memory_order_relaxed);
    VERIFY(mutex);

    int rc = futex(&cond->value, FUTEX_REQUEUE, 1, nullptr, &mutex->lock, INT_MAX);
    VERIFY(rc >= 0);
    return 0;
}
