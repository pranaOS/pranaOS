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
    {
    }

    /**
     * @brief Construct a new Directory::Directory object
     * 
     * @param other 
     */
    Directory::Directory(Directory&& other)
        : m_path(move(other.m_path))
        , m_directory_fd(other.m_directory_fd)
    {
        other.m_directory_fd = -1;
    }

    /**
     * @brief Destroy the Directory::Directory object
     * 
     */
    Directory::~Directory()
    {
        if (m_directory_fd != -1)
            MUST(System::close(m_directory_fd));
    }

    /**
     * @param fd 
     * @return ErrorOr<bool> 
     */
    ErrorOr<bool> Directory::is_valid_directory(int fd)
    {
        auto stat = TRY(System::fstat(fd));
        return stat.st_mode & S_IFDIR;
    }

    /**
     * @param fd 
     * @param path 
     * @return ErrorOr<Directory> 
     */
    ErrorOr<Directory> Directory::adopt_fd(int fd, Optional<LexicalPath> path)
    {
        if (!TRY(Directory::is_valid_directory(fd)))
            return Error::from_errno(ENOTDIR);

        return Directory { fd, move(path) };
    }

    /**
     * @param path 
     * @param create_directories 
     * @param creation_mode 
     * @return ErrorOr<Directory> 
     */
    ErrorOr<Directory> Directory::create(String path, CreateDirectories create_directories, mode_t creation_mode)
    {
        return create(LexicalPath { move(path) }, create_directories, creation_mode);
    }

    /**
     * @param path 
     * @param create_directories 
     * @param creation_mode 
     * @return ErrorOr<Directory> 
     */
    ErrorOr<Directory> Directory::create(LexicalPath path, CreateDirectories create_directories, mode_t creation_mode)
    {
        if (create_directories == CreateDirectories::Yes)
            TRY(ensure_directory(path, creation_mode));

        auto fd = TRY(System::open(path.string(), O_CLOEXEC));
        return adopt_fd(fd, move(path));
    }

    /**
     * @param path 
     * @param creation_mode 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> Directory::ensure_directory(LexicalPath const& path, mode_t creation_mode)
    {
        if (path.basename() == "/" || path.basename() == ".")
            return {};

        TRY(ensure_directory(path.parent(), creation_mode));

        auto return_value = System::mkdir(path.string(), creation_mode);
        
        if (return_value.is_error() && return_value.error().code() != EEXIST)
            return return_value;

        return {};
    }

    /**
     * @return ErrorOr<LexicalPath> 
     */
    ErrorOr<LexicalPath> Directory::path() const
    {
        if (!m_path.has_value())
            return Error::from_string_literal("Directory wasn't created with a path");
            
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
