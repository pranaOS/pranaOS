/**
 * @file definitions.h
 * @author Krisna Pranav
 * @brief Definitions
 * @version 6.0
 * @date 2024-10-07
 * 
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

namespace Kernel::Graphics 
{

    struct Timings 
    {
        /**
         * @return size_t 
         */
        size_t blanking_start() const
        {
            return active;
        }
        
        size_t blanking_end() const
        {
            return total;
        }

        size_t active;
        size_t sync_start;
        size_t sync_end;
        size_t total;
    }; // struct Timings

    struct Modesetting 
    {
        size_t pixel_clock_in_khz;
        Timings horizontal;
        Timings vertical;
    }; // struct Modesettings

} // namespace Kernel::Graphics