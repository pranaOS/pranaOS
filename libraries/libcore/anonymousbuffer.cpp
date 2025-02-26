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