/**
 * @file defs.hpp
 * @author Krisna Pranav
 * @brief defs
 * @version 6.0
 * @date 2024-08-01
 *
 * @copyright Copyright (c) 2021 - 2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <cstdint>
#include <memory>

namespace purefs::blkdev
{
    using sector_t = uint64_t;
    using scount_t = int64_t;
    using hwpart_t = uint8_t;
    using part_t = int16_t;

    constexpr hwpart_t default_hw_partition = 0;

    namespace internal
    {
        class disk_handle;
    } // namespace internal

    using disk_fd = std::shared_ptr<internal::disk_handle>;

    enum class media_status
    {
        healthy,
        uninit,
        nomedia,
        wprotect,
        error,
    }; // enum class media_status

    enum class info_type
    {
        sector_count,
        sector_size,
        erase_block,
        start_sector
    }; // enum class info_type

    enum class pm_state
    {
        active,
        low_power,
        suspend,
        force_suspend,
        power_off
    }; // enum class pm_state

    struct flags
    {
        enum _flags
        {
            no_parts_scan = 0x1
        }; // enum _flags
    }; // struct flags
} // namespace purefs::blkdev