/**
 * @file partition.hpp
 * @author Krisna Pranav
 * @brief partition
 * @version 6.0
 * @date 2024-08-03
 *
 * @copyright Copyright (c) 2021 - 2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include "defs.hpp"
#include <string>

namespace purefs::blkdev
{
    
    struct partition
    {
        int physical_number{};
        int mbr_number{-1};
        sector_t start_sector{};
        std::size_t num_sectors{};
        bool bootable{};
        unsigned char boot_unit{};
        unsigned short type{};
        std::string name;
    }; // struct partition

} // namespace purefs::blkdev