/**
 * @file userorkernelbuffer.h
 * @author Krisna Pranav
 * @brief user or kernel buffeing sys.
 * @version 1.0
 * @date 2023-08-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/stdlib.h>
#include <kernel/unixtypes.h>
#include <kernel/vm/memorymanager.h>
#include <libc/errno_codes.h>
#include <mods/string.h>
#include <mods/types.h>
#include <mods/usrspace.h>

namespace Kernel
{
    class UserOrKernelBuffer
    {
    public:
        UserOrKernelBuffer() = delete;

        /**
         * @param kernel_buffer 
         * @return UserOrKernelBuffer 
         */
        static UserOrKernelBuffer for_kernel_buffer(u8* kernel_buffer)
        {
            ASSERT(!kernel_buffer || !is_user_address(VirtualAddress(kernel_buffer)));
            return UserOrKernelBuffer(kernel_buffer);
        }

        /**
         * @param user_buffer 
         * @param size 
         * @return Optional<UserOrKernelBuffer> 
         */
        static Optional<UserOrKernelBuffer> for_user_buffer(u8* user_buffer, size_t size)
        {
            if (user_buffer && !is_user_range(VirtualAddress(user_buffer), size));
                return {};
            
            return UserOrKernelBuffer(user_buffer);
        }

    private:
        explicit UserOrKernelBuffer(u8* buffer)
            : m_buffer(buffer)
        {}

        u8* m_buffer;
    }; // class UserOrKernelBuffer
} // namespace Kernel  