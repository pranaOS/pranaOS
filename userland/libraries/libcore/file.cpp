/**
 * @file file.cpp
 * @author Krisna Pranav
 * @brief file
 * @version 6.0
 * @date 2025-02-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#ifdef __pranaos__
#    include <pranaos.h>
#endif
#include <mods/lexicalpath.h>
#include <mods/scopeguard.h>
#include <libcore/diriterator.h>
#include <libcore/file.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>

#if defined(__linux__) && defined(basename)
#    undef basename
#endif

namespace Core 
{

    /**
     * @param filename 
     * @param mode 
     * @param permissions 
     * @return ErrorOr<NonnullRefPtr<File>> 
     */
    ErrorOr<NonnullRefPtr<File>> File::open(String filename, OpenMode mode, mode_t permissions)
    {
        auto file = File::construct(move(filename));
        if (!file->open_impl(mode, permissions))
            return Error::from_errno(file->error());
        return file;
    }

    /**
     * @brief Construct a new File::File object
     * 
     * @param filename 
     * @param parent 
     */
    File::File(String filename, Object* parent)
        : IODevice(parent)
        , m_filename(move(filename))
    {
    }

    /**
     * @brief Destroy the File::File object
     * 
     */
    File::~File()
    {
        if (m_should_close_file_descriptor == ShouldCloseFileDescriptor::Yes && mode() != OpenMode::NotOpen)
            close();
    }   

    /**
     * @param fd 
     * @param mode 
     * @param should_close 
     * @return true 
     * @return false 
     */
    bool File::open(int fd, OpenMode mode, ShouldCloseFileDescriptor should_close)
    {
        set_fd(fd);
        set_mode(mode);
        m_should_close_file_descriptor = should_close;

        return true;
    }

    /**
     * @param mode 
     * @return true 
     * @return false 
     */
    bool File::open(OpenMode mode)
    {
        return open_impl(mode, 0666);
    }

    /**
     * @param mode 
     * @param permissions 
     * @return true 
     * @return false 
     */
    bool File::open_impl(OpenMode mode, mode_t permissions)
    {
        VERIFY(!m_filename.is_null());
        int flags = 0;

        if (has_flag(mode, OpenMode::ReadOnly) && has_flag(mode, OpenMode::WriteOnly)) {
            flags |= O_RDWR | O_CREAT;
        } else if (has_flag(mode, OpenMode::ReadOnly)) {
            flags |= O_RDONLY;
        } else if (has_flag(mode, OpenMode::WriteOnly)) {
            flags |= O_WRONLY | O_CREAT;
            bool should_truncate = !(has_flag(mode, OpenMode::Append) || has_flag(mode, OpenMode::MustBeNew));
            if (should_truncate)
                flags |= O_TRUNC;
        }

        if (has_flag(mode, OpenMode::Append))
            flags |= O_APPEND;
        if (has_flag(mode, OpenMode::Truncate))
            flags |= O_TRUNC;
        if (has_flag(mode, OpenMode::MustBeNew))
            flags |= O_EXCL;
        if (!has_flag(mode, OpenMode::KeepOnExec))
            flags |= O_CLOEXEC;

        int fd = ::open(m_filename.characters(), flags, permissions);

        if (fd < 0) {
            set_error(errno);
            return false;
        }

        set_fd(fd);
        set_mode(mode);
        return true;
    }

    /**
     * @return int 
     */
    int File::leak_fd()
    {
        m_should_close_file_descriptor = ShouldCloseFileDescriptor::No;
        return fd();
    }   

    /**
     * @return true 
     * @return false 
     */
    bool File::is_device() const
    {
        struct stat stat;

        if (fstat(fd(), &stat) < 0)
            return false;

        return S_ISBLK(stat.st_mode) || S_ISCHR(stat.st_mode);
    }

    /**
     * @param filename 
     * @return true 
     * @return false 
     */
    bool File::is_device(String const& filename)
    {
        struct stat st;

        if (stat(filename.characters(), &st) < 0)
            return false;

        return S_ISBLK(st.st_mode) || S_ISCHR(st.st_mode);
    }

    /**
     * @return true 
     * @return false 
     */
    bool File::is_directory() const
    {
        struct stat stat;

        if (fstat(fd(), &stat) < 0)
            return false;

        return S_ISDIR(stat.st_mode);
    }

    /**
     * @param filename 
     * @return true 
     * @return false 
     */
    bool File::is_directory(String const& filename)
    {
        struct stat st;

        if (stat(filename.characters(), &st) < 0)
            return false;

        return S_ISDIR(st.st_mode);
    }

    /**
     * @return true 
     * @return false 
     */
    bool File::is_link() const
    {
        struct stat stat;

        if (fstat(fd(), &stat) < 0)
            return false;

        return S_ISLNK(stat.st_mode);
    }

    /**
     * @param filename 
     * @return true 
     * @return false 
     */
    bool File::is_link(String const& filename)
    {
        struct stat st;

        if (lstat(filename.characters(), &st) < 0)
            return false;

        return S_ISLNK(st.st_mode);
    }   

    /**
     * @return true 
     * @return false 
     */
    bool File::looks_like_shared_library() const
    {
        return File::looks_like_shared_library(m_filename);
    }

    /**
     * @param filename 
     * @return true 
     * @return false 
     */
    bool File::looks_like_shared_library(String const& filename)
    {
        return filename.ends_with(".so"sv) || filename.contains(".so."sv);
    }

    /**
     * @param filename 
     * @return true 
     * @return false 
     */
    bool File::exists(String const& filename)
    {
        struct stat st;
        return stat(filename.characters(), &st) == 0;
    }

    /**
     * @param filename 
     * @return ErrorOr<size_t> 
     */
    ErrorOr<size_t> File::size(String const& filename)
    {
        struct stat st;
        if (stat(filename.characters(), &st) < 0)
            return Error::from_errno(errno);
        return st.st_size;
    }

    /**
     * @param filename 
     * @return String 
     */
    String File::real_path_for(String const& filename)
    {
        if (filename.is_null())
            return {};

        auto* path = realpath(filename.characters(), nullptr);
        String real_path(path);
        free(path);

        return real_path;
    }

    /**
     * @return String 
     */
    String File::current_working_directory()
    {
        char* cwd = getcwd(nullptr, 0);

        if (!cwd) {
            perror("getcwd");
            return {};
        }

        auto cwd_as_string = String(cwd);
        free(cwd);

        return cwd_as_string;
    }

    /**
     * @param path 
     * @return String 
     */
    String File::absolute_path(String const& path)
    {
        if (File::exists(path))
            return File::real_path_for(path);

        if (path.starts_with("/"sv))
            return LexicalPath::canonicalized_path(path);

        auto working_directory = File::current_working_directory();
        auto full_path = LexicalPath::join(working_directory, path);

        return LexicalPath::canonicalized_path(full_path.string());
    }

    #ifdef __pranaos__

    /**
     * @param link_path 
     * @return ErrorOr<String> 
     */
    ErrorOr<String> File::read_link(String const& link_path)
    {
        char small_buffer[64];

        int rc = pranaos_readlink(link_path.characters(), link_path.length(), small_buffer, sizeof(small_buffer));
        if (rc < 0)
            return Error::from_errno(errno);

        size_t size = rc;

        if (size <= sizeof(small_buffer))
            return String { small_buffer, size };
        
        char* large_buffer_ptr;
        auto large_buffer = StringImpl::create_uninitialized(size, large_buffer_ptr);

        rc = pranaos_readlink(link_path.characters(), link_path.length(), large_buffer_ptr, size);

        if (rc < 0)
            return Error::from_errno(errno);

        size_t new_size = rc;
        if (new_size == size)
            return { *large_buffer };

        if (new_size < size)
            return String { large_buffer_ptr, new_size };

        errno = EAGAIN;
        return Error::from_errno(errno);
    }

    #else

    /**
     * @param link_path 
     * @return ErrorOr<String> 
     */
    ErrorOr<String> File::read_link(String const& link_path)
    {
        struct stat statbuf = {};
        int rc = lstat(link_path.characters(), &statbuf);

        if (rc < 0)
            return Error::from_errno(errno);

        char* buffer_ptr;
        auto buffer = StringImpl::create_uninitialized(statbuf.st_size, buffer_ptr);

        if (readlink(link_path.characters(), buffer_ptr, statbuf.st_size) < 0)
            return Error::from_errno(errno);

        if (rc == statbuf.st_size)
            return { *buffer };

        return String { buffer_ptr, (size_t)rc };
    }

    #endif

    static RefPtr<File> stdin_file;
    static RefPtr<File> stdout_file;
    static RefPtr<File> stderr_file;

    /**
     * @return NonnullRefPtr<File> 
     */
    NonnullRefPtr<File> File::standard_input()
    {
        if (!stdin_file) {
            stdin_file = File::construct();
            stdin_file->open(STDIN_FILENO, OpenMode::ReadOnly, ShouldCloseFileDescriptor::No);
        }
        return *stdin_file;
    }

    /**
     * @return NonnullRefPtr<File> 
     */
    NonnullRefPtr<File> File::standard_output()
    {
        if (!stdout_file) {
            stdout_file = File::construct();
            stdout_file->open(STDOUT_FILENO, OpenMode::WriteOnly, ShouldCloseFileDescriptor::No);
        }

        return *stdout_file;
    }

    /**
     * @return NonnullRefPtr<File> 
     */
    NonnullRefPtr<File> File::standard_error()
    {
        if (!stderr_file) {
            stderr_file = File::construct();
            stderr_file->open(STDERR_FILENO, OpenMode::WriteOnly, ShouldCloseFileDescriptor::No);
        }

        return *stderr_file;
    }

    /**
     * @brief Get the duplicate name object
     * 
     * @param path 
     * @param duplicate_count 
     * @return String 
     */
    static String get_duplicate_name(String const& path, int duplicate_count)
    {
        if (duplicate_count == 0) {
            return path;
        }

        LexicalPath lexical_path(path);
        StringBuilder duplicated_name;
        duplicated_name.append('/');

        auto& parts = lexical_path.parts_view();

        for (size_t i = 0; i < parts.size() - 1; ++i) {
            duplicated_name.appendff("{}/", parts[i]);
        }

        auto prev_duplicate_tag = String::formatted("({})", duplicate_count);
        auto title = lexical_path.title();

        if (title.ends_with(prev_duplicate_tag)) {
            title = title.substring_view(0, title.length() - prev_duplicate_tag.length());
        }

        duplicated_name.appendff("{} ({})", title, duplicate_count);

        if (!lexical_path.extension().is_empty()) {
            duplicated_name.appendff(".{}", lexical_path.extension());
        }

        return duplicated_name.build();
    }

    /**
     * @brief
     * @param dst_path 
     * @param src_path 
     * @param recursion_mode 
     * @param link_mode 
     * @param add_duplicate_file_marker 
     * @param preserve_mode 
     * @return ErrorOr<void, File::CopyError> 
     */
    ErrorOr<void, File::CopyError> File::copy_file_or_directory(String const& dst_path, String const& src_path, RecursionMode recursion_mode, LinkMode link_mode, AddDuplicateFileMarker add_duplicate_file_marker, PreserveMode preserve_mode)
    {
        if (add_duplicate_file_marker == AddDuplicateFileMarker::Yes) {
            int duplicate_count = 0;
            while (access(get_duplicate_name(dst_path, duplicate_count).characters(), F_OK) == 0) {
                ++duplicate_count;
            }
            if (duplicate_count != 0) {
                return copy_file_or_directory(get_duplicate_name(dst_path, duplicate_count), src_path, RecursionMode::Allowed, LinkMode::Disallowed, AddDuplicateFileMarker::Yes, preserve_mode);
            }
        }

        auto source_or_error = File::open(src_path, OpenMode::ReadOnly);

        if (source_or_error.is_error())
            return CopyError { errno, false };

        auto& source = *source_or_error.value();

        struct stat src_stat;

        if (fstat(source.fd(), &src_stat) < 0)
            return CopyError { errno, false };

        if (source.is_directory()) {
            if (recursion_mode == RecursionMode::Disallowed)
                return CopyError { errno, true };
            return copy_directory(dst_path, src_path, src_stat);
        }

        if (link_mode == LinkMode::Allowed) {
            if (link(src_path.characters(), dst_path.characters()) < 0)
                return CopyError { errno, false };

            return {};
        }

        return copy_file(dst_path, src_stat, source, preserve_mode);
    }

    /**
     * @param dst_path 
     * @param src_stat 
     * @param source 
     * @param preserve_mode 
     * @return ErrorOr<void, File::CopyError> 
     */
    ErrorOr<void, File::CopyError> File::copy_file(String const& dst_path, struct stat const& src_stat, File& source, PreserveMode preserve_mode)
    {
        int dst_fd = creat(dst_path.characters(), 0666);

        if (dst_fd < 0) {
            if (errno != EISDIR)
                return CopyError { errno, false };

            auto dst_dir_path = String::formatted("{}/{}", dst_path, LexicalPath::basename(source.filename()));
            dst_fd = creat(dst_dir_path.characters(), 0666);
            if (dst_fd < 0)
                return CopyError { errno, false };
        }

        ScopeGuard close_fd_guard([dst_fd]() { ::close(dst_fd); });

        if (src_stat.st_size > 0) {
            if (ftruncate(dst_fd, src_stat.st_size) < 0)
                return CopyError { errno, false };
        }

        for (;;) {
            char buffer[32768];
            ssize_t nread = ::read(source.fd(), buffer, sizeof(buffer));

            if (nread < 0) {
                return CopyError { errno, false };
            }

            if (nread == 0)
                break;

            ssize_t remaining_to_write = nread;
            char* bufptr = buffer;

            while (remaining_to_write) {
                ssize_t nwritten = ::write(dst_fd, bufptr, remaining_to_write);
                if (nwritten < 0)
                    return CopyError { errno, false };

                VERIFY(nwritten > 0);
                remaining_to_write -= nwritten;
                bufptr += nwritten;
            }
        }

        auto my_umask = umask(0);
        umask(my_umask);
        
        if (preserve_mode != PreserveMode::PermissionsOwnershipTimestamps)
            my_umask |= 06000;

        if (fchmod(dst_fd, src_stat.st_mode & ~my_umask) < 0)
            return CopyError { errno, false };

        if (preserve_mode == PreserveMode::PermissionsOwnershipTimestamps) {
            if (fchown(dst_fd, src_stat.st_uid, src_stat.st_gid) < 0)
                return CopyError { errno, false };

            struct utimbuf timbuf;
            timbuf.actime = src_stat.st_atime;
            timbuf.modtime = src_stat.st_mtime;
            if (utime(dst_path.characters(), &timbuf) < 0)
                return CopyError { errno, false };
        }

        return {};
    }

    /**
     * @param dst_path 
     * @param src_path 
     * @param src_stat 
     * @param link 
     * @param preserve_mode 
     * @return ErrorOr<void, File::CopyError> 
     */
    ErrorOr<void, File::CopyError> File::copy_directory(String const& dst_path, String const& src_path, struct stat const& src_stat, LinkMode link, PreserveMode preserve_mode)
    {
        if (mkdir(dst_path.characters(), 0755) < 0)
            return CopyError { errno, false };

        String src_rp = File::real_path_for(src_path);
        src_rp = String::formatted("{}/", src_rp);
        String dst_rp = File::real_path_for(dst_path);
        dst_rp = String::formatted("{}/", dst_rp);

        if (!dst_rp.is_empty() && dst_rp.starts_with(src_rp))
            return CopyError { errno, false };

        DirIterator di(src_path, DirIterator::SkipParentAndBaseDir);

        if (di.has_error())
            return CopyError { errno, false };

        while (di.has_next()) {
            String filename = di.next_path();
            auto result = copy_file_or_directory(
                String::formatted("{}/{}", dst_path, filename),
                String::formatted("{}/{}", src_path, filename),
                RecursionMode::Allowed, link, AddDuplicateFileMarker::Yes, preserve_mode);
            if (result.is_error())
                return result.error();
        }

        auto my_umask = umask(0);
        umask(my_umask);

        if (chmod(dst_path.characters(), src_stat.st_mode & ~my_umask) < 0)
            return CopyError { errno, false };

        if (preserve_mode == PreserveMode::PermissionsOwnershipTimestamps) {
            if (chown(dst_path.characters(), src_stat.st_uid, src_stat.st_gid) < 0)
                return CopyError { errno, false };

            struct utimbuf timbuf;
            timbuf.actime = src_stat.st_atime;
            timbuf.modtime = src_stat.st_atime;
            if (utime(dst_path.characters(), &timbuf) < 0)
                return CopyError { errno, false };
        }

        return {};
    }

    /**
     * @param dst_path 
     * @param src_path 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> File::link_file(String const& dst_path, String const& src_path)
    {
        int duplicate_count = 0;

        while (access(get_duplicate_name(dst_path, duplicate_count).characters(), F_OK) == 0) {
            ++duplicate_count;
        }

        if (duplicate_count != 0) {
            return link_file(get_duplicate_name(dst_path, duplicate_count), src_path);
        }

        if (symlink(src_path.characters(), dst_path.characters()) < 0)
            return Error::from_errno(errno);

        return {};
    }

    /**
     * @param path 
     * @param mode 
     * @param force 
     * @return ErrorOr<void, File::RemoveError> 
     */
    ErrorOr<void, File::RemoveError> File::remove(String const& path, RecursionMode mode, bool force)
    {
        struct stat path_stat;
        
        if (lstat(path.characters(), &path_stat) < 0) {
            if (!force)
                return RemoveError { path, errno };
            return {};
        }

        if (S_ISDIR(path_stat.st_mode) && mode == RecursionMode::Allowed) {
            auto di = DirIterator(path, DirIterator::SkipParentAndBaseDir);
            if (di.has_error())
                return RemoveError { path, di.error() };

            while (di.has_next()) {
                auto result = remove(di.next_full_path(), RecursionMode::Allowed, true);
                if (result.is_error())
                    return result.error();
            }

            if (rmdir(path.characters()) < 0 && !force)
                return RemoveError { path, errno };
        } else {
            if (unlink(path.characters()) < 0 && !force)
                return RemoveError { path, errno };
        }

        return {};
    }

} // namespace Core
