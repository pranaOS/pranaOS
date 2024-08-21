/**
 * @file RTC.h
 * @author Krisna Pranav
 * @brief RTC
 * @version 6.0
 * @date 2023-06-23
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "../kstd/unix_types.h"
#include "../interrupt/irqhandler.h"
#include "../kstd/kstddef.h"
#include "timekeeper.h"

#define RTC_IRQ 0x8

#define CMOS_SECONDS 0x0
#define CMOS_MINUTES 0x2
#define CMOS_HOURS 0x4
#define CMOS_WEEKDAY 0x6
#define CMOS_DAY 0x7
#define CMOS_MONTH 0x8
#define CMOS_YEAR 0x9
#define CMOS_CENTURY 0x32
#define CMOS_STATUS_A 0x0A
#define CMOS_STATUS_B 0x0B
#define CMOS_STATUS_C 0x0C
#define CMOS_STATUS_UPDATE_IN_PROGRESS  0x80
#define CMOS_SQUARE_WAVE_INTERRUPT_FLAG 0x40

#define RTC_FREQUENCY 1024
#define RTC_FREQUENCYVAL_MIN 2
#define RTC_FREQUENCYVAL_MAX 14
#define RTC_FREQUENCY_DIVIDER 32768

#define bcd(val) ((val / 16) * 10 + (val & 0xf))

class RTC: public IRQHandler, public TimeKeeper {
public:
    /**
     * @brief Construct a new RTC object
     * 
     * @param time 
     */
    RTC(TimeManager* time);

    /**
     * @brief timestamp
     * 
     * @return time_t 
     */
    static time_t timestamp();

    /**
     * @brief handle_irq
     * 
     * @param regs 
     */
    void handle_irq(Registers* regs) override;

    /**
     * @brief mark_in_req
     * 
     * @return true 
     * @return false 
     */
    bool mark_in_irq() override;

    int frequency() override;
    void enable() override;
    void disable() override;

private:
    /**
     * @brief Set the frequency object
     * 
     * @param frequency 
     * @return true 
     * @return false 
     */
    bool set_frequency(int frequency);

    int _timestamp = 0;
    int _frequency = 0;
};