/**
 * @file anonymousbuffer.cpp
 * @author Krisna Pranav
 * @brief anonymous buffer
 * @version 6.0
 * @date 2025-02-26
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/try.h>
#include <libcore/anonymousbuffer.h>
#include <libcore/system.h>
#include <libipc/file.h>
#include <fcntl.h>
#include <sys/mman.h>

namespace Core 
{

    /**
     * @param size 
     * @return ErrorOr<AnonymousBuffer> 
     */
    ErrorOr<AnonymousBuffer> AnonymousBuffer::create_with_size(size_t size)
    {
        auto fd = TRY(Core::System::anon_create(size, O_CLOEXEC));
        return create_from_anon_fd(fd, size);
    }

    /**
     * @param fd 
     * @param size 
     * @return ErrorOr<NonnullRefPtr<AnonymousBufferImpl>> 
     */
    ErrorOr<NonnullRefPtr<AnonymousBufferImpl>> AnonymousBufferImpl::create(int fd, size_t size)
    {
        auto* data = mmap(nullptr, round_up_to_power_of_two(size, PAGE_SIZE), PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, fd, 0);
        if (data == MAP_FAILED)
            return Error::from_errno(errno);
        return Mods::adopt_nonnull_ref_or_enomem(new (nothrow) AnonymousBufferImpl(fd, size, data));
    }

    /**
     * @brief Destroy the AnonymousBufferImpl::AnonymousBufferImpl object
     * 
     */
    AnonymousBufferImpl::~AnonymousBufferImpl()
    {
        if (m_fd != -1) {
            auto rc = close(m_fd);
            VERIFY(rc == 0);
        }
        auto rc = munmap(m_data, round_up_to_power_of_two(m_size, PAGE_SIZE));
        VERIFY(rc == 0);
    }

    /**
     * @param fd 
     * @param size 
     * @return ErrorOr<AnonymousBuffer> 
     */
    ErrorOr<AnonymousBuffer> AnonymousBuffer::create_from_anon_fd(int fd, size_t size)
    {
        auto impl = TRY(AnonymousBufferImpl::create(fd, size));
        return AnonymousBuffer(move(impl));
    }   

    /**
     * @brief Construct a new AnonymousBuffer::AnonymousBuffer object
     * 
     * @param impl 
     */
    AnonymousBuffer::AnonymousBuffer(NonnullRefPtr<AnonymousBufferImpl> impl)
        : m_impl(move(impl))
    {
    }

    /**
     * @brief Construct a new AnonymousBufferImpl::AnonymousBufferImpl object
     * 
     * @param fd 
     * @param size 
     * @param data 
     */
    AnonymousBufferImpl::AnonymousBufferImpl(int fd, size_t size, void* data)
        : m_fd(fd)
        , m_size(size)
        , m_data(data)
    {
    }

} // namespace Core
