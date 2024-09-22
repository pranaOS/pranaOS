/**
 * @file lockrank.h
 * @author Krisna Pranav
 * @brief Lock Rank
 * @version 6.0
 * @date 2024-09-22
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/enumbits.h>

namespace Kernel 
{
    enum class LockRank : int 
    {
        None = 0x000,
        MemoryManager = 0x001,
        Interrupts = 0x002,
        FileSystem = 0x004,
        Thread = 0x008,
        Process = 0x010,
    }; // enum class LockRank : int

    MOD_ENUM_BITWISE_OPERATORS(LockRank);

    void track_lock_acquire(LockRank);
    void track_lock_release(LockRank);
} // namespace Kernel
