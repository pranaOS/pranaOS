/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <drivers/x86/pit.h>
#include <libkernel/kassert.h>
#include <libkernel/log.h>
#include <platform/generic/system.h>
#include <tasking/cpu.h>
#include <tasking/sched.h>
#include <time/time_manager.h>

static int ticks_to_sched = 0;
static int second = TIMER_TICKS_PER_SECOND;
static int _pit_set_frequency(uint16_t freq);

static int _pit_set_frequency(uint16_t freq)
{
    system_disable_interrupts();
    uint32_t divisor = PIT_BASE_FREQ / freq;
    if (divisor > 0xffff) {
        return -1;
    }
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);
    port_byte_out(0x43, 0x36); 
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
    system_enable_interrupts();
    return 0;
}

void pit_setup()
{
    int res = _pit_set_frequency(TIMER_TICKS_PER_SECOND);
    if (res < 0) {
        kpanic("Pit: failed to set freq");
    }
    set_irq_handler(IRQ0, pit_handler);
}

void pit_handler()
{
    cpu_tick();
    timeman_timer_tick();
    sched_tick();
}