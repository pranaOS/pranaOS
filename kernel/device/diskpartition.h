/**
 * @file diskpartition.h
 * @author Krisna Pranav
 * @brief disk partition
 * @version 1.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

// #include "blockdevice.h"
#include "blockdevice.h"
#include <mods/refptr.h>
#include <kernel/device/blockdevice.h>

namespace Kernel 
{

    class DiskPartition final : public BlockDevice 
    {
    public:

        /**
         * @param block_offset 
         * @param block_limit 
         * @return NonnullRefPtr<DiskPartition> 
         */
        static NonnullRefPtr<DiskPartition> create(BlockDevice&, unsigned block_offset, unsigned block_limit);

        /// @brief Destroy the DiskPartition object
        virtual ~DiskPartition();

        /// @brief: start_request
        virtual void start_request(AsyncBlockDeviceRequest&) override;

        /**
         * @return KResultOr<size_t> 
         */
        virtual KResultOr<size_t> read(FileDescription&, size_t, UserOrKernelBuffer&, size_t) override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool can_read(const FileDescription&, size_t) const override;

        /**
         * @return KResultOr<size_t> 
         */
        virtual KResultOr<size_t> write(FileDescription&, size_t, const UserOrKernelBuffer&, size_t) override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool can_write(const FileDescription&, size_t) const override;

    private:

        /**
         * @return const char* 
         */
        virtual const char* class_name() const override;

        /**
         * @param block_offset 
         * @param block_limit 
         */
        DiskPartition(BlockDevice&, unsigned block_offset, unsigned block_limit);

        NonnullRefPtr<BlockDevice> m_device;

        unsigned m_block_offset;
        unsigned m_block_limit;

    }; // class DiskPartition

} // namespace Kernel
