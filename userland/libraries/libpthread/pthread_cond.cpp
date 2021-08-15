/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Assertions.h>
#include <base/Atomic.h>
#include <base/Types.h>
#include <errno.h>
#include <pthread.h>
#include <pranaos.h>
#include <sys/types.h>
#include <time.h>

int pthread_condattr_init(pthread_condattr_t* attr)
{
    attr->clockid = CLOCK_MONOTONIC_COARSE;
    return 0;
}

int pthread_condattr_destroy(pthread_condattr_t*)
{
    return 0;
}

int pthread_condattr_setclock(pthread_condattr_t* attr, clockid_t clock)
{
    attr->clockid = clock;
    return 0;
}

static constexpr u32 NEED_TO_WAKE_ONE = 1;
static constexpr u32 NEED_TO_WAKE_ALL = 2;
static constexpr u32 INCREMENT = 4;

int pthread_cond_init(pthread_cond_t* cond, const pthread_condattr_t* attr)
{
    cond->mutex = nullptr;
    cond->value = 0;
    cond->clockid = attr ? attr->clockid : CLOCK_MONOTONIC_COARSE;
    return 0;
}

int pthread_cond_destroy(pthread_cond_t*)
{
    return 0;
}

int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex)
{
    return pthread_cond_timedwait(cond, mutex, nullptr);
}

int pthread_cond_timedwait(pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* abstime)
{
    pthread_mutex_t* old_mutex = Base::atomic_exchange(&cond->mutex, mutex, Base::memory_order_relaxed);
    if (old_mutex && old_mutex != mutex)
        TODO();

    u32 value = Base::atomic_fetch_or(&cond->value, NEED_TO_WAKE_ONE | NEED_TO_WAKE_ALL, Base::memory_order_release) | NEED_TO_WAKE_ONE | NEED_TO_WAKE_ALL;
    pthread_mutex_unlock(mutex);
    int rc = futex_wait(&cond->value, value, abstime, cond->clockid);
    if (rc < 0 && errno != EAGAIN)
        return errno;

    __pthread_mutex_lock_pessimistic_np(mutex);
    return 0;
}

int pthread_cond_signal(pthread_cond_t* cond)
{
    u32 value = Base::atomic_fetch_add(&cond->value, INCREMENT, Base::memory_order_relaxed);

    if (!(value & NEED_TO_WAKE_ONE)) [[likely]]
        return 0;

    value = Base::atomic_fetch_and(&cond->value, ~NEED_TO_WAKE_ONE, Base::memory_order_relaxed);
    
    if (!(value & NEED_TO_WAKE_ONE)) [[likely]]
        return 0;

    int rc = futex_wake(&cond->value, 1);
    VERIFY(rc >= 0);
    
    if (rc > 0)
        Base::atomic_fetch_or(&cond->value, NEED_TO_WAKE_ONE, Base::memory_order_relaxed);

    return 0;
}

int pthread_cond_broadcast(pthread_cond_t* cond)
{

    u32 value = Base::atomic_fetch_add(&cond->value, INCREMENT, Base::memory_order_relaxed);

    if (!(value & NEED_TO_WAKE_ALL)) [[likely]]
        return 0;

    Base::atomic_fetch_and(&cond->value, ~(NEED_TO_WAKE_ONE | NEED_TO_WAKE_ALL), Base::memory_order_acquire);

    pthread_mutex_t* mutex = Base::atomic_load(&cond->mutex, Base::memory_order_relaxed);
    VERIFY(mutex);

    int rc = futex(&cond->value, FUTEX_REQUEUE | FUTEX_PRIVATE_FLAG, 1, nullptr, &mutex->lock, INT_MAX);
    VERIFY(rc >= 0);
    return 0;
}
