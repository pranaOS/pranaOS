/**
 * @file directory.h
 * @author Krisna Pranav
 * @brief directory
 * @version 6.0
 * @date 2025-02-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/error.h>
#include <mods/format.h>
#include <mods/lexicalpath.h>
#include <mods/noncopyable.h>
#include <mods/optional.h>
#include <libcore/stream.h>
#include <dirent.h>
#include <sys/stat.h>

namespace Core 
{

    class DirIterator;

    class Directory 
    {
        MOD_MAKE_NONCOPYABLE(Directory);

    public:
        /**
         * @brief Construct a new Directory object
         * 
         */
        Directory(Directory&&);

        /**
         * @brief Destroy the Directory object
         * 
         */
        ~Directory();

        enum class CreateDirectories : bool 
        {
            No,
            Yes,
        }; // enum class CreateDirectories : bool 

        /**
         * @param path 
         * @param creation_mode 
         * @return ErrorOr<Directory> 
         */
        static ErrorOr<Directory> create(LexicalPath path, CreateDirectories, mode_t creation_mode = 0755);

        /**
         * @param path 
         * @param creation_mode 
         * @return ErrorOr<Directory> 
         */
        static ErrorOr<Directory> create(String path, CreateDirectories, mode_t creation_mode = 0755);

        /**
         * @param fd 
         * @param path 
         * @return ErrorOr<Directory> 
         */
        static ErrorOr<Directory> adopt_fd(int fd, Optional<LexicalPath> path = {});

        /**
         * @param filename 
         * @param mode 
         * @return ErrorOr<NonnullOwnPtr<Stream::File>> 
         */
        ErrorOr<NonnullOwnPtr<Stream::File>> open(StringView filename, Stream::OpenMode mode) const;

        /**
         * @return ErrorOr<struct stat> 
         */
        ErrorOr<struct stat> stat() const;

        /**
         * @brief Create a iterator object
         * 
         * @return ErrorOr<DirIterator> 
         */
        ErrorOr<DirIterator> create_iterator() const;

        /**
         * @return ErrorOr<LexicalPath> 
         */
        ErrorOr<LexicalPath> path() const;

        /**
         * @param fd 
         * @return ErrorOr<bool> 
         */
        static ErrorOr<bool> is_valid_directory(int fd);

    private:
        /**
         * @brief Construct a new Directory object
         * 
         * @param directory_fd 
         * @param path 
         */
        Directory(int directory_fd, Optional<LexicalPath> path);

        /**
         * @param path 
         * @param creation_mode 
         * @return ErrorOr<void> 
         */
        static ErrorOr<void> ensure_directory(LexicalPath const& path, mode_t creation_mode = 0755);

        Optional<LexicalPath> m_path;

        int m_directory_fd;
    }; // class Directory

} // namespace Core

namespace Mods
{
    /**
     * @tparam  
     */
    template<>
    struct Formatter<Core::Directory> : Formatter<StringView> {
        /**
         * @param builder 
         * @param directory 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, Core::Directory const& directory)
        {
            auto path = directory.path();
            if (path.is_error())
                return Formatter<StringView>::format(builder, "<unknown>");
            return Formatter<StringView>::format(builder, path.release_value().string());
        }
    };

} // namespace Mods
