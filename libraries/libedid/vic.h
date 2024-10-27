/**
 * @file vic.h
 * @author Krisna Pranav
 * @brief VIC
 * @version 6.0
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/fixedpoint.h>
#include <mods/optional.h>
#include <mods/types.h>

namespace EDID 
{
    class VIC final 
    {
    public:
        struct Details 
        {
            enum class ScanType : u8 
            {
                NonInterlaced,
                Interlaced
            }; // enum class ScanType : u8 

            enum class AspectRatio : u8 
            {
                AR_4_3,
                AR_16_9,
                AR_64_27,
                AR_256_135,
            }; // enum class AspectRatio : u8 

            u8 vic_id;
            u16 horizontal_pixels;
            u16 vertical_lines;
            u32 refresh_rate_millihz;
            
            ScanType scan_type;
            AspectRatio aspect_ratio;

            FixedPoint<16, u32> refresh_rate_hz() const;
        }; // struct Details 

        /**
         * @return Details const* 
         */
        static Details const* find_details_by_vic_id(u8);
    }; // class VIC final 
} // namespace EDID 