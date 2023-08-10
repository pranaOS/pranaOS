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
            if (user_buffer && !is_user_range(VirtualAddress(user_buffer), size))
                return {};

            return UserOrKernelBuffer(user_buffer);
        }

        /**
         * @tparam UserspaceType 
         * @param userspace 
         * @param size 
         * @return Optional<UserOrKernelBuffer> 
         */
        template<typename UserspaceType>
        static Optional<UserOrKernelBuffer> for_user_buffer(UserspaceType userspace, size_t size)
        {
            if (!is_user_range(VirtualAddress(userspace.unsafe_userspace_ptr()), size))
                return {};

            return UserOrKernelBuffer(const_cast<u8*>((const u8*)userspace.unsafe_userspace_ptr()));
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_kernel_buffer() const;

        /**
         * @return const void* 
         */
        const void* user_or_kernel_ptr() const 
        { 
            return m_buffer; 
        }

        /**
         * @param offset 
         * @return UserOrKernelBuffer 
         */
        UserOrKernelBuffer offset(ssize_t offset) const
        {
            if (!m_buffer)
                return *this;

            UserOrKernelBuffer offset_buffer = *this;
            offset_buffer.m_buffer += offset;
            ASSERT(offset_buffer.is_kernel_buffer() == is_kernel_buffer());

            return offset_buffer;
        }
        
        /**
         * @param size 
         * @return String 
         */
        String copy_into_string(size_t size) const;

        /**
         * @param src 
         * @param offset 
         * @param len 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool write(const void* src, size_t offset, size_t len);

        /**
         * @param src 
         * @param len 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool write(const void* src, size_t len)
        {
            return write(src, 0, len);
        }

        /**
         * @param bytes 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool write(ReadonlyBytes bytes)
        {
            return write(bytes.data(), bytes.size());
        }

        /**
         * @param dest 
         * @param offset 
         * @param len 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool read(void* dest, size_t offset, size_t len) const;

        /**
         * @param dest 
         * @param len 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool read(void* dest, size_t len) const
        {
            return read(dest, 0, len);
        }
        
        /**
         * @param bytes 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool read(Bytes bytes) const
        {
            return read(bytes.data(), bytes.size());
        }

        /**
         * @param value 
         * @param offset 
         * @param len 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool memset(int value, size_t offset, size_t len);

        /**
         * @param value 
         * @param len 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool memset(int value, size_t len)
        {
            return memset(value, 0, len);
        }

        /**
         * @tparam BUFFER_BYTES 
         * @tparam F 
         * @param offset 
         * @param len 
         * @param f 
         * @return ssize_t 
         */
        template<size_t BUFFER_BYTES, typename F>
        [[nodiscard]] ssize_t write_buffered(size_t offset, size_t len, F f)
        {
            if (!m_buffer)
                return -EFAULT;

            if (is_kernel_buffer()) {
                return f(m_buffer + offset, len);
            }

            u8 buffer[BUFFER_BYTES];

            size_t nwritten = 0;

            while (nwritten < len) {
                auto to_copy = min(sizeof(buffer), len - nwritten);
                ssize_t copied = f(buffer, to_copy);

                if (copied < 0)
                    return copied;

                ASSERT((size_t)copied <= to_copy);

                if (!write(buffer, nwritten, (size_t)copied))
                    return -EFAULT;

                nwritten += (size_t)copied;

                if ((size_t)copied < to_copy)
                    break;
            }

            return (ssize_t)nwritten;
        }

        /**
         * @tparam BUFFER_BYTES 
         * @tparam F 
         * @param len 
         * @param f 
         * @return ssize_t 
         */
        template<size_t BUFFER_BYTES, typename F>
        [[nodiscard]] ssize_t write_buffered(size_t len, F f)
        {
            return write_buffered<BUFFER_BYTES, F>(0, len, f);
        }       

        /**
         * @tparam BUFFER_BYTES 
         * @tparam F 
         * @param offset 
         * @param len 
         * @param f 
         * @return ssize_t 
         */
        template<size_t BUFFER_BYTES, typename F>
        [[nodiscard]] ssize_t read_buffered(size_t offset, size_t len, F f) const
        {
            if (!m_buffer)
                return -EFAULT;

            if (is_kernel_buffer()) {
                return f(m_buffer + offset, len);
            }

            u8 buffer[BUFFER_BYTES];

            size_t nread = 0;

            while (nread < len) {
                auto to_copy = min(sizeof(buffer), len - nread);

                if (!read(buffer, nread, to_copy))
                    return -EFAULT;

                ssize_t copied = f(buffer, to_copy);
                if (copied < 0)
                    return copied;

                ASSERT((size_t)copied <= to_copy);
                nread += (size_t)copied;

                if ((size_t)copied < to_copy)
                    break;
            }

            return nread;
        }

        /**
         * @tparam BUFFER_BYTES 
         * @tparam F 
         * @param len 
         * @param f 
         * @return ssize_t 
         */
        template<size_t BUFFER_BYTES, typename F>
        [[nodiscard]] ssize_t read_buffered(size_t len, F f) const
        {
            return read_buffered<BUFFER_BYTES, F>(0, len, f);
        }

    private:
        /**
         * @param buffer 
         */
        explicit UserOrKernelBuffer(u8* buffer)
            : m_buffer(buffer)
        { }

        u8* m_buffer;

    }; // class UserOrKernelBuffer

} // namespace Kernel
