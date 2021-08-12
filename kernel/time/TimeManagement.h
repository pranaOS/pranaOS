/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/NonnullRefPtrVector.h>
#include <base/RefPtr.h>
#include <base/Time.h>
#include <base/Types.h>
#include <kernel/arch/x86/RegisterState.h>
#include <kernel/KResult.h>
#include <kernel/UnixTypes.h>

namespace Kernel {

#define OPTIMAL_TICKS_PER_SECOND_RATE 250
#define OPTIMAL_PROFILE_TICKS_PER_SECOND_RATE 1000

class HardwareTimerBase;

enum class TimePrecision {
    Coarse = 0,
    Precise
};

class TimeManagement {
    BASE_MAKE_ETERNAL;

public:
    TimeManagement();
    static void initialize(u32 cpu);
    static TimeManagement& the();

    static bool is_valid_clock_id(clockid_t);
    Time current_time(clockid_t) const;
    Time monotonic_time(TimePrecision = TimePrecision::Coarse) const;
    Time monotonic_time_raw() const
    {
        return monotonic_time(TimePrecision::Precise);
    }
    Time epoch_time(TimePrecision = TimePrecision::Precise) const;
    void set_epoch_time(Time);
    time_t ticks_per_second() const;
    time_t boot_time() const;

    bool is_system_timer(const HardwareTimerBase&) const;

    static void update_time(const RegisterState&);
    static void update_time_hpet(const RegisterState&);
    void increment_time_since_boot_hpet();
    void increment_time_since_boot();

    static bool is_hpet_periodic_mode_allowed();

    bool enable_profile_timer();
    bool disable_profile_timer();

    u64 uptime_ms() const;
    static Time now();

    timespec remaining_epoch_time_adjustment() const { return m_remaining_epoch_time_adjustment; }
    
    void set_remaining_epoch_time_adjustment(const timespec& adjustment) { m_remaining_epoch_time_adjustment = adjustment; }

    bool can_query_precise_time() const { return m_can_query_precise_time; }

private:
    bool probe_and_set_legacy_hardware_timers();
    bool probe_and_set_non_legacy_hardware_timers();
    Vector<HardwareTimerBase*> scan_and_initialize_periodic_timers();
    Vector<HardwareTimerBase*> scan_for_non_periodic_timers();
    NonnullRefPtrVector<HardwareTimerBase> m_hardware_timers;
    void set_system_timer(HardwareTimerBase&);
    static void system_timer_tick(const RegisterState&);

    static u64 scheduling_current_time(bool);

    Atomic<u32> m_update1 { 0 };
    u32 m_ticks_this_second { 0 };
    u64 m_seconds_since_boot { 0 };
    
    timespec m_epoch_time { 0, 0 };
    timespec m_remaining_epoch_time_adjustment { 0, 0 };
    Atomic<u32> m_update2 { 0 };

    u32 m_time_ticks_per_second { 0 }; 
    bool m_can_query_precise_time { false };
    bool m_updating_time { false };

    RefPtr<HardwareTimerBase> m_system_timer;
    RefPtr<HardwareTimerBase> m_time_keeper_timer;

    Atomic<u32> m_profile_enable_count { 0 };
    RefPtr<HardwareTimerBase> m_profile_timer;
};

}
