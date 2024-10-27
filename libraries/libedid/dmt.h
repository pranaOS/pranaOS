/**
 * @file dmt.h
 * @author Krisna Pranav
 * @brief DMT
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
    class DMT final 
    {
    public:
        struct CVT 
        {
            u8 bytes[3];
        }; // struct CVT 

        struct MonitorTiming 
        {
            enum class ScanType : u8 
            {
                NonInterlaced,
                Interlaced
            }; // enum class ScanType : u8 

            enum class CVTCompliance : u8 
            {
                NotCompliant,
                Compliant,
                CompliantReducedBlanking,
                CompliantReducedBlankingV2,
            }; // enum class CVTCompliance : u8 

            u8 dmt_id;
            u8 std_bytes[2];
            u8 char_width_pixels;
            u16 horizontal_pixels;
            u16 vertical_lines;
            u32 horizontal_frequency_hz;
            u32 vertical_frequency_millihz;
            u32 pixel_clock_khz;
            u8 horizontal_front_porch_pixels;
            u8 vertical_front_porch_lines;
            u16 horizontal_blank_pixels;
            u16 vertical_blank_lines;
            u16 horizontal_sync_time_pixels;
            u8 vertical_sync_time_lines;
            CVTCompliance cvt_compliance { CVTCompliance::NotCompliant };
            u8 cvt_bytes[3] {};
            ScanType scan_type { ScanType::NonInterlaced };

            /**
             * @return ALWAYS_INLINE 
             */
            ALWAYS_INLINE bool has_std() const 
            { 
                return std_bytes[0] != 0; 
            }

            /**
             * @return ALWAYS_INLINE 
             */
            ALWAYS_INLINE bool has_cvt() const 
            { 
                return cvt_bytes[0] != 0; 
            }

            /**
             * @return FixedPoint<16, u32> 
             */
            FixedPoint<16, u32> horizontal_frequency_khz() const;

            /**
             * @return FixedPoint<16, u32> 
             */
            FixedPoint<16, u32> vertical_frequency_hz() const;
            u32 refresh_rate_hz() const;
    #ifndef KERNEL
            String name() const;
    #endif
        }; // struct MonitorTiming 

        /**
         * @return MonitorTiming const* 
         */
        static MonitorTiming const* find_timing_by_dmt_id(u8);

        /**
         * @return MonitorTiming const* 
         */
        static MonitorTiming const* find_timing_by_std_id(u8, u8);

        /**
         * @return MonitorTiming const* 
         */
        static MonitorTiming const* find_timing_by_cvt(CVT);
    }; // class DMT final 
} // namespace EDID 