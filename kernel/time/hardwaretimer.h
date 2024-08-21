/**
 * @file hardwaretimer.h
 * @author Krisna Pranav
 * @brief hardware timer
 * @version 6.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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
    }; // enum HardwareTimerType

    template<typename InterruptHandlerType>
    class HardwareTimer;

    class HardwareTimerBase : public RefCounted<HardwareTimerBase> 
    {
    public:
        virtual ~HardwareTimerBase() { }

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

        /// @brief: set periodic + set non periodic
        virtual void set_periodic() = 0;
        virtual void set_non_periodic() = 0;

        /**
         * @return size_t 
         */
        virtual size_t ticks_per_second() const = 0;

        /// @brief: rest to deafult ticks per second
        virtual void reset_to_default_ticks_per_second() = 0;

        /**
         * @param frequency 
         * @return true 
         * @return false 
         */
        virtual bool try_to_set_frequency(size_t frequency) = 0;
        virtual bool is_capable_of_frequency(size_t frequency) const = 0;

        /**
         * @param frequency 
         * @return size_t 
         */
        virtual size_t calculate_nearest_possible_frequency(size_t frequency) const = 0;
    };

    template<>
    class HardwareTimer<IRQHandler>
        : public HardwareTimerBase
        , public IRQHandler {
    public:
        /**
         * @return const char* 
         */
        virtual const char* purpose() const override
        {
            if (TimeManagement::the().is_system_timer(*this))
                return "System Timer";

            return model();
        }

        /**
         * @param callback 
         * @return Function<void(const RegisterState&)> 
         */
        virtual Function<void(const RegisterState&)> set_callback(Function<void(const RegisterState&)> callback) override
        {
            disable_irq();

            auto previous_callback = move(m_callback);
            m_callback = move(callback);

            enable_irq();

            return previous_callback;
        }

    protected:

        /**
         * @param irq_number 
         * @param callback 
         */
        HardwareTimer(u8 irq_number, Function<void(const RegisterState&)> callback = nullptr)
            : IRQHandler(irq_number)
            , m_callback(move(callback))
        {
        }

        /**
         * @param regs 
         */
        virtual void handle_irq(const RegisterState& regs) override
        {
            if (m_callback)
                m_callback(regs);
        }

        u64 m_frequency { OPTIMAL_TICKS_PER_SECOND_RATE };

    private:
        Function<void(const RegisterState&)> m_callback;
    };

    template<>
    class HardwareTimer<GenericInterruptHandler>
        : public HardwareTimerBase
        , public GenericInterruptHandler {
    public:

        /**
         * @return const char* 
         */
        virtual const char* purpose() const override
        {
            return model();
        }

        /**
         * @param callback 
         * @return Function<void(const RegisterState&)> 
         */
        virtual Function<void(const RegisterState&)> set_callback(Function<void(const RegisterState&)> callback) override
        {
            auto previous_callback = move(m_callback);

            m_callback = move(callback);

            return previous_callback;
        }

        virtual size_t sharing_devices_count() const override 
        { 
            return 0; 
        }
        
        virtual bool is_shared_handler() const override 
        { 
            return false; 
        }

        virtual bool is_sharing_with_others() const 
        { 
            return false; 
        }

        /**
         * @return HandlerType 
         */
        virtual HandlerType type() const override 
        { 
            return HandlerType::IRQHandler; 
        }

        /**
         * @return const char* 
         */
        virtual const char* controller() const override 
        { 
            return nullptr; 
        }

        virtual bool eoi() override;

    protected:
        /**
         * @param irq_number 
         * @param callback 
         */
        HardwareTimer(u8 irq_number, Function<void(const RegisterState&)> callback = nullptr)
            : GenericInterruptHandler(irq_number)
            , m_callback(move(callback))
        {
        }

        /**
         * @param regs 
         */
        virtual void handle_interrupt(const RegisterState& regs) override
        {
            if (m_callback)
                m_callback(regs);
        }

        u64 m_frequency { OPTIMAL_TICKS_PER_SECOND_RATE };

    private:
        Function<void(const RegisterState&)> m_callback;
    }; // class HardwareTimer with GenericInput

} // namespace Kernel
