/**
 * @file semaphore.cpp
 * @author Krisna Pranav
 * @brief semaphore
 * @version 6.0
 * @date 2024-10-26
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include <errno.h>
#include <mods/assertions.h>
#include <mods/atomic.h>
#include <mods/types.h>
#include <pranaos.h>
#include <semaphore.h>

static constexpr u32 POST_WAKES = 1 << 31;

/**
 * @param ...
 * @return sem_t*
 */
sem_t* sem_open(const char*, int, ...)
{
    errno = ENOSYS;
    return nullptr;
}

/**
 * @return int
 */
int sem_close(sem_t*)
{
    errno = ENOSYS;
    return -1;
}

/**
 * @return int
 */
int sem_unlink(const char*)
{
    errno = ENOSYS;
    return -1;
}

/**
 * @param sem
 * @param shared
 * @param value
 * @return int
 */
int sem_init(sem_t* sem, int shared, unsigned int value)
{
    if(shared)
    {
        errno = ENOSYS;
        return -1;
    }

    if(value > SEM_VALUE_MAX)
    {
        errno = EINVAL;
        return -1;
    }

    sem->value = value;
    return 0;
}

/**
 * @return int
 */
int sem_destroy(sem_t*)
{
    return 0;
}

/**
 * @param sem
 * @param sval
 * @return int
 */
int sem_getvalue(sem_t* sem, int* sval)
{
    u32 value = Mods::atomic_load(&sem->value, Mods::memory_order_relaxed);
    *sval = value & ~POST_WAKES;
    return 0;
}

/**
 * @param sem
 * @return int
 */
int sem_post(sem_t* sem)
{
    u32 value = Mods::atomic_fetch_add(&sem->value, 1u, Mods::memory_order_release);

    if(!(value & POST_WAKES)) [[likely]]
        return 0;

    value = Mods::atomic_fetch_and(&sem->value, ~POST_WAKES, Mods::memory_order_relaxed);

    if(!(value & POST_WAKES)) [[likely]]
        return 0;

    int rc = futex_wake(&sem->value, 1);
    VERIFY(rc >= 0);

    return 0;
}

/**
 * @param sem
 * @return int
 */
int sem_trywait(sem_t* sem)
{
    u32 value = Mods::atomic_load(&sem->value, Mods::memory_order_relaxed);
    u32 count = value & ~POST_WAKES;

    if(count == 0)
        return EAGAIN;

    u32 desired = (count - 1) | (value & POST_WAKES);
    bool exchanged = Mods::atomic_compare_exchange_strong(&sem->value, value, desired, Mods::memory_order_acquire);

    if(exchanged) [[likely]]
        return 0;
    else
        return EAGAIN;
}

/**
 * @param sem
 * @return int
 */
int sem_wait(sem_t* sem)
{
    return sem_timedwait(sem, nullptr);
}

/**
 * @param sem
 * @param abstime
 * @return int
 */
int sem_timedwait(sem_t* sem, const struct timespec* abstime)
{
    u32 value = Mods::atomic_load(&sem->value, Mods::memory_order_relaxed);
    bool responsible_for_waking = false;

    while(true)
    {
        u32 count = value & ~POST_WAKES;
        if(count > 0) [[likely]]
        {
            u32 whether_post_wakes = value & POST_WAKES;
            bool going_to_wake = false;
            if(responsible_for_waking && !whether_post_wakes)
            {
                if(count > 1) [[unlikely]]
                    going_to_wake = true;

                whether_post_wakes = POST_WAKES;
            }

            u32 desired = (count - 1) | whether_post_wakes;
            bool exchanged = Mods::atomic_compare_exchange_strong(&sem->value, value, desired, Mods::memory_order_acquire);

            if(!exchanged) [[unlikely]]
                continue;

            if(going_to_wake) [[unlikely]]
            {
                int rc = futex_wake(&sem->value, count - 1);
                VERIFY(rc >= 0);
            }
            return 0;
        }

        if(value == 0)
        {
            bool exchanged = Mods::atomic_compare_exchange_strong(&sem->value, value, POST_WAKES, Mods::memory_order_relaxed);
            if(!exchanged) [[unlikely]]
                continue;
            value = POST_WAKES;
        }

        responsible_for_waking = true;
        futex_wait(&sem->value, value, abstime, CLOCK_REALTIME);

        value = 1;
    }
}
