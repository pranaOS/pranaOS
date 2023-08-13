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

    private:
        BlockDevice& m_block_device;

        const RequestType m_request_type;

        const u32 m_block_index;
        const u32 m_block_count;

        UserOrKernelBuffer m_buffer;

        const size_t m_buffer_size;
    }; // class AsyncBlockDeviceRequest

} // namespace Kernel