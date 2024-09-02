/**
 * @file inodewatcherflags.h
 * @author Krisna Pranav
 * @brief Inode Watcher Flags
 * @version 6.0
 * @date 2024-09-02
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/enumbits.h>
#include <mods/types.h>

enum class InodeWatcherFlags : u32 
{
    None = 0,
    Nonblock = 1 << 0,
    CloseOnExec = 1 << 1,
}; // enum class InodaWatcherFlags

MOD_ENUM_BITWISE_OPERATORS(InodeWatcherFlags);