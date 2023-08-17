/**
 * @file hardwaretimer.h
 * @author Krisna Pranav
 * @brief hardware timer
 * @version 1.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/function.h>
#include <mods/refcounted.h>
#include <mods/string.h>
#include <kernel/interrupt/irqhandler.h>
#include <kernel/time/timemanagement.h>

namespace Kernel
{
        
    enum class HardwareTimerType
    {
        i8253 = 0x1,
        RTC = 0x2,
        HighPrecisionEventTimer = 0x3,
        LocalAPICTimer = 0x4
    };

    template<typename InterruptHandlerType>
    class HardwareType;

    class HardwareTimerBase : public RefCounted<HardwareTimerBase> 
    {
    public:

        /// @brief Destroy the HardwareTimerBase object
        virtual ~HardwareTimerBase() {}

        /**
         * @return const char* 
         */
        virtual const char* model() const = 0;

        /**
         * @return HardwareTimerType 
         */
        virtual HardwareTimerType timer_type() const = 0;

        /**
         * @return Function<void(const RegisterState&)> 
         */
        virtual Function<void(const RegisterState&)> set_callback(Function<void(const RegisterState&)>) = 0;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_periodic() const = 0;
        virtual bool is_periodic_capable() const = 0;

        /// @breif: set periodic (or) non
        virtual void set_periodic() = 0;
        virtual void set_non_periodic() = 0;

        /**
         * @return size_t 
         */
        virtual size_t ticks_per_second() const = 0;

    }; 

} // namespace Kernel