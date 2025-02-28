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

    MOD_ENUM_BITWISE_OPERATORS(FileWatcherEvent::Type);

    class FileWatcherBase 
    {
    public:
        virtual ~FileWatcherBase() = default;

        /**
         * @param path 
         * @return true 
         * @return false 
         */
        bool is_watching(String const& path) const 
        { 
            return m_path_to_wd.find(path) != m_path_to_wd.end(); 
        }
        
    protected:  
        /**
         * @brief Construct a new File Watcher Base object
         * 
         * @param watcher_fd 
         */
        FileWatcherBase(int watcher_fd)
            : m_watcher_fd(watcher_fd)
        {}
        
        int m_watcher_fd { -1 };
        HashMap<String, unsigned> m_path_to_wd;
        HashMap<unsigned, String> m_wd_to_path;
    }; // class FileWatcherBase 
} // namespace Core
