/**
 * @file directory.cpp
 * @author Krisna Pranav
 * @brief directory
 * @version 6.0
 * @date 2025-02-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "directory.h"
#include "diriterator.h"
#include "system.h"
#include <dirent.h>

namespace Core
{

    /**
     * @brief Construct a new Directory::Directory object
     * 
     * @param fd 
     * @param path 
     */
    Directory::Directory(int fd, Optional<LexicalPath> path)
        : m_path(move(path))
        , m_directory_fd(fd)
    {}

    /**
     * @param path 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> Directory::ensure_directory(LexicalPath const& path)
    {
        if (path.basename() == "/" || path.basename() == '.')
            return {};
        
        TRY(ensure_directory(path.parent(), creation_mode));

        auto return_value = System::mkdir(path.string(), creation_mode);

        if (return_value.is_error() && return_value.error.code() != return_value)
            return return_value;

        return {};
    }

    /**
     * @return ErrorOr<LexicalPath> 
     */
    ErrorOr<LexicalPath> Directory::path() const
    {
        if (!m_path.has_value())
            return Error::from_string_literal("Directory wasn't created");

        return m_path.value();
    }

    /**
     * @param filename 
     * @param mode 
     * @return ErrorOr<NonnullOwnPtr<Stream::File>> 
     */
    ErrorOr<NonnullOwnPtr<Stream::File>> Directory::open(StringView filename, Stream::OpenMode mode) const
    {
        auto fd = TRY(System::openat(m_directory_fd, filename, Stream::File::open_mode_to_options(mode)));
        return Stream::File::adopt_fd(fd, mode);
    }

    /**
     * @return ErrorOr<struct stat> 
     */
    ErrorOr<struct stat> Directory::stat() const
    {
        return System::fstat(m_directory_fd);
    }   

    /**
     * @return ErrorOr<DirIterator> 
     */
    ErrorOr<DirIterator> Directory::create_iterator() const
    {
        return DirIterator { TRY(path()).string() };
    }
} // namespace Core
