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
        /**
         * @return APICTimer* 
         */
        static APICTimer* initialize(u8, HardwareTimerBase&);

        /**
         * @return HardwareTimerType 
         */
        virtual HardwareTimerType timer_type() const override 
        { 
            return HardwareTimerType::LocalAPICTimer; 
        }

        /**
         * @return const char* 
         */
        virtual const char* model() const override 
        { 
            return "LocalAPIC"; 
        }

        /**
         * @return size_t 
         */
        virtual size_t ticks_per_second() const override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_periodic() const override 
        { 
            return m_timer_mode == APIC::TimerMode::Periodic; 
        }

        virtual bool is_periodic_capable() const override 
        { 
            return true; 
        }

        /// @brief: set periodic & non periodic
        virtual void set_periodic() override;
        virtual void set_non_periodic() override;

        /// @brief: rest
        virtual void reset_to_default_ticks_per_second() override;

        /**
         * @param frequency 
         * @return true 
         * @return false 
         */
        virtual bool try_to_set_frequency(size_t frequency) override;
        virtual bool is_capable_of_frequency(size_t frequency) const override;

        /**
         * @param frequency 
         * @return size_t 
         */
        virtual size_t calculate_nearest_possible_frequency(size_t frequency) const override;


        /// @breif: [enable/disable] local timer
        void enable_local_timer();
        void disable_local_timer();

    private:
        /// @brief Construct a new APICTimer object
        explicit APICTimer(u8, Function<void(const RegisterState&)>);

        /**
         * @return true 
         * @return false 
         */
        bool calibrate(HardwareTimerBase&);

        u32 m_timer_period { 0 };

        APIC::TimerMode m_timer_mode { APIC::TimerMode::Periodic };

    }; // class APICTimer

} // namespace Kernel
