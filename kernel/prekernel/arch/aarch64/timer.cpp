/**
 * @file timer.cpp
 * @author Krisna Pranav
 * @brief Timer
 * @version 6.0
 * @date 2024-10-23
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/prekernel/arch/aarch64/mmio.h>
#include <kernel/prekernel/arch/aarch64/mailbox.h>
#include <kernel/prekernel/arch/aarch64/timer.h>
#include <kernel/prekernel/arch/aarch64/utils.h>

namespace Prekernel 
{

    struct TimerRegisters 
    {
        u32 control_and_status;
        u32 counter_low;
        u32 counter_high;
        u32 compare[4];
    }; // struct TimerRegisters

    enum FlagBits 
    {
        SystemTimerMatch0 = 1 << 0,
        SystemTimerMatch1 = 1 << 1,
        SystemTimerMatch2 = 1 << 2,
        SystemTimerMatch3 = 1 << 3,
    }; // enum FlagBits

    /**
     * @brief Construct a new Timer::Timer object
     * 
     */
    Timer::Timer()
        : m_registers(MMIO::the().peripheral<TimerRegisters>(0x3000))
    {
    }

    /**
     * @return Timer& 
     */
    Timer& Timer::the()
    {
        static Timer instance;
        return instance;
    }
    
    /**
     * @return u64 
     */
    u64 Timer::microseconds_since_boot()
    {
        u32 high = m_registers->counter_high;
        u32 low = m_registers->counter_low;

        if (high != m_registers->counter_high) 
        {
            high = m_registers->counter_high;
            low = m_registers->counter_low;
        }
        return (static_cast<u64>(high) << 32) | low;
    }

    class SetClockRateMboxMessage : Prekernel::Mailbox::Message 
    {
    public:
        u32 clock_id;
        u32 rate_hz;
        u32 skip_setting_turbo;

        /**
         * @brief Set the Clock Rate Mbox Message object
         * 
         */
        SetClockRateMboxMessage()
            : Prekernel::Mailbox::Message(0x0003'8002, 12)
        {
            clock_id = 0;
            rate_hz = 0;
            skip_setting_turbo = 0;
        }
    }; // class SetClockRateMboxMessage: Prekernel::MailBox::Message

    /**
     * @param clock_id 
     * @param rate_hz 
     * @param skip_setting_turbo 
     * @return u32 
     */
    u32 Timer::set_clock_rate(ClockID clock_id, u32 rate_hz, bool skip_setting_turbo)
    {
        struct __attribute__((aligned(16))) 
        {
            Prekernel::Mailbox::MessageHeader header;
            SetClockRateMboxMessage set_clock_rate;
            Prekernel::Mailbox::MessageTail tail;
        } message_queue;

        message_queue.set_clock_rate.clock_id = static_cast<u32>(clock_id);
        message_queue.set_clock_rate.rate_hz = rate_hz;
        message_queue.set_clock_rate.skip_setting_turbo = skip_setting_turbo ? 1 : 0;

        if (!Prekernel::Mailbox::the().send_queue(&message_queue, sizeof(message_queue))) {
            warnln("Timer::set_clock_rate: failed!");
            return 0;
        }

        return message_queue.set_clock_rate.rate_hz;
    }
} // namespace Prekernel
