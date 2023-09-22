/**
 * @file directorywatcher.h
 * @author Krisna Pranav
 * @brief directory watcher
 * @version 6.0
 * @date 2023-09-22
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/api/inodewatcherevent.h>
#include <mods/function.h>
#include <mods/noncopyable.h>
#include <mods/string.h>

namespace Core
{
    class DirectoryWatcher
    {
    public:
        /**
         * @brief Construct a new Directory Watcher object
         * 
         * @param path 
         */
        explicit DirectoryWatcher(const String& path);

        /// @brief Destroy the Directory Watcher object
        ~DirectoryWatcher();

        struct Event
        {
            enum class Type
            {
                ChildAdded,
                ChildRemoved,
            };
        }; // struct Event

    private:
        String get_child_with_inode_index(unsigned) const;

        String m_path;

        int m_watcher_fd { -1 };
    }; // class DirectoryWatcher
}