/**
 * @file blockdevice.h
 * @author Krisna Pranav
 * @brief block device
 * @version 1.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/device/device.h>

namespace Kernel
{

    class BlockDevice;

    class AsyncBlockDeviceRequest : public AsyncDeviceRequest
    {   
    public:
        enum RequestType
        {
            Read,
            Write,
        }; 

        /** 
         * @return RequestType 
         */
        RequestType request_type() const
        {
            return m_request_type;
        }

        /**
         * @return u32 
         */
        u32 block_index() const
        {
            return m_block_index;
        }

        /**
         * @return u32 
         */
        u32 block_count() const
        {
            return m_block_count;
        }

        /// @brief: buffer
        UserOrKernelBuffer& buffer 
        {
            return m_buffer;
        }

        /**
         * @return const UserOrKernelBuffer& 
         */
        const UserOrKernelBuffer& buffer() const
        {
            return m_buffer;
        }

        /**
         * @return size_t 
         */
        size_t buffer_size() const
        {
            return m_buffer_size;
        }

        /**
         * @return const char* 
         */
        virtual const char* name() const override
        {
            switch (m_request_type) {
            case Read:
                return "blockDeviceRequest (read)";
            case Write:
                return "BlockDeviceRequest (write)";
            default:
                ASSERT_NOT_REACHED();
            }
        }

    private:
        BlockDevice& m_block_device;

        const RequestType m_request_type;

        const u32 m_block_index;
        const u32 m_block_count;

        UserOrKernelBuffer m_buffer;

        const size_t m_buffer_size;
    }; // class AsyncBlockDeviceRequest

} // namespace Kernel