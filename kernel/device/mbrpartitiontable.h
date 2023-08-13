/**
 * @file mbrpartitiontable.h
 * @author Krisna Pranav
 * @brief mbr partition table
 * @version 1.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/refptr.h>
#include <mods/vector.h>
#include <kernel/device/diskpartition.h>

namespace Kernel
{
    struct [[gnu::packed]] MBRPartitionEntry
    {
        u8 status;
        u8 chs1[3];
        u8 type;
        u8 chs2[3];
        u8 offset;
    };

    class MRPartitionTable
    {
        MOD_MAKE_ETERNAL

    private:
        NonnullRefPtr<BlockDevice> m_device;
    }
} // namespace Kernel