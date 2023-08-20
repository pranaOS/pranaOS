/**
 * @file apictimer.h
 * @author Krisna Pranav aka (krishpranav)
 * @brief apic timer
 * @version 6.0
 * @date 2023-08-20
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/nonnullrefptr.h>
#include <mods/types.h>
#include <kernel/interrupt/genericinterrupthandler.h>
#include <kernel/time/hardwaretimer.h>

namespace Kernel
{

    class APICTimer final : public HardwareTimer<GenericInterruptHandler>
    {
    public:
        static APICTimer* initialize(u8, HardwareTimerBase&);

        /**

         * @return HardwareTimerType 
         */
        virtual HardwareTimerType timer_type() const override
        {
            return HardwareTimer::LocalAPICTimer;
        }

    private:
        explicit APICTimer(u8, Function<void(const RegisterState&)>);

        bool calibrate(HardwareTimerBase&);

        u32 m_timer_period { 0 };

        APIC::TimerMode m_timer_mode { APIC::TimerMode::Periodic };
    }; // class APICTimer

} // namespace Kernel