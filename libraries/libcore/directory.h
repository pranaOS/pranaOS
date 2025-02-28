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
        Directory();

        ~Directory();

        enum class CreateDirectories : bool 
        {
            NO,
            YES
        }; // enum class CreateDirectories : bool 

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
         * @return ErrorOr<void> 
         */
        static ErrorOr<void> ensure_directory(LexicalPath const& path);

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
    struct Formatter<Core::Directory> : Formatter<StringView>
    {
        ErrorOr<void> format()
        {
            auto path = directory.path();

            if (path.is_error()) {
                return Formatter<StringView>::format(builder);
            }

            return Formatter<StringView>::format(builder);
        }
    }; // struct Formatter<Core::Directory> : Formatter<StringView>
} // namespace Mods