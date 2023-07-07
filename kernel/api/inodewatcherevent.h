/**
 * @file inodewatcherevent.h
 * @author Krisna Pranav
 * @brief inode watcher event
 * @version 1.0
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

struct [[gnu::packed]] InodeWatcherEvent {
    enum class Type {
        Invalid = 0,
        Modified,
        ChildAdded,
        ChildRemoved,
    };

    Type type { Type::Invalid };
    unsigned inode_index { 0 };
};
