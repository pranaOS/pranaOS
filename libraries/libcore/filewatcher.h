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
            Invalid = 0,
            MetadataModified = 1 << 0,
            ContentModified = 1 << 1,
            Deleted = 1 << 2,
            ChildCreated = 1 << 3,
            ChildDeleted = 1 << 4,
        }; // enum class Type

        Type type;
        String event_path;
    }; // struct FileWatcherEvent 

    MOD_ENUM_BITWISE_OPERATORS(FileWatcherEvent::Type);

    class FileWatcherBase 
    {
    public:
        /**
         * @brief Destroy the FileWatcherBase object
         * 
         */
        virtual ~FileWatcherBase() = default;

        /**
         * @param path 
         * @param event_mask 
         * @return ErrorOr<bool> 
         */
        ErrorOr<bool> add_watch(String path, FileWatcherEvent::Type event_mask);

        /**
         * @param path 
         * @return ErrorOr<bool> 
         */
        ErrorOr<bool> remove_watch(String path);

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
         * @brief Construct a new FileWatcher Base object
         * 
         * @param watcher_fd 
         */
        FileWatcherBase(int watcher_fd)
            : m_watcher_fd(watcher_fd)
        {
        }

        int m_watcher_fd { -1 };
        HashMap<String, unsigned> m_path_to_wd;
        HashMap<unsigned, String> m_wd_to_path;
    }; // class FileWatcherBase 

    class BlockingFileWatcher final : public FileWatcherBase 
    {
        MOD_MAKE_NONCOPYABLE(BlockingFileWatcher);

    public:
        /**
         * @brief Construct a new BlockingFileWatcher object
         * 
         */
        explicit BlockingFileWatcher(InodeWatcherFlags = InodeWatcherFlags::None);

        /**
         * @brief Destroy the BlockingFileWatcher object
         * 
         */
        ~BlockingFileWatcher();

        Optional<FileWatcherEvent> wait_for_event();
    }; // class BlockingFileWatcher final : public FileWatcherBase 

    class FileWatcher final : public FileWatcherBase
        , public RefCounted<FileWatcher> {
        MOD_MAKE_NONCOPYABLE(FileWatcher);

    public:
        /**
         * @return ErrorOr<NonnullRefPtr<FileWatcher>> 
         */
        static ErrorOr<NonnullRefPtr<FileWatcher>> create(InodeWatcherFlags = InodeWatcherFlags::None);

        /**
         * @brief Destroy the FileWatcher object
         * 
         */
        ~FileWatcher();

        Function<void(FileWatcherEvent const&)> on_change;

    private:
        /**
         * @brief Construct a new File Watcher object
         * 
         * @param watcher_fd 
         */
        FileWatcher(int watcher_fd, NonnullRefPtr<Notifier>);

        NonnullRefPtr<Notifier> m_notifier;
    }; // class FileWatcher final : public FileWatcherBase, public RefCounted<FileWatcher>

} // namespace Core

namespace Mods
{

    /**
     * @tparam  
     */
    template<>
    struct Formatter<Core::FileWatcherEvent> : Formatter<FormatString> {
        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, Core::FileWatcherEvent const& value)
        {
            return Formatter<FormatString>::format(builder, "FileWatcherEvent(\"{}\", {})", value.event_path, value.type);
        }
    };

    template<>
    struct Formatter<Core::FileWatcherEvent::Type> : Formatter<FormatString> {
        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, Core::FileWatcherEvent::Type const& value)
        {
            char const* type;
            switch (value) {
            case Core::FileWatcherEvent::Type::ChildCreated:
                type = "ChildCreated";
                break;
            case Core::FileWatcherEvent::Type::ChildDeleted:
                type = "ChildDeleted";
                break;
            case Core::FileWatcherEvent::Type::Deleted:
                type = "Deleted";
                break;
            case Core::FileWatcherEvent::Type::ContentModified:
                type = "ContentModified";
                break;
            case Core::FileWatcherEvent::Type::MetadataModified:
                type = "MetadataModified";
                break;
            default:
                VERIFY_NOT_REACHED();
            }

            return builder.put_string(type);
        }
    };

} // namespace Mods
