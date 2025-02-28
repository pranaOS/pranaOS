/**
 * @file filewatcher.h
 * @author Krisna Pranav
 * @brief file watcher
 * @version 6.0
 * @date 2025-02-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/enumbits.h>
#include <mods/function.h>
#include <mods/noncopyable.h>
#include <mods/nonnullrefptr.h>
#include <mods/refcounted.h>
#include <mods/string.h>
#include <kernel/api/inodewatcherevent.h>
#include <kernel/api/inodewatcherflags.h>
#include <libcore/notifier.h>

namespace Core
{
    struct FileWatcherEvent 
    {
        enum class Type
        {
            
        }; // enum class Type

        Type type;
        String event_path;
    }; // struct FileWatcherEvent 
} // namespace Core
