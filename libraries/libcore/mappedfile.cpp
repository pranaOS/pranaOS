/**
 * @file mappedfile.cpp
 * @author Krisna Pranav
 * @brief mapped file
 * @version 6.0
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/scopeguard.h>
#include <mods/string.h>
#include <libcore/mappedfile.h>
#include <libcore/system.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

namespace Core 
{

    /**
     * @param path 
     * @return ErrorOr<NonnullRefPtr<MappedFile>> 
     */
    ErrorOr<NonnullRefPtr<MappedFile>> MappedFile::map(String const& path)
    {
        auto fd = TRY(Core::System::open(path, O_RDONLY | O_CLOEXEC, 0));
        return map_from_fd_and_close(fd, path);
    }

    /**
     * @param fd 
     * @param path 
     * @return ErrorOr<NonnullRefPtr<MappedFile>> 
     */
    ErrorOr<NonnullRefPtr<MappedFile>> MappedFile::map_from_fd_and_close(int fd, [[maybe_unused]] String const& path)
    {
        TRY(Core::System::fcntl(fd, F_SETFD, FD_CLOEXEC));

        ScopeGuard fd_close_guard = [fd] {
            close(fd);
        };

        auto stat = TRY(Core::System::fstat(fd));
        auto size = stat.st_size;

        auto* ptr = TRY(Core::System::mmap(nullptr, size, PROT_READ, MAP_SHARED, fd, 0, 0, path));

        return adopt_ref(*new MappedFile(ptr, size));
    }

    /**
     * @brief Construct a new MappedFile::MappedFile object
     * 
     * @param ptr 
     * @param size 
     */
    MappedFile::MappedFile(void* ptr, size_t size)
        : m_data(ptr)
        , m_size(size)
    {
    }

    /**
     * @brief Destroy the MappedFile::MappedFile object
     * 
     */
    MappedFile::~MappedFile()
    {
        MUST(Core::System::munmap(m_data, m_size));
    }

} // namespace Core
