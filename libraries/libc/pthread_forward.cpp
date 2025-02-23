/**
 * @file pthread_forward.cpp
 * @author Krisna Pranav
 * @brief pthread forward
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/assertions.h>
#include <libc/bits/pthread_forward.h>

static PthreadFunctions s_pthread_functions;

/**
 * @param funcs 
 */
void __init_pthread_forward(PthreadFunctions funcs)
{
    s_pthread_functions = funcs;
}

/**
 * @param mutex 
 * @return int 
 */
int pthread_mutex_trylock(pthread_mutex_t* mutex)
{
    VERIFY(s_pthread_functions.pthread_mutex_trylock);
    return s_pthread_functions.pthread_mutex_trylock(mutex);
}

/**
 * @param mutex 
 * @return int 
 */
int pthread_mutex_destroy(pthread_mutex_t* mutex)
{
    VERIFY(s_pthread_functions.pthread_mutex_destroy);
    return s_pthread_functions.pthread_mutex_destroy(mutex);
}

/**
 * @param attr 
 * @return int 
 */
int pthread_mutexattr_init(pthread_mutexattr_t* attr)
{
    VERIFY(s_pthread_functions.pthread_mutexattr_init);
    return s_pthread_functions.pthread_mutexattr_init(attr);
}

/**
 * @param attr 
 * @param type 
 * @return int 
 */
int pthread_mutexattr_settype(pthread_mutexattr_t* attr, int type)
{
    VERIFY(s_pthread_functions.pthread_mutexattr_settype);
    return s_pthread_functions.pthread_mutexattr_settype(attr, type);
}

/**
 * @param attr 
 * @return int 
 */
int pthread_mutexattr_destroy(pthread_mutexattr_t* attr)
{
    VERIFY(s_pthread_functions.pthread_mutexattr_destroy);
    return s_pthread_functions.pthread_mutexattr_destroy(attr);
}

/**
 * @param self 
 * @param callback 
 * @return int 
 */
int pthread_once(pthread_once_t* self, void (*callback)(void))
{
    VERIFY(s_pthread_functions.pthread_once);
    return s_pthread_functions.pthread_once(self, callback);
}

/**
 * @param cond 
 * @return int 
 */
int pthread_cond_broadcast(pthread_cond_t* cond)
{
    VERIFY(s_pthread_functions.pthread_cond_broadcast);
    return s_pthread_functions.pthread_cond_broadcast(cond);
}

/**
 * @param cond 
 * @param attr 
 * @return int 
 */
int pthread_cond_init(pthread_cond_t* cond, pthread_condattr_t const* attr)
{
    VERIFY(s_pthread_functions.pthread_cond_init);
    return s_pthread_functions.pthread_cond_init(cond, attr);
}

/**
 * @param cond 
 * @return int 
 */
int pthread_cond_signal(pthread_cond_t* cond)
{
    VERIFY(s_pthread_functions.pthread_cond_signal);
    return s_pthread_functions.pthread_cond_signal(cond);
}

/**
 * @param cond 
 * @param mutex 
 * @return int 
 */
int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex)
{
    VERIFY(s_pthread_functions.pthread_cond_wait);
    return s_pthread_functions.pthread_cond_wait(cond, mutex);
}

/**
 * @param cond 
 * @return int 
 */
int pthread_cond_destroy(pthread_cond_t* cond)
{
    VERIFY(s_pthread_functions.pthread_cond_destroy);
    return s_pthread_functions.pthread_cond_destroy(cond);
}

/**
 * @param cond 
 * @param mutex 
 * @param abstime 
 * @return int 
 */
int pthread_cond_timedwait(pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* abstime)
{
    VERIFY(s_pthread_functions.pthread_cond_timedwait);
    return s_pthread_functions.pthread_cond_timedwait(cond, mutex, abstime);
}
