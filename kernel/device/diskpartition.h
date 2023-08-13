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
         * @return KResultOr<size_t> 
         */
        virtual KResultOr<size_t> read(FileDescription&, size_t, UserOrKernelBuffer&) const override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool can_read(const FileDescription&, size_t) const override;

        /**
         * @return KResultOr<size_t> 
         */
        virtual KResultOr<size_t> write(FileDescription&, size_t, const UserOrKernelBuffer&) override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool can_write(const FileDescription&, size_t) const override;

    private:
        NonnullRefPtr<BlockDevice> m_device;

        unsigned m_block_offset;
        unsigned m_block_limit;
    }; // class DiskPartition

} // namespace Kernel