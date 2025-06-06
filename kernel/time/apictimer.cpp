/**
 * @file apictimer.cpp
 * @author Krisna Pranav aka (krishpranav)
 * @brief apic timer
 * @version 6.0
 * @date 2023-08-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/io.h>
#include <kernel/scheduler.h>
#include <kernel/thread.h>
#include <kernel/arch/i386/cpu.h>
#include <kernel/interrupt/apic.h>
#include <kernel/time/apictimer.h>
#include <kernel/time/timemanagement.h>

namespace Kernel 
{

    #define APIC_TIMER_MEASURE_CPU_CLOCK

    /**
     * @param interrupt_number 
     * @param calibration_source 
     * @return APICTimer* 
     */
    APICTimer* APICTimer::initialize(u8 interrupt_number, HardwareTimerBase& calibration_source)
    {
        auto* timer = new APICTimer(interrupt_number, nullptr);

        if (!timer->calibrate(calibration_source)) {
            delete timer;
            return nullptr;
        }

        return timer;
    }

    /**
     * @param interrupt_number 
     * @param callback 
     */
    APICTimer::APICTimer(u8 interrupt_number, Function<void(const RegisterState&)> callback)
        : HardwareTimer<GenericInterruptHandler>(interrupt_number, move(callback))
    {
        disable_remap();
    }

    /**
     * @param calibration_source 
     * @return true 
     * @return false 
     */
    bool APICTimer::calibrate(HardwareTimerBase& calibration_source)
    {
        ASSERT_INTERRUPTS_DISABLED();

        klog() << "APICTimer: Using " << calibration_source.model() << " as calibration source";

        auto& apic = APIC::the();

    #ifdef APIC_TIMER_MEASURE_CPU_CLOCK
        bool supports_tsc = Processor::current().has_feature(CPUFeature::TSC);
    #endif

        const size_t ticks_in_100ms = calibration_source.ticks_per_second() / 10;
        Atomic<size_t> calibration_ticks = 0;

    #ifdef APIC_TIMER_MEASURE_CPU_CLOCK
        volatile u64 start_tsc = 0, end_tsc = 0;
    #endif
        volatile u32 start_apic_count = 0, end_apic_count = 0;
        auto original_source_callback = calibration_source.set_callback([&](const RegisterState&) {
            u32 current_timer_count = apic.get_timer_current_count();
    #ifdef APIC_TIMER_MEASURE_CPU_CLOCK
            u64 current_tsc = supports_tsc ? read_tsc() : 0;
    #endif

            auto prev_tick = calibration_ticks.fetch_add(1, Mods::memory_order_acq_rel);
            if (prev_tick == 0) {
    #ifdef APIC_TIMER_MEASURE_CPU_CLOCK
                start_tsc = current_tsc;
    #endif
                start_apic_count = current_timer_count;
            } else if (prev_tick + 1 == ticks_in_100ms) {
    #ifdef APIC_TIMER_MEASURE_CPU_CLOCK
                end_tsc = current_tsc;
    #endif
                end_apic_count = current_timer_count;
            }
        });

        auto original_callback = set_callback([&](const RegisterState&) {
            klog() << "APICTimer: Timer fired during calibration!";
            ASSERT_NOT_REACHED(); 
        });
        apic.setup_local_timer(0xffffffff, APIC::TimerMode::Periodic, true);

        sti();

        while (calibration_ticks.load(Mods::memory_order_relaxed) < ticks_in_100ms)
            ;
        cli();

        calibration_source.set_callback(move(original_source_callback));
        set_callback(move(original_callback));

        disable_local_timer();

        auto delta_apic_count = start_apic_count - end_apic_count; 
        m_timer_period = (delta_apic_count * apic.get_timer_divisor()) / ticks_in_100ms;

        auto apic_freq = (delta_apic_count * apic.get_timer_divisor()) / apic.get_timer_divisor();
        if (apic_freq < 1000000) {
            klog() << "APICTimer: Frequency too slow!";
            return false;
        }
        klog() << "APICTimer: Bus clock speed: " << (apic_freq / 1000000) << "." << (apic_freq % 1000000) << " MHz";
    #ifdef APIC_TIMER_MEASURE_CPU_CLOCK
        if (supports_tsc) {
            auto delta_tsc = end_tsc - start_tsc;
            klog() << "APICTimer: CPU clock speed: " << (delta_tsc / 1000000) << "." << (delta_tsc % 1000000) << " MHz";
        }
    #endif

        m_frequency = calibration_source.ticks_per_second();

        enable_local_timer();

        return true;
    }

    /// @brief: enable local timer
    void APICTimer::enable_local_timer()
    {
        APIC::the().setup_local_timer(m_timer_period, m_timer_mode, true);
    }

    /// @breif: disable local timer
    void APICTimer::disable_local_timer()
    {
        APIC::the().setup_local_timer(0, APIC::TimerMode::OneShot, false);
    }

    /**
     * @return size_t 
     */
    size_t APICTimer::ticks_per_second() const
    {
        return m_frequency;
    }

    /// @breif: set periodic
    void APICTimer::set_periodic()
    {
        ASSERT_NOT_REACHED();
    }

    /// @breif: set non periodic 
    void APICTimer::set_non_periodic()
    {
        ASSERT_NOT_REACHED();
    }

    /// @breif: reset to default ticks per second 
    void APICTimer::reset_to_default_ticks_per_second()
    {
    }

    /**
     * @param frequency 
     * @return true 
     * @return false 
     */
    bool APICTimer::try_to_set_frequency(size_t frequency)
    {
        (void)frequency;
        return true;
    }

    /**
     * @param frequency 
     * @return true 
     * @return false 
     */
    bool APICTimer::is_capable_of_frequency(size_t frequency) const
    {
        (void)frequency;
        return false;
    }

    /**
     * @param frequency 
     * @return size_t 
     */
    size_t APICTimer::calculate_nearest_possible_frequency(size_t frequency) const
    {
        (void)frequency;
        return 0;
    }

} // namespace Kernel