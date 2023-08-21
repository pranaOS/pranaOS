/**
 * @file apictimer.cpp
 * @author Krisna Pranav aka (krishpranav)
 * @brief apic timer
 * @version 6.0
 * @date 2023-08-21
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "apictimer.h"
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
    APICTimer* APICTimer::initialize(u8 interrupt_number, HardwareTimer& calibration_source)
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

    /// @breif: enable local timer by setting it up.
    void APICTimer::enable_local_timer()
    {
        APIC::the().setup_local_timer(m_timer_period, m_timer_mode, true);
    }

    /// @brief: disable local timer
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

} // namespace Kernel