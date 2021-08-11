/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/arch/x86/InterruptDisabler.h>
#include <kernel/CMOS.h>
#include <kernel/IO.h>
#include <kernel/time/RTC.h>
#include <kernel/time/TimeManagement.h>

namespace Kernel {
#define IRQ_TIMER 8
#define MAX_FREQUENCY 8000

NonnullRefPtr<RealTimeClock> RealTimeClock::create(Function<void(const RegisterState&)> callback)
{
    return adopt_ref(*new RealTimeClock(move(callback)));
}
RealTimeClock::RealTimeClock(Function<void(const RegisterState&)> callback)
    : HardwareTimer(IRQ_TIMER, move(callback))
{
    InterruptDisabler disabler;
    NonMaskableInterruptDisabler nmi_disabler;
    enable_irq();
    CMOS::write(0x8B, CMOS::read(0xB) | 0x40);
    reset_to_default_ticks_per_second();
}
bool RealTimeClock::handle_irq(const RegisterState& regs)
{
    auto result = HardwareTimer::handle_irq(regs);
    CMOS::read(0x8C);
    return result;
}

size_t RealTimeClock::ticks_per_second() const
{
    return m_frequency;
}

void RealTimeClock::reset_to_default_ticks_per_second()
{
    InterruptDisabler disabler;
    bool success = try_to_set_frequency(1024);
    VERIFY(success);
}


static int quick_log2(size_t number)
{
    int count = 0;
    while (number >>= 1)
        count++;
    return count;
}

bool RealTimeClock::try_to_set_frequency(size_t frequency)
{
    InterruptDisabler disabler;
    if (!is_capable_of_frequency(frequency))
        return false;
    disable_irq();
    u8 previous_rate = CMOS::read(0x8A);
    u8 rate = quick_log2(32768 / frequency) + 1;
    dbgln("RTC: Set rate to {}", rate);
    CMOS::write(0x8A, (previous_rate & 0xF0) | rate);
    m_frequency = frequency;
    dbgln("RTC: Set frequency to {} Hz", frequency);
    enable_irq();
    return true;
}
bool RealTimeClock::is_capable_of_frequency(size_t frequency) const
{
    VERIFY(frequency != 0);
    if (frequency > MAX_FREQUENCY)
        return false;
    if (32768 % frequency)
        return false;

    u16 divider = 32768 / frequency;
    return (divider <= 16384 && divider >= 4); 
}
size_t RealTimeClock::calculate_nearest_possible_frequency(size_t frequency) const
{
    VERIFY(frequency != 0);
    return frequency;
}

}
