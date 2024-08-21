/**
 * @file timemanagement.h
 * @author Krisna Pranav
 * @brief TimeManagement
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/nonnullrefptrvector.h>
#include <mods/refptr.h>
#include <mods/types.h>
#include <kernel/kresult.h>
#include <kernel/unixtypes.h>

namespace Kernel 
{

    #define OPTIMAL_TICKS_PER_SECOND_RATE 1000

    class HardwareTimerBase;

    class TimeManagement 
    {
        MOD_MAKE_ETERNAL;

    public:
        TimeManagement();

        /// @brief: initialize;
        static bool initialized();

        /// @param cpu
        static void initialize(u32 cpu);
        
        static TimeManagement& the();

        /**
         * @param ticks 
         * @param ticks_per_second 
         * @return timespec 
         */
        static timespec ticks_to_time(u64 ticks, time_t ticks_per_second);

        /**
         * @param tspec 
         * @param ticks_per_second 
         * @return u64 
         */
        static u64 time_to_ticks(const timespec& tspec, time_t ticks_per_second);

        /**
         * @param clock_id 
         * @return KResultOr<timespec> 
         */
        KResultOr<timespec> current_time(clockid_t clock_id) const;

        /**
         * @return timespec 
         */
        timespec monotonic_time() const;

        /**
         * @return timespec 
         */
        timespec epoch_time() const;

        void set_epoch_time(timespec);

        /**
         * @return time_t 
         */
        time_t ticks_per_second() const;

        /**
         * @return time_t 
         */
        time_t boot_time() const;

        /**
         * @return true 
         * @return false 
         */
        bool is_system_timer(const HardwareTimerBase&) const;

        /// @brief: update_time
        static void update_time(const RegisterState&);

        /// @brief: increment_time from boot
        void increment_time_since_boot(const RegisterState&);

        static bool is_hpet_periodic_mode_allowed();

        /**
         * @return u64 
         */
        u64 uptime_ms() const;

        /**
         * @return u64 
         */
        u64 monotonic_ticks() const;

        /**
         * @return timeval 
         */
        static timeval now_as_timeval();

        /**
         * @return timespec 
         */
        timespec remaining_epoch_time_adjustment() const 
        { 
            return m_remaining_epoch_time_adjustment; 
        }

        /**
         * @param adjustment 
         */
        void set_remaining_epoch_time_adjustment(const timespec& adjustment) 
        { 
            m_remaining_epoch_time_adjustment = adjustment; 
        }

    private:
        bool probe_and_set_legacy_hardware_timers();
        bool probe_and_set_non_legacy_hardware_timers();
        Vector<HardwareTimerBase*> scan_and_initialize_periodic_timers();
        Vector<HardwareTimerBase*> scan_for_non_periodic_timers();
        NonnullRefPtrVector<HardwareTimerBase> m_hardware_timers;
        void set_system_timer(HardwareTimerBase&);
        static void timer_tick(const RegisterState&);

        Atomic<u32> m_update1 { 0 };
        u32 m_ticks_this_second { 0 };
        u32 m_seconds_since_boot { 0 };
        timespec m_epoch_time { 0, 0 };
        timespec m_remaining_epoch_time_adjustment { 0, 0 };
        Atomic<u32> m_update2 { 0 };

        RefPtr<HardwareTimerBase> m_system_timer;
        RefPtr<HardwareTimerBase> m_time_keeper_timer;
    }; // class TimeManagement

}
