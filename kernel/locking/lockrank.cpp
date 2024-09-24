/**
 * @file lockrank.cpp
 * @author Krisna Pranav
 * @brief Lock Rank
 * @version 6.0
 * @date 2024-09-24
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/locking/lockrank.h>
#include <kernel/thread.h>

namespace Kernel 
{

    /**
     * @param rank 
     */
    void track_lock_acquire(LockRank rank)
    {
        if constexpr (LOCK_RANK_ENFORCEMENT) {
            auto* thread = Thread::current();
            if (thread && !thread->is_crashing())
                thread->track_lock_acquire(rank);
        }
    }

    /**
     * @param rank 
     */
    void track_lock_release(LockRank rank)
    {
        if constexpr (LOCK_RANK_ENFORCEMENT) {
            auto* thread = Thread::current();
            if (thread && !thread->is_crashing())
                thread->track_lock_release(rank);
        }
    }
} // namespace Kernel