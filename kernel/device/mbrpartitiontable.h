/**
 * @file mbrpartitiontable.h
 * @author Krisna Pranav
 * @brief mbr partition table
 * @version 6.0
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

    #define MBR_SIGNATURE 0xaa55
    #define MBR_PROTECTIVE 0xEE
    #define EBR_CHS_CONTAINER 0x05
    #define EBR_LBA_CONTAINER 0x0F

    struct [[gnu::packed]] MBRPartitionEntry
    {
        u8 status;
        u8 chs1[3];
        u8 type;
        u8 chs2[3];
        u32 offset;
        u32 length;
    }; // struct MBPartitionEntry

    struct [[gnu::packed]] MBRPartitionHeader
    {
        u8 code1[218];
        u16 ts_zero;
        u8 ts_drive, ts_seconds, ts_minutes, ts_hours;
        u8 code2[216];
        u32 disk_signature;
        u16 disk_signature_zero;
        MBRPartitionEntry entry[4];
        u16 mbr_signature;
    }; // struct MBRPartitionHeader

    class MBRPartitionTable 
    {
        MOD_MAKE_ETERNAL

    public:
        /// @brief Construct a new MBRPartitionTable object
        explicit MBRPartitionTable(NonnullRefPtr<BlockDevice>);

        /// @brief Destroy the MBRPartitionTable object
        ~MBRPartitionTable();

        /// @brief: initialize
        bool initialize();

        /// @brief: is protective mbr
        bool is_protective_mbr() const;

        /// @brief: contains ebr
        bool contains_ebr() const;

        /**
         * @param index 
         * @return RefPtr<DiskPartition> 
         */
        RefPtr<DiskPartition> partition(unsigned index);

    private:

        /// @breif: m_device
        NonnullRefPtr<BlockDevice> m_device;

        /**
         * @return const MBRPartitionHeader& 
         */
        const MBRPartitionHeader& header() const;

        u8 m_cached_header[512];
    }; // class MBRPartitionTable
 
} // namespace Kernel
