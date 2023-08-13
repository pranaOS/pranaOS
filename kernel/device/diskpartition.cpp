/**
 * @file diskpartition.cpp
 * @author Krisna Pranav
 * @brief disk partition
 * @version 1.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include "blockdevice.h"
#include "diskpartition.h"
#include <kernel/device/diskpartition.h>
#include <kernel/filesystem/filedescription.h>

namespace Kernel
{       
    /**
     * @param device 
     * @param block_offset 
     * @param block_limit 
     * @return NonnullRefPtr<DiskPartition> 
     */
    NonnullRefPtr<DiskPartition> DiskPartition::create(BlockDevice& device, unsigned block_offset, unsigned block_limit)
    {
        return adopt(*new DiskPartition(device, block_offset, block_limit));
    }

    DiskPartition::~DiskPartition() 
    {}

    /**
     * @param request 
     */
    void DiskPartition::start_request(AsyncBlockDeviceRequest& request)
    {
        request.add_sub_request(m_device->make_request<AsyncBlockDeviceRequest>(request.request_type(),
            request.block_index() + m_block_offset, request.block_count(), request.buffer(), request.buffer_size()));
    }

    /**
     * @param fd 
     * @param offset 
     * @param outbuf 
     * @param len 
     * @return KResultOr<size_t> 
     */
    KResultOr<size_t> DiskPartition::read(FileDescription& fd, size_t offset, UserOrKernelBuffer& outbuf, size_t len)
    {
        unsigned adjust = m_block_offset * block_size();

        return m_device->read(fd, offset + adjust, outbuf, len);
    }

    /**
     * @return const char* 
     */
    const char* DiskPartition::class_name() const
    {
        return "DiskPartition";
    }

} // namespace Kernel