/**
 * @file timer.h
 * @author Krisna Pranav
 * @brief Timer
 * @version 6.0
 * @date 2024-10-21
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/types.h>

namespace Prekernel
{
  struct TimerRegisters;

  class Timer
  {
    public:
      /**
       * @return Timer& 
       */
      static Timer& the();

      u64 microseconds_since_boot();

      enum class ClockID
      {
          Reserved = 0,
          EMMC = 1,
          UART = 2,
          ARM = 3,
          CORE = 4,
          V3D = 5,
          H264 = 6,
          ISP = 7,
          SDRAM = 8,
          PIXEL = 9,
          PWM = 10,
          HEVC = 11,
          EMMC2 = 12,
          M2MC = 13,
          PIXEL_BVB = 14,
      }; // enum class ClockID

      /**
       * @brief Set the clock rate object
       * 
       * @param rate_hz 
       * @param skip_setting_turbo 
       * @return u32 
       */
      u32 set_clock_rate(ClockID, u32 rate_hz, bool skip_setting_turbo = true);

    private:
      /**
       * @brief Construct a new Timer object
       * 
       */
      Timer();

      TimerRegisters volatile* m_registers;
  }; // class Timer
} // namespace Prekernel
