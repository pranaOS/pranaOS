/**
 * @file diskpartition.cpp
 * @author Krisna Pranav
 * @brief disk partition
 * @version 6.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

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
    
    /**
     * @param device 
     * @param block_offset 
     * @param block_limit 
     */
    DiskPartition::DiskPartition(BlockDevice& device, unsigned block_offset, unsigned block_limit)
        : BlockDevice(100, 0, device.block_size())
        , m_device(device)
        , m_block_offset(block_offset)
        , m_block_limit(block_limit)
    {
    }

    /// @brief Destroy the DiskPartition::DiskPartition object
    DiskPartition::~DiskPartition()
    { }

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

    #ifdef OFFD_DEBUG
        klog() << "DiskPartition::read offset=" << fd.offset() << " adjust=" << adjust << " len=" << len;
    #endif

        return m_device->read(fd, offset + adjust, outbuf, len);
    }

    /**
     * @param fd 
     * @param offset 
     * @return true 
     * @return false 
     */
    bool DiskPartition::can_read(const FileDescription& fd, size_t offset) const
    {
        unsigned adjust = m_block_offset * block_size();

    #ifdef OFFD_DEBUG
        klog() << "DiskPartition::can_read offset=" << offset << " adjust=" << adjust;
    #endif

        return m_device->can_read(fd, offset + adjust);
    }

    /**
     * @param fd 
     * @param offset 
     * @param inbuf 
     * @param len 
     * @return KResultOr<size_t> 
     */
    KResultOr<size_t> DiskPartition::write(FileDescription& fd, size_t offset, const UserOrKernelBuffer& inbuf, size_t len)
    {
        unsigned adjust = m_block_offset * block_size();

    #ifdef OFFD_DEBUG
        klog() << "DiskPartition::write offset=" << offset << " adjust=" << adjust << " len=" << len;
    #endif

        return m_device->write(fd, offset + adjust, inbuf, len);
    }

    /**
     * @param fd 
     * @param offset 
     * @return true 
     * @return false 
     */
    bool DiskPartition::can_write(const FileDescription& fd, size_t offset) const
    {
        unsigned adjust = m_block_offset * block_size();

    #ifdef OFFD_DEBUG
        klog() << "DiskPartition::can_write offset=" << offset << " adjust=" << adjust;
    #endif

        return m_device->can_write(fd, offset + adjust);
    }

    /**
     * @return const char* 
     */
    const char* DiskPartition::class_name() const
    {
        return "DiskPartition";
    }

} // namespace Kernel
