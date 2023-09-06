/**
 * @file userorkernelbuffer.cpp
 * @author Krisna Pranav
 * @brief user or kernel buffer
 * @version 6.0
 * @date 2023-09-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/userorkernelbuffer.h>
#include <kernel/vm/memorymanager.h>

namespace Kernel 
{
    /**
     * @return true 
     * @return false 
     */
    bool UserOrKernelBuffer::is_kernel_buffer() const
    {
        return !is_user_address(VirtualAddress(m_buffer));
    }

    /**
     * @param size 
     * @return String 
     */
    String UserOrKernelBuffer::copy_into_string(size_t size) const
    {
        if (!m_buffer)
            return {};

        if (is_user_address(VirtualAddress(m_buffer))) {
            char* buffer;
            auto data_copy = StringImpl::create_uninitialized(size, buffer);

            if (!copy_from_user(buffer, m_buffer, size))
                return {};

            return data_copy;
        }

        return String(ReadonlyBytes { m_buffer, size });
    }

    /**
     * @param src 
     * @param offset 
     * @param len 
     * @return true 
     * @return false 
     */
    bool UserOrKernelBuffer::write(const void* src, size_t offset, size_t len)
    {
        if (!m_buffer)
            return false;

        if (is_user_address(VirtualAddress(m_buffer)))
            return copy_to_user(m_buffer + offset, src, len);

        memcpy(m_buffer + offset, src, len);

        return true;
    }

    /**
     * @param dest 
     * @param offset 
     * @param len 
     * @return true 
     * @return false 
     */
    bool UserOrKernelBuffer::read(void* dest, size_t offset, size_t len) const
    {
        if (!m_buffer)
            return false;

        if (is_user_address(VirtualAddress(m_buffer)))
            return copy_from_user(dest, m_buffer + offset, len);

        memcpy(dest, m_buffer + offset, len);

        return true;
    }

    /**
     * @param value 
     * @param offset 
     * @param len 
     * @return true 
     * @return false 
     */
    bool UserOrKernelBuffer::memset(int value, size_t offset, size_t len)
    {
        if (!m_buffer)
            return false;

        if (is_user_address(VirtualAddress(m_buffer)))
            return memset_user(m_buffer + offset, value, len);

        ::memset(m_buffer + offset, value, len);

        return true;
    }
} // namespace Kernel