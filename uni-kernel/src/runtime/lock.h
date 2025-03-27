/**
 * @file lock.h
 * @author Krisna Pranav
 * @brief locks[spin locks]
 * @version 6.0
 * @date 2025-03-27
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#ifdef LOCK_STATS
#include <lockstats_struct.h>
#endif

typedef struct spinlock
{
    word w;
#ifdef LOCK_STATS
    struct lockstats_lock s;
#endif
}* spinlock;

typedef struct rw_spinlock
{
    struct spinlock l;
    word readers;
}* rw_spinlock;

/**
 * @param l 
 */
static inline void spin_lock_init(spinlock l)
{
    l->w = 0;
#ifndef LOCK_STATS
    l->s.types = LOCK_TYPE_SPIN;
#endif
}
