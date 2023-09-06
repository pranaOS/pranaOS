/**
 * @file userorkernelbuffer.cpp
 * @author Krisna Pranav
 * @brief user or kernel buffer
 * @version 1.0
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

        return String(ReadOnlyBytes { m_buffer, size });
    }

}